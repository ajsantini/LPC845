/*
===============================================================================
 Name        : Ejemplo6-ADC-THR.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include <cr_section_macros.h>

#include <HAL_SYSCON.h>
#include <HAL_GPIO.h>
#include <HAL_ADC.h>

#include <stddef.h>

#define LED_AZUL		HAL_GPIO_PORTPIN_1_1
#define LED_OFF_STATE	1
#define LED_ON_STATE	0

#define LedOff()		hal_gpio_set_pin(LED_AZUL)
#define LedOn()			hal_gpio_clear_pin(LED_AZUL)

#define	ADC_SAMPLE_RATE	480e3
#define	ADC_CHANNEL		0

#define ADC_THR_LOW		1500
#define ADC_THR_HIGH	2000

void callback(void);

static const hal_adc_sequence_config_t adc_config =
{
	.channels = (1 << 0),
	.trigger = HAL_ADC_TRIGGER_SEL_NONE,
	.trigger_pol = 0,
	.sync_bypass = HAL_ADC_SYNC_SEL_ENABLE_SYNC,
	.mode = 0,
	.burst = 1,
	.single_step = 1,
	.low_priority = 0,
	.callback = NULL
};

static hal_adc_channel_compare_result_t result;

int main(void) {

	hal_syscon_config_fro_direct(1,1);

	hal_gpio_init(1);
	hal_gpio_set_dir(LED_AZUL, HAL_GPIO_DIR_OUTPUT, LED_OFF_STATE);

	hal_adc_init_sync_mode(ADC_SAMPLE_RATE, HAL_ADC_LOW_POWER_MODE_DISABLED);
	hal_adc_sequence_config(HAL_ADC_SEQUENCE_SEL_A, &adc_config);
	hal_adc_threshold_config(HAL_ADC_THRESHOLD_SEL_0, ADC_THR_LOW, ADC_THR_HIGH);
	hal_adc_threshold_channel_config(ADC_CHANNEL, HAL_ADC_THRESHOLD_SEL_0, HAL_ADC_THRESHOLD_INTERRUPT_SEL_CROSSING);
	hal_adc_threshold_register_interrupt(callback);

	hal_adc_sequence_start(HAL_ADC_SEQUENCE_SEL_A);

    while(1) {

    }
    return 0 ;
}

void callback(void)
{
	hal_adc_threshold_get_comparison_results(&result);

	if(result.result_range == HAL_ADC_COMPARISON_RANGE_INSIDE)
	{
		LedOn();
	}
	else if(result.result_range == HAL_ADC_COMPARISON_RANGE_BELOW)
	{
		LedOff();
	}
}
