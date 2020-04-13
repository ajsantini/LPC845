/**
 * @file HAL_ADC.c
 * @brief Funciones a nivel de aplicacion del periferico ADC (LPC845)
 * @author Augusto Santini
 * @author Esteban E. Chiama
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
#include <HPL_IOCON.h>

/** Máxima frecuencia de conversión admitida por el ADC (modo sincrónico) */
#define	ADC_MAX_FREQ_SYNC		((uint32_t) 1.2e6)

/** Máxima frecuencia de conversión admitida por el ADC (modo asincrónico) */
#define	ADC_MAX_FREQ_ASYNC		((uint32_t) 0.6e6)

/** Cantidad de ciclos de clock necesarios por el \e ADC para generar una conversión */
#define	ADC_CYCLE_DELAY			(25)

/** Cantidad de canales disponibles en el \e ADC */
#define	ADC_CHANNEL_AMOUNT		(12)

static void dummy_irq_callback(void);

/** Callback cuando terminan las secuencias de conversión */
static void (*adc_seq_completed_callback[2])(void) =
{
	dummy_irq_callback,
	dummy_irq_callback
};

static void (*adc_overrun_callback)(void) = dummy_irq_callback; //!< Callback cuando ocurre un overrun

static void (*adc_compare_callback)(void) = dummy_irq_callback; //!< Callbacks para las comparaciones de ADC

typedef struct
{
	uint8_t SEQA_burst : 1;
	uint8_t SEQB_burst : 1;
}flag_sequence_burst_mode_t;

static flag_sequence_burst_mode_t flag_seq_burst_mode =
{
	.SEQA_burst = 0,
	.SEQB_burst = 0
};

/**
 * @brief Inicializar el \e ADC en modo \b asincrónico
 *
 * Realiza la calibración de hardware y fija la frecuencia de muestreo deseada.
 * Nota: Solamente se debe realizar el llamado a una de las dos funciones de inicialización del \e ADC
 *
 * @see hal_adc_clock_source_en
 * @see hal_adc_low_power_mode_en
 * @param[in] sample_freq Frecuencia de sampleo deseada
 * @param[in] div Divisor para la lógica del \e ADC
 * @param[in] clock_source Fuente de clock para el \e ADC
 * @param[in] low_power Selección de modo de bajo consumo
 */
void hal_adc_init_async_mode(uint32_t sample_freq, uint8_t div, hal_adc_clock_source_en clock_source, hal_adc_low_power_mode_en low_power)
{
	uint32_t aux;

	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_ADC);
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_ADC);

	SYSCON_assert_reset(SYSCON_RESET_SEL_ADC);
	SYSCON_clear_reset(SYSCON_RESET_SEL_ADC);

	SYSCON_set_adc_clock(SYSCON_ADC_CLOCK_SEL_FRO, 1);

	ADC_set_vrange(ADC_VRANGE_HIGH_VOLTAGE);

	ADC_hardware_calib(hal_syscon_system_clock_get() / 500e3);

	if(sample_freq > ADC_MAX_FREQ_ASYNC)
	{
		sample_freq = ADC_MAX_FREQ_ASYNC;
	}

	sample_freq *= ADC_CYCLE_DELAY;

	/*
	 * El cálculo de la frecuencia de muestreo se hace con una frecuencia
	 * que depende de la selección de clock en el SYSCON
	 */
	if(clock_source == HAL_ADC_CLOCK_SOURCE_FRO)
	{
		aux = hal_syscon_fro_clock_get() / sample_freq;
	}
	else
	{
		aux = hal_syscon_pll_clock_get() / sample_freq;
	}

	SYSCON_set_adc_clock(clock_source, (uint8_t) aux);

	ADC_control_config(div, ADC_OPERATION_MODE_ASYNCHRONOUS, low_power);
}

/**
 * @brief Inicializar el \e ADC en modo \b sincrónico
 *
 * Realiza la calibración de hardware y fija la frecuencia de muestreo deseada.
 *
 * @see hal_adc_clock_source_en
 * @see hal_adc_low_power_mode_en
 * @param[in] sample_freq Frecuencia de sampleo deseada
 * @param[in] low_power Selección de modo de bajo consumo
 */
void hal_adc_init_sync_mode(uint32_t sample_freq, hal_adc_low_power_mode_en low_power)
{
	uint32_t aux;

	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_ADC);
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_ADC);

	SYSCON_assert_reset(SYSCON_RESET_SEL_ADC);
	SYSCON_clear_reset(SYSCON_RESET_SEL_ADC);

	SYSCON_set_adc_clock(SYSCON_ADC_CLOCK_SEL_FRO, 1);
	ADC_set_vrange(ADC_VRANGE_HIGH_VOLTAGE);

	ADC_hardware_calib(hal_syscon_system_clock_get() / 500e3);

	// El cálculo de la frecuencia de muestreo se hace con la frecuencia del main clock

	if(sample_freq > ADC_MAX_FREQ_SYNC)
	{
		sample_freq = ADC_MAX_FREQ_SYNC;
	}

	sample_freq *= ADC_CYCLE_DELAY;

	aux = hal_syscon_system_clock_get() / sample_freq;

	if(aux > 0)
	{
		aux--;
	}

	ADC_control_config((uint8_t) aux, ADC_OPERATION_MODE_SYNCHRONOUS, low_power);
}

