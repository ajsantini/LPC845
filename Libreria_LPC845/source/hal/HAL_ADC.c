/**
 * @file HAL_ADC.c
 * @brief Funciones a nivel de aplicacion del periferico ADC (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <stddef.h>
#include <HAL_ADC.h>
#include <HAL_SYSCON.h>
#include <HPL_ADC.h>
#include <HPL_SYSCON.h>
#include <HPL_SWM.h>
#include <HPL_NVIC.h>

#define	ADC_MAX_FREQ			((uint32_t) 1.2e6) //<! Maxima frecuencia de conversion admitida por el ADC
#define	ADC_CHANNEL_AMOUNT		12

static void dummy_irq_callback(void);

static void (*adc_seq_completed_callback[2])(void) = //!< Callback cuando terminan las secuencias de conversion
{
	dummy_irq_callback,
	dummy_irq_callback
};

static void (*adc_overrun_callback)(void) = dummy_irq_callback; //!< Callback cuando ocurre un overrun

static void (*adc_compare_callback)(void) = dummy_irq_callback; //!< Callbacks para las comparaciones de ADC

/**
 * @brief Inicializar el ADC
 *
 * Realiza la calibracion de hardware y fija la frecuencia de sampleo deseada.
 *
 * @param[in] sample_freq Frecuencia de sampleo deseada
 * @param[in] div Divisor para la logica del ADC (tambien afecta a la frecuencia de sampleo en modo sincronico)
 * @param[in] clock_source Fuente de clock para el ADC (solo importa para modo asincronico)
 * @param[in] mode Seleccion de modo de operacion, sincronico o asincronico
 * @param[in] low_power Seleccion de modo de bajo consumo
 */
void hal_adc_init(uint32_t sample_freq, uint8_t div, hal_adc_clock_source_en clock_source, hal_adc_operation_mode_en mode, hal_adc_low_power_mode_en low_power)
{
	sample_freq %= ADC_MAX_FREQ;

	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_ADC);
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_ADC);
	SYSCON_clear_reset(SYSCON_RESET_SEL_ADC);

	SYSCON_set_adc_clock(SYSCON_ADC_CLOCK_SEL_FRO, 1);
	ADC_set_vrange(ADC_VRANGE_HIGH_VOLTAGE);

	ADC_hardware_calib(hal_syscon_get_fro_clock() / 500e3);

	if(mode == HAL_ADC_OPERATION_MODE_ASYNCHRONOUS)
	{
		uint32_t aux;

		// El calculo de la frecuencia de sampleo se hace con una frecuencia
		// que depende de la seleccion de clock en el SYSCON
		if(clock_source == HAL_ADC_CLOCK_SOURCE_FRO)
		{
			aux = hal_syscon_get_fro_clock() / sample_freq;
		}
		else
		{
			aux = hal_syscon_get_pll_clock() / sample_freq;
		}

		SYSCON_set_adc_clock(clock_source, (uint8_t) aux);

		ADC_control_config(div, mode, low_power);
	}
	else
	{
		// El calculo de la frecuencia de sampleo se hace con la frecuencia
		// del main clock, ya dividida
		ADC_control_config(div, mode, low_power);
	}
}

/**
 * @brief Configurar una secuencia de conversion
 *
 * Esta funcion no habilita la secuencia, al menos que el parametro burst este activo
 *
 * @param[in] sequence Seleccion de secuencia a configurar
 * @param[in] config Configuracion deseada para la secuencia
 */
void hal_adc_config_sequence(hal_adc_sequence_sel_en sequence, const hal_adc_sequence_config_t *config)
{
	uint8_t counter;

	ADC_sequence_config_channels(sequence, config->channels);

	SWM_init();

	for(counter = 0; counter < ADC_CHANNEL_AMOUNT; counter++)
	{
		if(config->channels & (1 << counter))
		{
			SWM_enable_ADC(counter, SWM_ENABLE);
		}
	}

	SWM_deinit();

	ADC_sequence_config_trigger(sequence, config->trigger);
	ADC_sequence_config_trigger_pol(sequence, config->trigger_pol);
	ADC_sequence_config_sync(sequence, config->sync_bypass);

	if(config->single_step)
	{
		ADC_sequence_set_singlestep(sequence);
	}
	else
	{
		ADC_sequence_clear_singlestep(sequence);
	}

	ADC_sequence_config_interrupt_mode(sequence, config->mode);

	if(sequence == HAL_ADC_SEQUENCE_SEL_A)
	{
		if(config->low_priority)
		{
			ADC_sequence_A_lowpriority_set();
		}
		else
		{
			ADC_sequence_A_lowpriority_clear();
		}
	}

	if(config->callback != NULL)
	{
		adc_seq_completed_callback[sequence] = config->callback;
		ADC_enable_sequence_interrupt(sequence);
	}
	else
	{
		adc_seq_completed_callback[sequence] = dummy_irq_callback;
		ADC_disable_sequence_interrupt(sequence);
	}


	if(config->burst)
	{
		ADC_sequence_set_burst(sequence);
	}
	else
	{
		ADC_sequence_clear_burst(sequence);
	}

	if(sequence == HAL_ADC_SEQUENCE_SEL_A)
	{
		NVIC_enable_interrupt(NVIC_IRQ_SEL_ADC_SEQA);
	}
	else
	{
		NVIC_enable_interrupt(NVIC_IRQ_SEL_ADC_SEQB);
	}
}

