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

#define	ADC_SAMPLE_RATE		480e3

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

hal_adc_threshold_interrupt_sel_en thr_irq_type = HAL_ADC_THRESHOLD_INTERRUPT_SEL_CROSSING;

static const hal_adc_threshold_config_t thrConf =
{
		.low = 1500,
		.high = 2000,
		.chans = 0x1,
		.irq_modes = &thr_irq_type
};

#define MAX_TOGGLES		5

static hal_adc_channel_compare_result_t result;
static uint8_t toggle_count = 0;

void callback_threshold(void);

void callback_threshold(void)
{
	hal_adc_get_comparison_results(&result);

	if( result.result_range == HAL_ADC_COMPARISON_RANGE_BELOW )
	{
		hal_gpio_toggle_pin(LED_AZUL);
		toggle_count++;
		if(toggle_count == MAX_TOGGLES)
		{
			hal_adc_stop_sequence(HAL_ADC_SEQUENCE_SEL_A);
		}
	}
}

int main(void) {

	hal_syscon_config_fro_direct(1,1);

	hal_gpio_init(1);
	hal_gpio_set_dir(LED_AZUL, HAL_GPIO_DIR_OUTPUT, LED_OFF_STATE);

	hal_adc_init_sync_mode(ADC_SAMPLE_RATE, HAL_ADC_LOW_POWER_MODE_DISABLED);
	hal_adc_config_sequence(HAL_ADC_SEQUENCE_SEL_A, &adc_config);

	hal_adc_config_threshold(HAL_ADC_THRESHOLD_SEL_0, &thrConf);
	hal_adc_register_threshold_interrupt( callback_threshold );

	hal_adc_start_sequence(HAL_ADC_SEQUENCE_SEL_A);

    while(1) {

    }
    return 0 ;
}
