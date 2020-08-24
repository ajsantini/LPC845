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

/**
 * @brief Inicialización del periférico Comparador Analógico.
 * @see hal_acmp_deinit
 * @see hal_acmp_config
 */
void hal_acmp_init(void)
{
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_ACMP);
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_ACMP);

	SYSCON_assert_reset(SYSCON_RESET_SEL_ACMP);
	SYSCON_clear_reset(SYSCON_RESET_SEL_ACMP);

	NVIC_enable_interrupt(NVIC_IRQ_SEL_CMP_CAPT);
}

/**
 * @brief De-inicialización del periferico Comparador Analógico.
 *
 * Además de la usual deinicialización de un periférico,
 * libera todos los pines posiblemente utilizados por el comparador de su función analógica.
 */
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

/**
 * @brief Configuración de parámetros generales del comparador analógico.
 *
 * Esta función configura las siguientes características del comparador:
 *
 * - Sincronización de la señal de salida del comparador con clock de bus.
 * - Rango de histérisis sobre la comparación para validar un cambio de la salida del comparador.
 * - Habilitación o deshabilitación de interrupciones, y según qué tipo de flanco se quiera
 * de la salida del comparador.
 * .
 *
 * @param[in] acmp_config Puntero a estructura con parámetros a configurar.
 *
 * @see hal_acpm_config_t
 * @see hal_acmp_ladder_config
 * @see hal_acmp_input_select
 */
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

/**
 * @brief Configuración de la Voltage Ladder del comparador analógico.
 *
 * Configura las siguientes características de la Voltage Ladder del comparador:
 *
 * - Habilitación o no.
 * - Tensión de referencia.
 * - Fracción ('step') utilizada de dicha tensión de referencia.
 * .
 *
 * @param[in] ladder_config Puntero a estructura con parámetros de configuración deseados de la Voltage Ladder.
 *
 * @see hal_acmp_ladder_config_t
 * @see hal_acmp_config
 * @see hal_acmp_ladder_config
 * @see hal_acmp_input_select
 * @see hal_acmp_deinit
 */
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

/**
 * @brief Selecciona las entradas positiva y negativa deseadas para el comparador.
 *
 * Para entradas analógicas además realiza la configuración necesaria del pin externo.
 *
 * @param[in] positive_input Selección de entrada para la entrada positiva del comparador analógico.
 * @param[in] negative_input Selección de entrada para la entrada negativa del comparador analógico.
 * @see hal_acmp_input_voltage_sel_en
 * @see hal_acmp_ladder_config
 * @see hal_acmp_deinit
 */
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

/** @brief Asigna la salida del comparador a un pin externo del microcontrolador.
 *
 *	@note: No realiza ningún tipo de configuración del pin respecto a sus resistores.
 *
 *  @param[in] port_pin Indica puerto y pin deseado.
 *  @see hal_acmp_output_pin_clear
 */
void hal_acmp_output_pin_set(hal_gpio_portpin_en port_pin)
{
	SWM_init();
	SWM_assign_COMP0_OUT(HAL_GPIO_PORTPIN_TO_PORT(port_pin), HAL_GPIO_PORTPIN_TO_PIN(port_pin));
	SWM_deinit();
}

/**
 * @brief Deshace la asignación de la salida del comparador a un pin externo del microcontrolador.
 * @see hal_acmp_output_pin_set
 */
void hal_acmp_output_pin_clear()
{
	SWM_init();
	SWM_assign_COMP0_OUT(HAL_GPIO_PORTPIN_TO_PORT(0xFF), HAL_GPIO_PORTPIN_TO_PIN(0xFF));
	SWM_deinit();
}
