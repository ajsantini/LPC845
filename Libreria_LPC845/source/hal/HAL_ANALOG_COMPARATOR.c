/*
 * HAL_ANALOG_COMPARATOR.c
 *
 *  Created on: Apr 12, 2020
 *      Author: stv
 */

#include <HPL_SYSCON.h>
#include <HPL_NVIC.h>
#include <HPL_SWM.h>
#include <HPL_IOCON.h>
#include <HPL_ANALOG_COMPARATOR.h>

typedef enum
{
	HAL_ACMP_OUTPUT_DIRECT = 0,
	HAL_ACMP_OUTPUT_SYNC,
}hal_acmp_output_control_en;

typedef enum
{
	HAL_ACMP_HYSTERESIS_NONE = 0,
	HAL_ACMP_HYSTERESIS_5mV,
	HAL_ACMP_HYSTERESIS_10mV,
	HAL_ACMP_HYSTERESIS_20mV
}hal_acmp_hysteresis_sel_en;

typedef enum
{
	HAL_ACMP_LADDER_VREF_VDD = 0,
	HAL_ACMP_LADDER_VREF_VDDCMP_PIN
}hal_acmp_ladder_vref_sel_en;

typedef enum
{
	HAL_ACMP_EDGE_FALLING = 0,
	HAL_ACMP_EDGE_RISING,
	HAL_ACMP_EDGE_BOTH
}hal_acmp_edge_sel_en;

typedef struct
{
	hal_acmp_output_control_en output_control;
	hal_acmp_hysteresis_sel_en hysteresis;

	uint8_t ladder_enable;
	hal_acmp_ladder_vref_sel_en ladder_vref_sel;
	uint8_t ladder_step;

	uint8_t interrupt_enable;
	hal_acmp_edge_sel_en edge_sel;

}hal_acpm_config_t;

typedef enum
{
	HAL_ACMP_INPUT_VOLTAGE_VLADDER_OUT = 0,
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I1,
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I2,
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I3,
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I4,
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I5,
	HAL_ACMP_INPUT_VOLTAGE_BANDGAP,
	HAL_ACMP_INPUT_VOLTAGE_DACOUT0
}hal_acmp_input_voltage_sel_en;

void hal_analog_comparator_init(void)
{
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_ACMP);
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_ACMP);

	SYSCON_assert_reset(SYSCON_RESET_SEL_ACMP);
	SYSCON_clear_reset(SYSCON_RESET_SEL_ACMP);

	NVIC_enable_interrupt(NVIC_IRQ_SEL_CMP_CAPT);
}

void hal_acmp_deinit(void)
{
	SYSCON_assert_reset(SYSCON_RESET_SEL_ACMP);

	SYSCON_disable_clock(SYSCON_ENABLE_CLOCK_SEL_ACMP);
	SYSCON_power_down_peripheral(SYSCON_POWER_SEL_ACMP);

	NVIC_disable_interrupt(NVIC_IRQ_SEL_CMP_CAPT);

	SWM_init();
	SWM_enable_VDDCMP(SWM_DISABLE);
	SWM_enable_ACMP(1, SWM_DISABLE);
	SWM_enable_ACMP(2, SWM_DISABLE);
	SWM_enable_ACMP(3, SWM_DISABLE);
	SWM_enable_ACMP(4, SWM_DISABLE);
	SWM_enable_ACMP(5, SWM_DISABLE);
}

void hal_acmp_config(const hal_acpm_config_t *acmp_config)
{
	ACMP_control_config(acmp_config->edge_sel, acmp_config->output_control, acmp_config->hysteresis);

	SWM_init();
	if(acmp_config->ladder_enable)
	{
		if(acmp_config->ladder_vref_sel ==  HAL_ACMP_LADDER_VREF_VDDCMP_PIN)
		{
			IOCON_init();
			IOCON_disable_pullup_acmp_ladder_external_ref_VDDCMP();
			IOCON_deinit();
			SWM_enable_VDDCMP(SWM_ENABLE);
		}
		else
		{
			SWM_enable_VDDCMP(SWM_DISABLE);
		}

		ACMP_ladder_vref_select(acmp_config->ladder_vref_sel);
		ACMP_ladder_step_set( (0x1F) & (acmp_config->ladder_step) );
		ACMP_ladder_enable();
	}
	else
	{
		ACMP_ladder_disable();
		SWM_enable_VDDCMP(SWM_DISABLE);
	}
	SWM_deinit();

	if(acmp_config->interrupt_enable)
	{
		ACMP_interrupt_enable();
	}
	else
	{
		ACMP_interrupt_disable();
	}
}

void hal_acmp_input_select(hal_acmp_input_voltage_sel_en positive_input,
						 hal_acmp_input_voltage_sel_en negative_input)
{
	SWM_init();
	IOCON_init();

	if( (positive_input == HAL_ACMP_INPUT_VOLTAGE_ACMP_I1) ||
		(negative_input == HAL_ACMP_INPUT_VOLTAGE_ACMP_I1) )
	{
		IOCON_disable_pullup_acmp_input(1);
		SWM_enable_ACMP(1, SWM_ENABLE);
	}
	else
	{
		SWM_enable_ACMP(1, SWM_DISABLE);
	}

	if( (positive_input == HAL_ACMP_INPUT_VOLTAGE_ACMP_I2) ||
		(negative_input == HAL_ACMP_INPUT_VOLTAGE_ACMP_I2) )
	{
		IOCON_disable_pullup_acmp_input(2);
		SWM_enable_ACMP(2, SWM_ENABLE);
	}
	else
	{
		SWM_enable_ACMP(2, SWM_DISABLE);
	}

	if( (positive_input == HAL_ACMP_INPUT_VOLTAGE_ACMP_I3) ||
		(negative_input == HAL_ACMP_INPUT_VOLTAGE_ACMP_I3) )
	{
		IOCON_disable_pullup_acmp_input(3);
		SWM_enable_ACMP(3, SWM_ENABLE);
	}
	else
	{
		SWM_enable_ACMP(3, SWM_DISABLE);
	}

	if( (positive_input == HAL_ACMP_INPUT_VOLTAGE_ACMP_I4) ||
		(negative_input == HAL_ACMP_INPUT_VOLTAGE_ACMP_I4) )
	{
		IOCON_disable_pullup_acmp_input(4);
		SWM_enable_ACMP(4, SWM_ENABLE);
	}
	else
	{
		SWM_enable_ACMP(4, SWM_DISABLE);
	}

	if( (positive_input == HAL_ACMP_INPUT_VOLTAGE_ACMP_I5) ||
		(negative_input == HAL_ACMP_INPUT_VOLTAGE_ACMP_I5) )
	{
		IOCON_disable_pullup_acmp_input(5);
		SWM_enable_ACMP(5, SWM_ENABLE);
	}
	else
	{
		SWM_enable_ACMP(5, SWM_DISABLE);
	}

	IOCON_deinit();
	SWM_deinit();

	ACMP_voltage_input_select(positive_input, negative_input);
}
