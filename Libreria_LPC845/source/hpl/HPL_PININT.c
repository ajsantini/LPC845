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

static void (*pinint_callbacks[8])(void) = { //!< Callbacks para las 8 interrupciones disponibles
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
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
 * @return Estado de la configuracion de interrupcion de pin
 */
int32_t PININT_configure_pin_interrupt(PININT_config_t *config)
{
	if(config->port > 1)
	{
		return PININT_CONFIGURE_INVALID_PORT;
	}

	if(config->pin > 31)
	{
		return PININT_CONFIGURE_INVALID_PIN;
	}

	if(config->port == 1 && config->pin > 9)
	{
		return PININT_CONFIGURE_INVALID_PORTPIN;
	}

	if(config->interrupt > 8)
	{
		return PININT_CONFIGURE_INVALID_INTERRUPT;
	}

	if(!SYSCON->SYSAHBCLKCTRL0.GPIO_INT)
	{
		return PININT_CONFIGURE_NOT_CLOCKED;
	}

	// Modo: Nivel o Flanco
	if(config->mode)
	{
		PININT->ISEL.PMODE |= (1 << config->interrupt);
	}
	else
	{
		PININT->ISEL.PMODE &= ~(1 << config->interrupt);
	}

	// Interrupciones en flanco/nivel positivo
	if(config->int_on_rising_edge)
	{
		PININT->IENR.ENRL |= (1 << config->interrupt);
	}
	else
	{
		PININT->IENR.ENRL &= ~(1 << config->interrupt);
	}

	// Interrupciones en flanco/nivel negativo
	if(config->int_on_falling_edge)
	{
		PININT->IENF.ENAF |= (1 << config->interrupt);
	}
	else
	{
		PININT->IENF.ENAF &= ~(1 << config->interrupt);
	}

	// Registro callback a llamar para la interrupcion configurada;
	PININT_register_callback(config->int_callback, config->interrupt);

	// Habilitacion de interrupcion de pin
	SYSCON->PINTSEL[config->interrupt].INTPIN = (config->port * 32) + config->pin;

	// Habilitacion de interrupciones en el NVIC
	switch(config->interrupt)
	{
	case 0:
		NVIC->ISER0.ISE_PININT0 = 1;
		break;
	case 1:
		NVIC->ISER0.ISE_PININT1 = 1;
		break;
	case 2:
		NVIC->ISER0.ISE_PININT2 = 1;
		break;
	case 3:
		NVIC->ISER0.ISE_PININT3 = 1;
		break;
	case 4:
		NVIC->ISER0.ISE_PININT4 = 1;
		break;
	case 5:
		NVIC->ISER0.ISE_PININT5 = 1;
		break;
	case 6:
		NVIC->ISER0.ISE_PININT6 = 1;
		break;
	case 7:
		NVIC->ISER0.ISE_PININT7 = 1;
		break;
	default: break;
	}

	return PININT_CONFIGURE_SUCCESS;
}

/**
 * @brief Interrupcion para PININT0
 */
void PININT0_IRQHandler(void)
{
	if(pinint_callbacks[0] != NULL)
	{
		pinint_callbacks[0]();

		// Limpio flag de interrupcion
		PININT->IST.PSTAT |= (1 << 0);
	}
}

/**
 * @brief Interrupcion para PININT1
 */
void PININT1_IRQHandler(void)
{
	if(pinint_callbacks[1] != NULL)
	{
		pinint_callbacks[1]();

		// Limpio flag de interrupcion
		PININT->IST.PSTAT |= (1 << 1);
	}
}

/**
 * @brief Interrupcion para PININT2
 */
void PININT2_IRQHandler(void)
{
	if(pinint_callbacks[2] != NULL)
	{
		pinint_callbacks[2]();

		// Limpio flag de interrupcion
		PININT->IST.PSTAT |= (1 << 2);
	}
}

/**
 * @brief Interrupcion para PININT3
 */
void PININT3_IRQHandler(void)
{
	if(pinint_callbacks[3] != NULL)
	{
		pinint_callbacks[3]();

		// Limpio flag de interrupcion
		PININT->IST.PSTAT |= (1 << 3);
	}
}

/**
 * @brief Interrupcion para PININT4
 */
void PININT4_IRQHandler(void)
{
	if(pinint_callbacks[4] != NULL)
	{
		pinint_callbacks[4]();

		// Limpio flag de interrupcion
		PININT->IST.PSTAT |= (1 << 4);
	}
}

/**
 * @brief Interrupcion para PININT5
 */
void PININT5_IRQHandler(void)
{
	if(pinint_callbacks[5] != NULL)
	{
		pinint_callbacks[5]();

		// Limpio flag de interrupcion
		PININT->IST.PSTAT |= (1 << 5);
	}
}

/**
 * @brief Interrupcion para PININT6 y USART3
 */
void PININT6_IRQHandler(void)
{
	if(PININT->IST.PSTAT & (1 << 6))
	{
		if(pinint_callbacks[6] != NULL)
		{
			pinint_callbacks[6]();

			// Limpio flag de interrupcion
			PININT->IST.PSTAT |= (1 << 6);
		}
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
		if(pinint_callbacks[7] != NULL)
		{
			pinint_callbacks[7]();

			// Limpio flag de interrupcion
			PININT->IST.PSTAT |= (1 << 7);
		}
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