/**
 * @brief Habilitar una secuencia
 * @param[in] sequence Secuencia a habilitar
 */
void hal_adc_enable_sequence(hal_adc_sequence_sel_en sequence)
{
	ADC_sequence_enable(sequence);
}

/**
 * @brief Disparar conversiones en una secuencia
 *
 * La configuracion de la secuencia, en particular el parametro single_step, influye
 * en si esta funcion dispara una secuencia entera o un paso de la misma.
 *
 * @param[in] sequence Secuencia a disparar
 */
void hal_adc_start_sequence(hal_adc_sequence_sel_en sequence)
{
	ADC_sequence_set_start(sequence);
}

/**
 * @brief Obtener resultado de la secuencia
 *
 * El comportamiento de esta funcion depende de la configuracion de la secuencia, en particular
 * de la configuracion <b>MODE</b>. En caso de estar configurada para interrumpir al final de cada
 * conversion, la funcion unicamente guardara el resultado de la conversion en el primer lugar
 * del parametro <e>result</e>, caso contrario, guardara la cantidad de canales habilitados en la
 * conversion en los distintos lugares del parametro <e>result</e>.
 *
 * @param[in] sequence Secuencia de la cual obtener el resultado
 * @param[out] result Lugares donde guardar los resultados de la secuencia
 * @return Resultado de la funcion
 */
hal_adc_sequence_result_en hal_adc_get_sequence_result(hal_adc_sequence_sel_en sequence, hal_adc_sequence_result_t *result[])
{
	if(ADC_sequence_get_mode(sequence) == ADC_INTERRUPT_MODE_EOC)
	{
		ADC_global_data_t data = ADC_get_global_data(sequence);

		if(data.DATAVALID)
		{
			(*result[0]).channel = HAL_ADC_RESULT_CHANNEL_GLOBAL;
			(*result[0]).result = data.RESULT;

			return HAL_ADC_SEQUENCE_RESULT_VALID;
		}
		else
		{
			return HAL_ADC_SEQUENCE_RESULT_INVALID;
		}
	}
	else
	{
		uint16_t configured_channels = ADC_sequence_get_channels(sequence);
		uint8_t channel_counter;
		uint8_t result_counter = 0;

		for(channel_counter = 0; channel_counter < ADC_CHANNEL_AMOUNT; channel_counter++)
		{
			if(configured_channels & (1 << channel_counter))
			{
				ADC_channel_data_t data = ADC_get_channel_data(channel_counter);

				if(data.DATAVALID)
				{
					(*result[result_counter]).channel = (hal_adc_result_channel_en) channel_counter;
					(*result[result_counter++]).result = data.RESULT;
				}
			}
		}

		return HAL_ADC_SEQUENCE_RESULT_VALID;
	}
}

/**
 * @brief Funcion dummy para usar como default para las interrupciones
 */
static void dummy_irq_callback(void)
{
	return;
}

/**
 * @brief Funcion de interrupcion cuando termina la secuencia de conversion A del ADC
 */
void ADC_SEQA_IRQHandler(void)
{
	adc_seq_completed_callback[ADC_SEQUENCE_SEL_A]();

	if(ADC->SEQ_CTRL[ADC_SEQUENCE_SEL_A].MODE == 1)
	{
		ADC->FLAGS.SEQA_INT = 1;
	}
}

/**
 * @brief Funcion de interrupcion cuando termina la secuencia de conversion B del ADC
 */
void ADC_SEQB_IRQHandler(void)
{
	adc_seq_completed_callback[ADC_SEQUENCE_SEL_B]();

	if(ADC->SEQ_CTRL[ADC_SEQUENCE_SEL_B].MODE == 1)
	{
		ADC->FLAGS.SEQB_INT = 1;
	}
}

/**
 * @brief Funcion de interrupcion cuando se detecta alguna de las condiciones de threshold establecidas
 */
void ADC_THCMP_IRQHandler(void)
{
	adc_compare_callback();
}

/**
 * @brief Funcion de interrupcion cuando se detecta alguna de las condiciones de overrun
 */
void ADC_OVR_IRQHandler(void)
{
	adc_overrun_callback();
}
