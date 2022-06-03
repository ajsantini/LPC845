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

/** Cantidad de ciclos de clock necesarios por el *ADC* para generar una conversión */
#define	ADC_CYCLE_DELAY			(25)

/** Cantidad de canales disponibles en el *ADC* */
#define	ADC_CHANNEL_AMOUNT		(12)

static void dummy_irq_callback(void* data);

/** Callback cuando terminan las secuencias de conversión */
static void (*adc_seq_completed_callback[2])(void *) =
{
	dummy_irq_callback,
	dummy_irq_callback
};

/** Datos para pasarle al callback cuando se terminan las secuencias de conversion */
static void *adc_seq_completed_callback_data[2] = {
	NULL,
	NULL
};

static void (*adc_overrun_callback)(void*) = dummy_irq_callback; //!< Callback cuando ocurre un overrun

/** Datos para pasarle al callback de overrun */
static void *adc_overrun_callback_data = NULL;

static void (*adc_compare_callback)(void*) = dummy_irq_callback; //!< Callbacks para las comparaciones de ADC

/** Datos para pasarle al callback de compare */
static void *adc_compare_callback_data = NULL;

/** Flags para determinar si cada secuencia fue configurada en modo burst o no */
typedef struct
{
	uint8_t SEQA_burst : 1; /**< Flag burst para secuencia A */
	uint8_t SEQB_burst : 1; /**< Flag burst para secuencia B */
}flag_sequence_burst_mode_t;

/** Variable para tener la configuración de modo burst de cada secuencia */
static flag_sequence_burst_mode_t flag_seq_burst_mode =
{
	.SEQA_burst = 0,
	.SEQB_burst = 0
};

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

void hal_adc_deinit(void)
{
	SYSCON_assert_reset(SYSCON_RESET_SEL_ADC);

	SYSCON_disable_clock(SYSCON_ENABLE_CLOCK_SEL_ADC);

	SYSCON_power_down_peripheral(SYSCON_POWER_SEL_ADC);

	NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_OVR);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_SEQA);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_SEQB);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_THCMP);

	SWM_init();

	uint8_t idx_adc_channel = 0;
	while(idx_adc_channel < ADC_CHANNEL_AMOUNT)
	{
		SWM_enable_ADC(idx_adc_channel++, SWM_DISABLE);
	}

	SWM_deinit();
}

void hal_adc_inhibit_sequence_interrupts(hal_adc_sequence_sel_en sequence) {
	if (sequence == HAL_ADC_SEQUENCE_SEL_A) {
		NVIC_enable_interrupt(NVIC_IRQ_SEL_ADC_SEQA);
	} else {
		NVIC_enable_interrupt(NVIC_IRQ_SEL_ADC_SEQB);
	}
}

void hal_adc_deinhibit_sequence_interrupts(hal_adc_sequence_sel_en sequence) {
	if (sequence == HAL_ADC_SEQUENCE_SEL_A) {
		NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_SEQA);
	} else {
		NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_SEQB);
	}
}

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
		adc_seq_completed_callback_data[sequence] = config->cb_data;
		ADC_enable_sequence_interrupt(sequence);
	}
	else
	{
		adc_seq_completed_callback[sequence] = dummy_irq_callback;
		adc_seq_completed_callback_data[sequence] = NULL;
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

void hal_adc_threshold_config(hal_adc_threshold_sel_en threshold, uint16_t low, uint16_t high)
{
	ADC_set_compare_low_threshold(threshold, low);
	ADC_set_compare_high_threshold(threshold, high);
}

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

void hal_adc_threshold_register_interrupt(adc_comparison_interrupt_t callback, void *data)
{
	if(callback != NULL)
	{
		adc_compare_callback = callback;
		adc_compare_callback_data = data;
		NVIC_enable_interrupt(NVIC_IRQ_SEL_ADC_THCMP);
	}
	else
	{
		adc_compare_callback = dummy_irq_callback;
		adc_compare_callback_data = NULL;
		NVIC_disable_interrupt(NVIC_IRQ_SEL_ADC_THCMP);
	}
}

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

static void dummy_irq_callback(void* data)
{
	(void) data;
	return;
}

void ADC_SEQA_IRQHandler(void)
{
	if (adc_seq_completed_callback[ADC_SEQUENCE_SEL_A] != NULL) {
		adc_seq_completed_callback[ADC_SEQUENCE_SEL_A](adc_seq_completed_callback_data[ADC_SEQUENCE_SEL_A]);
	}

	if(ADC->SEQ_CTRL[ADC_SEQUENCE_SEL_A].MODE == 1)
	{
		ADC->FLAGS.SEQA_INT = 1;
	}
}

void ADC_SEQB_IRQHandler(void)
{
	if (adc_seq_completed_callback[ADC_SEQUENCE_SEL_B] != NULL) {
		adc_seq_completed_callback[ADC_SEQUENCE_SEL_B](adc_seq_completed_callback_data[ADC_SEQUENCE_SEL_B]);
	}

	if(ADC->SEQ_CTRL[ADC_SEQUENCE_SEL_B].MODE == 1)
	{
		ADC->FLAGS.SEQB_INT = 1;
	}
}

void ADC_THCMP_IRQHandler(void)
{
	if (adc_compare_callback != NULL) {
		adc_compare_callback(adc_compare_callback_data);
	}

	if(ADC_get_interrupt_flags().THCMP_INT)
	{
		ADC_clear_threshold_flags();
	}
}

void ADC_OVR_IRQHandler(void)
{
	#warning Habria que ver que flags y como limpiarlos si es que hace falta
	if (adc_overrun_callback != NULL) {
		adc_overrun_callback(adc_overrun_callback_data);
	}
}