/**
 * @brief De-inicialización del \e ADC
 */
void hal_adc_deinit(void)
{
	SYSCON_assert_reset(SYSCON_RESET_SEL_ADC);

	SYSCON_disable_clock(SYSCON_ENABLE_CLOCK_SEL_ADC);

	SYSCON_power_down_peripheral(SYSCON_POWER_SEL_ADC);

	NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_OVR);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_SEQA);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_SEQB);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_THCMP);
}

/**
 * @brief Configurar una secuencia de conversión
 *
 * Esta función no habilita la secuencia, al menos que el parametro \b burst este activo
 *
 * @see hal_adc_sequence_sel_en
 * @see hal_adc_sequence_config_t
 * @param[in] sequence Seleccion de secuencia a configurar
 * @param[in] config Configuracion deseada para la secuencia
 */
void hal_adc_sequence_config(hal_adc_sequence_sel_en sequence, const hal_adc_sequence_config_t *config)
{
	uint8_t counter;

	ADC_sequence_config_channels(sequence, config->channels);

	SWM_init();
	IOCON_init();

	for(counter = 0; counter < ADC_CHANNEL_AMOUNT; counter++)
	{
		if(config->channels & (1 << counter))
		{
			IOCON_disable_pullup_adc(counter);
			SWM_enable_ADC(counter, SWM_ENABLE);
		}
	}

	IOCON_deinit();
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
			ADC_sequence_A_make_low_priority();
		}
		else
		{
			ADC_sequence_A_make_high_priority();
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
		if(sequence == HAL_ADC_SEQUENCE_SEL_A)
		{
			flag_seq_burst_mode.SEQA_burst = 1;
		}
		else if(sequence == HAL_ADC_SEQUENCE_SEL_B)
		{
			flag_seq_burst_mode.SEQB_burst = 1;
		}
	}
	else
	{
		if(sequence == HAL_ADC_SEQUENCE_SEL_A)
		{
			flag_seq_burst_mode.SEQA_burst = 0;
		}
		else if(sequence == HAL_ADC_SEQUENCE_SEL_B)
		{
			flag_seq_burst_mode.SEQB_burst = 0;
		}
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
 * @brief Disparar conversiones en una secuencia
 *
 * La configuración de la secuencia, en particular el parametro \b single_step, influye
 * en si esta funcion dispara una secuencia entera o un paso de la misma.
 *
 * @see hal_adc_sequence_sel_en
 * @see hal_adc_stop_sequence
 * @param[in] sequence Secuencia a disparar
 */
void hal_adc_sequence_start(hal_adc_sequence_sel_en sequence)
{
	if(flag_seq_burst_mode.SEQA_burst && (sequence ==  HAL_ADC_SEQUENCE_SEL_A))
	{
		ADC_sequence_set_burst(sequence);
	}
	else if(flag_seq_burst_mode.SEQB_burst && (sequence ==  HAL_ADC_SEQUENCE_SEL_B))
	{
		ADC_sequence_set_burst(sequence);
	}
	else
	{
		ADC_sequence_enable(sequence);
		ADC_sequence_set_start(sequence);
	}
}

/**
 * @brief Detener conversiones en una secuencia de conversión
 * @param[in] sequence Secuencia a detener
 * @see hal_adc_sequence_sel_en
 * @see hal_adc_start_sequence
 */
void hal_adc_sequence_stop(hal_adc_sequence_sel_en sequence)
{
	if(flag_seq_burst_mode.SEQA_burst && (sequence ==  HAL_ADC_SEQUENCE_SEL_A))
	{
		ADC_sequence_clear_burst(sequence);
	}
	else if(flag_seq_burst_mode.SEQB_burst && (sequence ==  HAL_ADC_SEQUENCE_SEL_B))
	{
		ADC_sequence_clear_burst(sequence);
	}
	else
	{
		ADC_sequence_disable(sequence);
	}
}

/**
 * @brief Obtener resultado de la secuencia
 *
 * El comportamiento de esta funcion depende de la configuración de la secuencia, en particular
 * de la configuracion <b>MODE</b>. En caso de estar configurada para interrumpir al final de cada
 * conversión, la función únicamente guardara el resultado de la conversión en el primer lugar
 * del parámetro <e>result</e>, caso contrario, guardara la cantidad de canales habilitados en la
 * conversión en los distintos lugares del parámetro <e>result</e>.
 *
 * @see hal_adc_sequence_result_en
 * @see hal_adc_sequence_sel_en
 * @see hal_adc_sequence_result_t
 * @param[in] sequence Secuencia de la cual obtener el resultado
 * @param[out] result Lugares donde guardar los resultados de la secuencia
 * @return Resultado de la función
 */
hal_adc_sequence_result_en hal_adc_sequence_get_result(hal_adc_sequence_sel_en sequence, hal_adc_sequence_result_t *result)
{
	if(ADC_sequence_get_mode(sequence) == ADC_INTERRUPT_MODE_EOC)
	{
		ADC_global_data_t data = ADC_get_global_data(sequence);

		if(data.DATAVALID)
		{
			(result[0]).channel = HAL_ADC_RESULT_CHANNEL_GLOBAL;
			(result[0]).result = data.RESULT;

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
					(result[result_counter]).channel = (hal_adc_result_channel_en) channel_counter;
					(result[result_counter++]).result = data.RESULT;
				}
			}
		}

		return HAL_ADC_SEQUENCE_RESULT_VALID;
	}
}

