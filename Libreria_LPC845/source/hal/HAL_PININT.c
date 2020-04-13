/**
 * @file HAL_PININT.c
 * @brief Funciones a nivel de aplicacion del periferico PININT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_PININT.h>
#include <HAL_UART.h>
#include <HPL_PININT.h>
#include <HPL_SYSCON.h>
#include <HPL_SWM.h>
#include <HPL_NVIC.h>

static void dummy_irq_callback(void);

static void (*pinint_callbacks[8])(void) = { //!< Callbacks para las 8 interrupciones disponibles
		dummy_irq_callback,
		dummy_irq_callback,
		dummy_irq_callback,
		dummy_irq_callback,
		dummy_irq_callback,
		dummy_irq_callback,
		dummy_irq_callback
};

static void hal_pinint_handle_irq(hal_pinint_channel_en channel);

/**
 * @brief Inicialización del periférico
 */
void hal_pinint_init(void)
{
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_GPIO_INT);
}

/**
 * @brief De-Inicialización del periférico
 */
void hal_pinint_deinit(void)
{
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT0);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT1);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT2);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT3);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT4);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT5_DAC1);

	// Para el canal 6 y 7 no deshabilito interrupciones, dado que pueden ser usadas por UART3 y 4

	SYSCON_disable_clock(SYSCON_ENABLE_CLOCK_SEL_GPIO_INT);
}

/**
 * @brief Configurar interrupciones de pin
 * @param[in] config Configuración de interrupciones de pin
 * @see hal_pinint_config_t
 */
void hal_pinint_pin_interrupt_config(const hal_pinint_config_t *config)
{
	PININT_set_interrupt_mode(config->channel, config->mode);

	if(config->mode == HAL_PININT_INTERRUPT_MODE_LEVEL)
	{
		if(config->int_on_level == HAL_PININT_LEVEL_INT_HIGH)
		{
			PININT_enable_high_level(config->channel);
		}
		else
		{
			PININT_disable_high_level(config->channel);
		}
	}
	else
	{
		if(config->int_on_rising_edge)
		{
			PININT_enable_rising_edge(config->channel);
		}

		if(config->int_on_falling_edge)
		{
			PININT_enable_falling_edge(config->channel);
		}
	}

	if(config->callback != NULL)
	{
		pinint_callbacks[config->channel] = config->callback;
	}
	else
	{
		pinint_callbacks[config->channel] = dummy_irq_callback;
	}

	SYSCON_set_pinint_pin(config->channel, config->portpin);

	switch(config->channel)
	{
	case 0: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT0); break; }
	case 1: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT1); break; }
	case 2: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT2); break; }
	case 3: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT3); break; }
	case 4: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT4); break; }
	case 5: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT5_DAC1); break; }
	case 6: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT6_UART3); break; }
	case 7: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT7_UART4); break; }
	}
}

/**
 * @brief Registrar callback a llamar en interrupción de PININTn
 * @param[in] channel Canal al cual registrar el callback
 * @param[in] new_callback Puntero a función a ejecutar
 *
 * @note Cabe recordar que estos callbacks se ejecutan bajo el contexto de una interrupción, por lo que el
 * mismo deberá tener todas las consideraciones necesarias
 *
 * @see hal_pinint_channel_en
 * @see hal_pinint_callback_t
 */
void hal_pinint_register_callback(hal_pinint_channel_en channel, hal_pinint_callback_t new_callback)
{
	if(config->callback != NULL)
	{
		pinint_callbacks[channel] = new_callback;;
	}
	else
	{
		pinint_callbacks[channel] = dummy_irq_callback;
	}
}

/**
 * @brief Funcion dummy para inicializar los punteros de interrupciones
 */
static void dummy_irq_callback(void)
{
	return;
}

/**
 * @brief Manejo de interrupciones para el modulo
 * @param[in] channel Canal que generó la itnerrupción
 */
static void hal_pinint_handle_irq(hal_pinint_channel_en channel)
{
	pinint_callbacks[channel]();

	if(PININT_get_interrupt_mode(channel) == PININT_INTERRUPT_MODE_EDGE)
	{
		PININT_clear_edge_level_irq(channel);
	}
}

/**
 * @brief Interrupción para PININT0
 */
void PININT0_IRQHandler(void)
{
	hal_pinint_handle_irq(0);
}

/**
 * @brief Interrupción para PININT1
 */
void PININT1_IRQHandler(void)
{
	hal_pinint_handle_irq(1);
}

/**
 * @brief Interrupción para PININT2
 */
void PININT2_IRQHandler(void)
{
	hal_pinint_handle_irq(2);
}

/**
 * @brief Interrupción para PININT3
 */
void PININT3_IRQHandler(void)
{
	hal_pinint_handle_irq(3);
}

/**
 * @brief Interrupción para PININT4
 */
void PININT4_IRQHandler(void)
{
	hal_pinint_handle_irq(4);
}

/**
 * @brief Interrupción para PININT5
 */
void PININT5_IRQHandler(void)
{
	hal_pinint_handle_irq(5);
}

/**
 * @brief Interrupción para PININT6 y USART3
 */
void PININT6_IRQHandler(void)
{
	if(PININT->IST.PSTAT & (1 << 6))
	{
		hal_pinint_handle_irq(6);
	}

	UART3_irq();
}

/**
 * @brief Interrupción para PININT7 y USART4
 */
void PININT7_IRQHandler(void)
{
	if(PININT->IST.PSTAT & (1 << 7))
	{
		hal_pinint_handle_irq(7);
	}

	UART4_irq();
}
