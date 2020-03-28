/**
 * @file HAL_CTIMER.c
 * @brief Funciones a nivel de aplicacion del periferico CTIMER (LPC845)
 * @author Augusto Santini
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
 * @param[in] config Configuracion deseada
 */
void hal_ctimer_timer_mode_init(const hal_ctimer_timer_config_t *config)
{
	uint8_t counter;

	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_CTIMER);
	SYSCON_clear_reset(SYSCON_RESET_SEL_CTIMER);

	CTIMER_assert_counter_reset();
	CTIMER_disable_counter();
	CTIMER_clear_counter_reset();

	CTIMER_write_prescaler(config->clock_div);

	CTIMER_config_counter_timer_mode(CTIMER_MODE_TIMER);

	for(counter = 0; counter < MATCH_AMOUNT; counter++)
	{
		if(config->match_config[counter] != NULL)
		{
			const hal_ctimer_match_config_t *this_match = config->match_config[counter];

			CTIMER_write_match_value(counter, hal_ctimer_calc_match_value(this_match->match_value_useg));

			CTIMER_config_external_match(counter, this_match->match_action);

			SWM_init();

			if(this_match->enable_external_pin)
			{
				SWM_assign_T0_MAT(counter, this_match->match_pin / 32, this_match->match_pin % 32);
			}
			else
			{
				SWM_assign_T0_MAT(counter, 0xFF, 0xFF);
			}

			SWM_deinit();

			if(this_match->interrupt_on_match)
			{
				CTIMER_enable_interrupt_on_match(counter);
				match_callbacks[counter] = this_match->callback;
			}
			else
			{
				CTIMER_disable_interrupt_on_match(counter);
				match_callbacks[counter] = dummy_irq;
			}

			if(this_match->reset_on_match)
			{
				CTIMER_enable_reset_on_match(counter);
			}
			else
			{
				CTIMER_disable_reset_on_match(counter);
			}

			if(this_match->stop_on_match)
			{
				CTIMER_enable_stop_on_match(counter);
			}
			else
			{
				CTIMER_disable_stop_on_match(counter);
			}

			if(this_match->reload_on_match)
			{
				CTIMER_enable_reload_on_match(counter);
			}
			else
			{
				CTIMER_disable_reload_on_match(counter);
			}
		}
	}

	NVIC_enable_interrupt(NVIC_IRQ_SEL_CTIMER);
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
 * @brief Inicializar el CTIMER en modo PWM
 * @param[in] config Configuracion deseada
 */
void hal_ctimer_pwm_mode_init(const hal_ctimer_pwm_config_t *config)
{
	uint8_t counter;

	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_CTIMER);
	SYSCON_clear_reset(SYSCON_RESET_SEL_CTIMER);

	CTIMER_assert_counter_reset();
	CTIMER_disable_counter();
	CTIMER_clear_counter_reset();

	CTIMER_write_prescaler(config->clock_div);

	CTIMER_config_counter_timer_mode(CTIMER_MODE_TIMER);

	// El canal 3 se utiliza como delimitador del periodo del PWM
	CTIMER_write_match_value(CTIMER_MATCH_SEL_3, hal_ctimer_calc_match_value(config->pwm_period_useg));

	CTIMER_enable_reset_on_match(CTIMER_MATCH_SEL_3);
	CTIMER_disable_stop_on_match(CTIMER_MATCH_SEL_3);

	if(config->interrupt_on_period)
	{
		CTIMER_enable_interrupt_on_capture(CTIMER_MATCH_SEL_3);
		match_callbacks[3] = config->callback;
	}
	else
	{
		CTIMER_disable_interrupt_on_capture(CTIMER_MATCH_SEL_3);
		match_callbacks[3] = dummy_irq;
	}

	CTIMER_disable_reload_on_match(CTIMER_MATCH_SEL_3);

	for(counter = 0; counter < PWM_CHANNELS; counter++)
	{
		if(config->channels[counter] != NULL)
		{
			const hal_ctimer_pwm_channel_config_t *this_channel = config->channels[counter];
			float aux_calc;

			if(this_channel->duty >= 1000)
			{
				aux_calc = 0.0;
			}
			else if(this_channel->duty > 0)
			{
				aux_calc = (((float) this_channel->duty) * CTIMER_read_match_value(CTIMER_MATCH_SEL_3)) / 1000.0;
				aux_calc = ((float) CTIMER_read_match_value(CTIMER_MATCH_SEL_3)) - aux_calc;
			}
			else
			{
				aux_calc = (float) (CTIMER_read_match_value(CTIMER_MATCH_SEL_3) + 1);
			}

			CTIMER_write_match_value(counter, (uint32_t) (aux_calc));

			SWM_init();
			SWM_assign_T0_MAT(counter, this_channel->channel_pin / 32, this_channel->channel_pin % 32);
			SWM_deinit();

			if(this_channel->interrupt_on_action)
			{
				CTIMER_enable_interrupt_on_match(counter);
				match_callbacks[counter] = this_channel->callback;
			}
			else
			{
				CTIMER_disable_interrupt_on_match(counter);
				match_callbacks[counter] = dummy_irq;
			}

			CTIMER_enable_pwm(counter);
		}
	}

	NVIC_enable_interrupt(NVIC_IRQ_SEL_CTIMER);
}

/**
 * @brief Habilitar el conteo del ctimer
 */
void hal_ctimer_pwm_mode_run(void)
{
	CTIMER_enable_counter();
}

/**
 * @brief Inhabilitar el conteo del ctimer
 */
void hal_ctimer_pwm_mode_stop(void)
{
	CTIMER_disable_counter();
}

/**
 * @brief Reiniciar el conteo del ctimer
 */
void hal_ctimer_pwm_mode_reset(void)
{
	CTIMER_assert_counter_reset();
	CTIMER_clear_counter_reset();
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
