/**
 * @file HAL_ADC.c
 * @brief Funciones a nivel de aplicacion del periferico ADC (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_ADC.h>
#include <HPL_ADC.h>
#include <HPL_SYSCON.h>

#define	ADC_MAX_FREQ			1.2e6 //<! Maxima frecuencia de conversion admitida por el ADC

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
 *
 * @param[in] sample_freq Frecuencia de sampleo deseada
 */
void hal_adc_init(uint32_t sample_freq)
{
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_ADC);
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_ADC);

	ADC_set_vrange(ADC_VRANGE_HIGH_VOLTAGE);

	// Calibracion por hardware
	SYSCON_set_adc_clock(SYSCON_ADC_CLOCK_SEL_FRO, );

	SYSCON_set_adc_clock(SYSCON_ADC_CLOCK_SEL_FRO, 0);
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