/**
 * @brief Configurar valor de umbral de comparación.
 * @param[in] threshold	Selección de umbral a configurar
 * @param[in] low Umbral bajo
 * @param[in] high Umbral alto
 * @see hal_adc_threshold_sel_en
 */
void hal_adc_threshold_config(hal_adc_threshold_sel_en threshold, uint16_t low, uint16_t high)
{
	ADC_set_compare_low_threshold(threshold, low);
	ADC_set_compare_high_threshold(threshold, high);
}

/**
 * @brief Configura un canal para utilizar la funcionalidad de comparación con un umbral y su tipo de interrupción deseada.
 * @param[in] adc_channel Canal de ADC en el cual configurar el umbral
 * @param[in] threshold	Selección de umbral a configurar
 * @param[in] irq_mode Indica el tipo evento por el cual la comparación con el umbral dispara la interrupción, o la desactiva.
 * @see hal_adc_threshold_sel_en
 * @see hal_adc_threshold_interrupt_sel_en
 */
void hal_adc_threshold_channel_config(uint8_t adc_channel, hal_adc_threshold_sel_en threshold, hal_adc_threshold_interrupt_sel_en irq_mode)
{
	ADC_set_channel_threshold(adc_channel, threshold);

	if(irq_mode == HAL_ADC_THRESHOLD_IRQ_SEL_DISABLED)
	{
		ADC_disable_threshold_interrupt(adc_channel);
	}
	else
	{
		ADC_enable_threshold_interrupt(adc_channel, irq_mode);
	}
}

/**
 * @brief Registrar un callabck de interrupción para interrupción por threshold
 * @param[in] callback Callback a ejecutar en interrupción por threshold
 */
void hal_adc_threshold_register_interrupt(void (*callback)(void))
{
	if(callback != NULL)
	{
		adc_compare_callback = callback;
		NVIC_enable_interrupt(NVIC_IRQ_SEL_ADC_THCMP);
	}
	else
	{
		adc_compare_callback = dummy_irq_callback;
		NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_THCMP);
	}
}

/**
 * @brief Obtener resultados de comparación de la última conversión
 * @param[out] results Puntero a donde guardar los resultados
 *
 * El usuario \b debe garantizar que hayan por lo menos la cantidad de memoria reservada de este tipo
 * como cantidad de canales habilitados para comparar contra un umbral.
 *
 * @see hal_adc_channel_compare_result_t
 */
void hal_adc_threshold_get_comparison_results(hal_adc_channel_compare_result_t *results)
{
	ADC_interrupt_flags_t aux_flags_reg = ADC_get_interrupt_flags();
	uint16_t irq_thr_channels = 0xFFF & *((uint16_t *) &aux_flags_reg);

	uint8_t idx_result = 0;
	uint8_t idx_adc_channel = 0;

	while(idx_adc_channel < ADC_CHANNEL_AMOUNT)
	{
		if(irq_thr_channels & (1 << idx_adc_channel))
		{
			ADC_channel_data_t data = ADC_get_channel_data(idx_adc_channel);

			(results + idx_result)->channel = idx_adc_channel;
			(results + idx_result)->value = data.RESULT;
			(results + idx_result)->result_range = data.THCMPRANGE;
			(results + idx_result)->result_crossing = data.THCMPCROSS;

			idx_result++;
		}

		idx_adc_channel++;
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
 * @brief Función de interrupción cuando termina la secuencia de conversión A del \e ADC
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
 * @brief Función de interrupción cuando termina la secuencia de conversión B del \e ADC
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
 * @brief Función de interrupción cuando se detecta alguna de las condiciones de threshold establecidas
 */
void ADC_THCMP_IRQHandler(void)
{
	adc_compare_callback();

	if(ADC_get_interrupt_flags().THCMP_INT)
	{
		ADC_clear_threshold_flags();
	}
}

/**
 * @brief Función de interrupción cuando se detecta alguna de las condiciones de overrun
 */
void ADC_OVR_IRQHandler(void)
{
	#warning Habria que ver que flags y como limpiarlos si es que hace falta
	adc_overrun_callback();
}
