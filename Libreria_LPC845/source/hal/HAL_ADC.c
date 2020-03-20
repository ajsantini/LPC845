/**
 * @file HAL_ADC.c
 * @brief Funciones a nivel de aplicacion del periferico ADC (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_ADC.h>
#include <HAL_SYSCON.h>
#include <HPL_ADC.h>
#include <HPL_SYSCON.h>

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
 * Asume tension de trabajo alta y utiliza como fuente de clock el FRO sin dividir.
 * Configura el ADC obviando el modo bajo consumo.
 *
 * @param[in] sample_freq Frecuencia de sampleo deseada
 */
void hal_adc_init(uint32_t sample_freq)
{
	sample_freq %= ADC_MAX_FREQ;

	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_ADC);
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_ADC);
	SYSCON_set_adc_clock(SYSCON_ADC_CLOCK_SEL_FRO, 0);
	ADC_set_vrange(ADC_VRANGE_HIGH_VOLTAGE);

	// Calibracion por hardware
	ADC_hardware_calib(hal_syscon_get_fro_clock() / 500e3);

	ADC_control_config(hal_syscon_get_fro_clock() / sample_freq,
						ADC_OPERATION_MODE_ASYNCHRONOUS,
						ADC_LOW_POWER_MODE_DISABLED);
}

/**
 * @brief Configurar una secuencia de conversion
 *
 * Esta funcion no habilita la secuencia, al menos que el parametro burst este activo
 *
 * @param[in] sequence Seleccion de secuencia a configurar
 * @param[in] config Configuracion deseada para la secuencia
 */
void hal_adc_config_sequence(ADC_sequence_sel_en sequence, const hal_adc_sequence_config_t *config)
{
	ADC_sequence_config_channels(sequence, config->channels);
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

	if(sequence == ADC_SEQUENCE_SEL_A)
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

	adc_seq_completed_callback[sequence] = config->callback;

	if(config->burst)
	{
		ADC_sequence_set_burst(sequence);
	}
	else
	{
		ADC_sequence_clear_burst(sequence);
	}
}

/**
 * @brief Disparar conversiones en una secuencia
 *
 * La configuracion de la secuencia, en particular el parametro single_step, influye
 * en si esta funcion dispara una secuencia entera o un paso de la misma.
 *
 * @param[in] sequence Secuencia a disparar
 */
void hal_adc_start_sequence(ADC_sequence_sel_en sequence)
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
hal_adc_sequence_result_en hal_adc_get_sequence_result(ADC_sequence_sel_en sequence, hal_adc_sequence_result_t *result[])
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
					(*result[result_counter]).channel = channel_counter;
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
