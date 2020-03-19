/**
 * @file HPL_PININT.c
 * @brief Funciones a nivel de abstraccion de periferico para el PININT (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdio.h>
#include <stdint.h>
#include <HPL_PININT.h>
#include <HRI_SYSCON.h>
#include <HRI_PININT.h>
#include <HRI_NVIC.h>
#include <HRI_SWM.h>
#include <HRI_UART.h>

volatile PININT_per_t * const PININT = (PININT_per_t *) PININT_BASE; //!< Periferico PININT

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

/**
 * @brief Inicializa modulo de GPIO_INT
 *
 * Unicamente habilita el clock del modulo
 */
void PININT_init(void)
{
	SYSCON->SYSAHBCLKCTRL0.GPIO_INT = 1;
}

/**
 * @brief Inhabilita modulo de GPIO_INT
 *
 * Unicamente inhabilita el clock del modulo
 */
void PININT_deinit(void)
{
	SYSCON->SYSAHBCLKCTRL0.GPIO_INT = 0;
}

/**
 * @brief Configurar interrupciones de pin
 * @param[in] config Puntero a configuracion de interrupciones de pin
 */
void PININT_configure_pin_interrupt(PININT_config_t *config)
{
	// Modo: Nivel o Flanco
	if(config->mode)
	{
		PININT->ISEL.PMODE |= (1 << config->channel);
	}
	else
	{
		PININT->ISEL.PMODE &= ~(1 << config->channel);
	}

	// Interrupciones en flanco/nivel positivo
	if(config->int_on_rising_edge)
	{
		PININT->IENR.ENRL |= (1 << config->channel);
	}
	else
	{
		PININT->IENR.ENRL &= ~(1 << config->channel);
	}

	// Interrupciones en flanco/nivel negativo
	if(config->int_on_falling_edge)
	{
		PININT->IENF.ENAF |= (1 << config->channel);
	}
	else
	{
		PININT->IENF.ENAF &= ~(1 << config->channel);
	}
}

/**
 * @brief Interrupcion para PININT0
 */
void PININT0_IRQHandler(void)
{
	pinint_callbacks[0]();

	// Limpio flag de interrupcion
	PININT->IST.PSTAT |= (1 << 0);
}

/**
 * @brief Interrupcion para PININT1
 */
void PININT1_IRQHandler(void)
{
	pinint_callbacks[1]();

	// Limpio flag de interrupcion
	PININT->IST.PSTAT |= (1 << 1);
}

/**
 * @brief Interrupcion para PININT2
 */
void PININT2_IRQHandler(void)
{
	pinint_callbacks[2]();

	// Limpio flag de interrupcion
	PININT->IST.PSTAT |= (1 << 2);
}

/**
 * @brief Interrupcion para PININT3
 */
void PININT3_IRQHandler(void)
{
	pinint_callbacks[3]();

	// Limpio flag de interrupcion
	PININT->IST.PSTAT |= (1 << 3);
}

/**
 * @brief Interrupcion para PININT4
 */
void PININT4_IRQHandler(void)
{
	pinint_callbacks[4]();

	// Limpio flag de interrupcion
	PININT->IST.PSTAT |= (1 << 4);
}

/**
 * @brief Interrupcion para PININT5
 */
void PININT5_IRQHandler(void)
{
	pinint_callbacks[5]();

	// Limpio flag de interrupcion
	PININT->IST.PSTAT |= (1 << 5);
}

/**
 * @brief Interrupcion para PININT6 y USART3
 */
void PININT6_IRQHandler(void)
{
	if(PININT->IST.PSTAT & (1 << 6))
	{
		pinint_callbacks[6]();

		// Limpio flag de interrupcion
		PININT->IST.PSTAT |= (1 << 6);
	}

	UART3_irq();
}

/**
 * @brief Interrupcion para PININT7 y USART4
 */
void PININT7_IRQHandler(void)
{
	if(PININT->IST.PSTAT & (1 << 7))
	{
		pinint_callbacks[7]();

		// Limpio flag de interrupcion
		PININT->IST.PSTAT |= (1 << 7);
	}

	UART4_irq();
}

/**
 * @brief Registrar callback a llamar en interrupcion de PININTn
 * @param[in] new_callback Puntero a funcion a ejecutar
 * @param[in] interrupt Numero de PININT al cual registrar el callback
 * * @return Estado del registro del callback nuevo
 */
int32_t PININT_register_callback(void (*new_callback)(void), uint32_t interrupt)
{
	if(!SYSCON->SYSAHBCLKCTRL0.GPIO_INT)
	{
		return PININT_REGISTER_CALLBACK_NOT_CLOCKED;
	}

	if(interrupt > 7)
	{
		return PININT_REGISTER_CALLBACK_INVALID_INTERRUPT;
	}

	pinint_callbacks[interrupt] = new_callback;

	return PININT_REGISTER_CALLBACK_SUCCESS;
}

/**
 * @brief Funcion dummy para inicializar los punteros de interrupciones
 */
static void dummy_irq_callback(void)
{
	return;
}
