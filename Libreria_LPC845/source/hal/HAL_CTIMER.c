/**
 * @file HAL_CTIMER.c
 * @brief Funciones a nivel de aplicacion del periferico CTIMER (LPC845)
 * @author Augusto Santini
 * @author Esteban E. Chiama
 * @date 3/2020
 * @version 1.0
 */

#include <stddef.h>
#include <HAL_CTIMER.h>
#include <HAL_SYSCON.h>
#include <HAL_IOCON.h>
#include <HPL_CTIMER.h>
#include <HPL_NVIC.h>
#include <HPL_SWM.h>

#define		MATCH_AMOUNT		4
#define		CAPTURE_AMOUNT		4

#define		PWM_CHANNELS		3

static void dummy_irq(void);

static uint32_t hal_ctimer_calc_match_value(uint32_t match_value_useg);

void (*match_callbacks[MATCH_AMOUNT])(void) = //!< Callbacks para interrupciones de match
{
	dummy_irq,
	dummy_irq,
	dummy_irq,
	dummy_irq
};

void (*capture_callbacks[CAPTURE_AMOUNT])(void) = //!< Callbacks para interrupciones de capture
{
	dummy_irq,
	dummy_irq,
	dummy_irq,
	dummy_irq
};

/**
 * @brief Inicializacion del periferico en modo timer
 *
 * Esta funcion no pone a correr el contador.
 *
 * @param[in] clock_div Divisor del clock principal deseado (el valor efectivo es este valor + 1)
 */
void hal_ctimer_timer_mode_init(uint32_t clock_div)
{
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_CTIMER);
	SYSCON_clear_reset(SYSCON_RESET_SEL_CTIMER);

	CTIMER_assert_counter_reset();
	CTIMER_disable_counter();
	CTIMER_clear_counter_reset();

	CTIMER_write_prescaler(clock_div);

	CTIMER_config_counter_timer_mode(CTIMER_MODE_TIMER);

	NVIC_enable_interrupt(NVIC_IRQ_SEL_CTIMER);
}

/**
 * @brief Configurar un canal de match
 * @param[in] match_sel Match a configurar
 * @param[in] match_config Configuracion deseada
 */
void hal_ctimer_timer_mode_match_config(hal_ctimer_match_sel_en match_sel, const hal_ctimer_match_config_t *match_config)
{
	SWM_init();

	if(match_config->enable_external_pin)
	{
		SWM_assign_T0_MAT(match_sel, match_config->match_pin / 32, match_config->match_pin % 32);
	}
	else
	{
		SWM_assign_T0_MAT(match_sel, 0xFF, 0xFF);
	}

	SWM_deinit();

	if(match_config->interrupt_on_match)
	{
		CTIMER_enable_interrupt_on_match(match_sel);

		if(match_config->callback != NULL)
		{
			match_callbacks[match_sel] = match_config->callback;
		}
		else
		{
			match_callbacks[match_sel] = dummy_irq;
		}
	}
	else
	{
		CTIMER_disable_interrupt_on_match(match_sel);
		match_callbacks[match_sel] = dummy_irq;
	}

	if(match_config->reset_on_match)
	{
		CTIMER_enable_reset_on_match(match_sel);
	}
	else
	{
		CTIMER_disable_reset_on_match(match_sel);
	}

	if(match_config->stop_on_match)
	{
		CTIMER_enable_stop_on_match(match_sel);
	}
	else
	{
		CTIMER_disable_stop_on_match(match_sel);
	}

	if(match_config->reload_on_match)
	{
		CTIMER_enable_reload_on_match(match_sel);
		CTIMER_write_shadow_register(match_sel, hal_ctimer_calc_match_value(match_config->match_value_useg) );
	}
	else
	{
		CTIMER_disable_reload_on_match(match_sel);
	}

	CTIMER_write_match_value(match_sel, hal_ctimer_calc_match_value(match_config->match_value_useg));

	CTIMER_config_external_match(match_sel, match_config->match_action);
}

/**
 * @brief Habilitar el conteo del ctimer
 */
void hal_ctimer_timer_mode_run(void)
{
	CTIMER_enable_counter();
}

/**
 * @brief Inhabilitar el conteo del ctimer
 */
void hal_ctimer_timer_mode_stop(void)
{
	CTIMER_disable_counter();
}

/**
 * @brief Reiniciar el conteo del ctimer
 */
void hal_ctimer_timer_mode_reset(void)
{
	CTIMER_assert_counter_reset();
	CTIMER_clear_counter_reset();
}

/**
 * @brief Cambia el valor de MATCH del CTIMER seleccionado.
 *
 * Si el match deseado está configurado para realizar <em>reload on match</em>, se escribe el nuevo valor de match
 * será una actualización efectiva en cuanto el conteo actual alcance dicho match. Caso contrario, la actualización
 * del valor de match es inmediata.
 *
 * @param[in] match_sel Match a configurar
 * @param[in] match_value_useg Nuevo valor de match, en useg, deseado.
 */
void hal_ctimer_timer_mode_match_change_value(hal_ctimer_match_sel_en match, uint32_t match_value_useg)
{
	if(CTIMER_get_reload_on_match(match))
	{
		CTIMER_write_shadow_register(match, hal_ctimer_calc_match_value(match_value_useg));
	}
	else
	{
		CTIMER_write_match_value(match, hal_ctimer_calc_match_value(match_value_useg));
	}
}

/**
 * @brief Leer estado de match externo
 * @param[in] match Numero de match externo a consultar
 * @return Estado del match actual
 */
uint8_t hal_ctimer_match_read_output(hal_ctimer_match_sel_en match)
{
	return CTIMER_read_match_output(match);
}

