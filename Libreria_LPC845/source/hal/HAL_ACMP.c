/**
 * @file HAL_ACMP.h
 * @brief Declaraciones a nivel de aplicación del periférico ADC (LPC845)
 * @author Esteban E. Chiama
 * @date 4/2020
 * @version 1.0
 */

#include <HPL_SYSCON.h>
#include <HPL_NVIC.h>
#include <HPL_SWM.h>
#include <HPL_IOCON.h>
#include <HPL_ACMP.h>
#include <HAL_ACMP.h>

void hal_acmp_init(void)
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

	if(acmp_config->interrupt_enable)
	{
		ACMP_interrupt_enable();
	}
	else
	{
		ACMP_interrupt_disable();
	}
}

void hal_acmp_ladder_config(const hal_acmp_ladder_config_t *ladder_config)
{
	SWM_init();

	if(ladder_config->enable)
	{
		if(ladder_config->vref_sel ==  HAL_ACMP_LADDER_VREF_VDDCMP_PIN)
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

		ACMP_ladder_vref_select(ladder_config->vref_sel);
		ACMP_ladder_step_set( (0x1F) & (ladder_config->step) );
		ACMP_ladder_enable();
	}
	else
	{
		ACMP_ladder_disable();
		SWM_enable_VDDCMP(SWM_DISABLE);
	}

	SWM_deinit();
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
