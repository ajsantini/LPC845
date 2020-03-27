/**
 * @file HAL_CTIMER.c
 * @brief Funciones a nivel de aplicacion del periferico CTIMER (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_CTIMER.h>
#include <HPL_CTIMER.h>
#include <HPL_SYSCON.h>
#include <HPL_NVIC.h>

#define		MATCH_AMOUNT		4
#define		CAPTURE_AMOUNT		4

static void dummy_irq(void);

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
void hal_ctimer_init_timer_mode(const hal_ctimer_timer_config_t *config)
{
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_CTIMER);
	SYSCON_clear_reset(SYSCON_RESET_SEL_CTIMER);

	CTIMER_disable_counter();
	CTIMER_assert_counter_reset();

	CTIMER_write_prescaler(config->clock_div);


}

/**
 * @brief Funcion dummy para inicializar los callbacks de interrupcion
 */
static void dummy_irq(void)
{
	return;
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