/**
 * @brief Pone la señal de salida EM# (External Match #) en 1.
 * @param[in] match Numero de match externo a configurar
 */
void hal_ctimer_match_set_output(hal_ctimer_match_sel_en match)
{
	CTIMER_set_match_output(match);
}

/**
 * @brief Pone la señal de salida EM# (External Match #) en 0.
 * @param[in] match Numero de match externo a configurar
 */
void hal_ctimer_match_clear_output(hal_ctimer_match_sel_en match)
{
	CTIMER_clear_match_output(match);
}

/**
 * @brief Inicializar el CTIMER en modo PWM
 * @param[in] config Configuracion deseada
 */
void hal_ctimer_pwm_mode_init(const hal_ctimer_pwm_config_t *config)
{
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_CTIMER);
	SYSCON_clear_reset(SYSCON_RESET_SEL_CTIMER);

	CTIMER_assert_counter_reset();
	CTIMER_disable_counter();
	CTIMER_clear_counter_reset();

	CTIMER_write_prescaler(config->clock_div);

	CTIMER_config_counter_timer_mode(CTIMER_MODE_TIMER);

	CTIMER_enable_reload_on_match(CTIMER_MATCH_SEL_3);
	CTIMER_enable_reset_on_match(CTIMER_MATCH_SEL_3);
	CTIMER_disable_stop_on_match(CTIMER_MATCH_SEL_3);

	// El canal 3 se utiliza como delimitador del periodo del PWM
	CTIMER_write_match_value(CTIMER_MATCH_SEL_3, hal_ctimer_calc_match_value(config->pwm_period_useg));
	CTIMER_write_shadow_register(CTIMER_MATCH_SEL_3, hal_ctimer_calc_match_value(config->pwm_period_useg));

	if(config->interrupt_on_period)
	{
		CTIMER_enable_interrupt_on_match(CTIMER_MATCH_SEL_3);
		match_callbacks[CTIMER_MATCH_SEL_3] = config->callback;
	}
	else
	{
		CTIMER_disable_interrupt_on_match(CTIMER_MATCH_SEL_3);
		match_callbacks[CTIMER_MATCH_SEL_3] = dummy_irq;
	}

	NVIC_enable_interrupt(NVIC_IRQ_SEL_CTIMER);

	CTIMER_enable_counter();
}

/**
 * @brief Actualizar el periodo en modo PWM
 * @param[in] period_useg Nuevo periodo deseado en microsegundos
 */
void hal_ctimer_pwm_mode_period_set(uint32_t period_useg)
{
	CTIMER_write_shadow_register(CTIMER_MATCH_SEL_3, hal_ctimer_calc_match_value(period_useg));
}

/**
 * @brief Actualizar configuracion de algun canal de PWM
 * @param[in] channel_sel Seleccion de canal a configurar
 * @param[in] channel_config Configuracion del canal de PWM
 */
void hal_ctimer_pwm_mode_channel_config(hal_ctimer_pwm_channel_sel_en channel_sel, const hal_ctimer_pwm_channel_config_t *channel_config)
{
	float aux_calc;

	CTIMER_enable_reload_on_match(channel_sel);

	if(channel_config->duty >= 1000)
	{
		aux_calc = 0.0;
	}
	else if(channel_config->duty > 0)
	{
		aux_calc = (((float) channel_config->duty) * CTIMER_read_match_value(CTIMER_MATCH_SEL_3)) / 1000.0;
		aux_calc = ((float) CTIMER_read_match_value(CTIMER_MATCH_SEL_3)) - aux_calc;
	}
	else
	{
		aux_calc = (float) (CTIMER_read_match_value(CTIMER_MATCH_SEL_3) + 1);
	}

	CTIMER_write_shadow_register(channel_sel, (uint32_t) (aux_calc));

	SWM_init();
	SWM_assign_T0_MAT(channel_sel, channel_config->channel_pin / 32, channel_config->channel_pin % 32);
	SWM_deinit();

	if(channel_config->interrupt_on_action)
	{
		CTIMER_enable_interrupt_on_match(channel_sel);
		match_callbacks[channel_sel] = channel_config->callback;
	}
	else
	{
		CTIMER_disable_interrupt_on_match(channel_sel);
		match_callbacks[channel_sel] = dummy_irq;
	}

	CTIMER_enable_pwm(channel_sel);
}

/**
 * @brief Funcion dummy para inicializar los callbacks de interrupcion
 */
static void dummy_irq(void)
{
	return;
}

/**
 * @brief Calcular el valor que debe ir en el registro de match a partir de un valor de useg deseado
 * @return Valor que debe ir en el registro de match
 */
static uint32_t hal_ctimer_calc_match_value(uint32_t match_value_useg)
{
	float per_clock = hal_syscon_get_system_clock() / (CTIMER_read_prescaler() + 1);

	return (uint32_t) ((per_clock * match_value_useg) / 1e6);
}

/**
 * @brief Interrupcion de CTIMER
 */
void CTIMER0_IRQHandler(void)
{
	uint8_t counter;

	for(counter = 0; counter < MATCH_AMOUNT; counter++)
	{
		if(CTIMER_get_match_irq_flag(counter))
		{
			match_callbacks[counter]();
			CTIMER_clear_match_irq_flag(counter);
		}
	}

	for(counter = 0; counter < CAPTURE_AMOUNT; counter++)
	{
		if(CTIMER_get_capture_irq_flag(counter))
		{
			capture_callbacks[counter]();
			CTIMER_clear_capture_irq_flag(counter);
		}
	}
}
