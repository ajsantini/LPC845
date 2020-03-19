/**
 * @file HPL_GPIO.c
 * @brief Funciones a nivel de abstraccion de periferico para el GPIO (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdint.h>
#include <HPL_GPIO.h>
#include <HRI_GPIO.h>
#include <HRI_SYSCON.h>

/**
 * @brief Inicializa el puerto
 *
 * Lo unico que hace es habilitar el clock para el puerto requerido
 *
 * @param[in] port Puerto a inicializar. Si se pasa un puerto invalido no hace nada
 */
void GPIO_init(uint32_t port)
{
	if(port > 1)
	{
		return;
	}

	if(port == 0)
	{
		SYSCON->SYSAHBCLKCTRL0.GPIO0 = 1;
	}
	else
	{
		SYSCON->SYSAHBCLKCTRL0.GPIO1 = 1;
	}
}

/**
 * @brief Inhabilita el puerto
 *
 * Lo unico que hace es inhabilitar el clock para el puerto requerido
 *
 * @param[in] port Puerto a inhabilitar. Si se pasa un puerto invalido no hace nada
 */
void GPIO_deinit(uint32_t port)
{
	if(port > 1)
	{
		return;
	}

	if(port == 0)
	{
		SYSCON->SYSAHBCLKCTRL0.GPIO0 = 0;
	}
	else
	{
		SYSCON->SYSAHBCLKCTRL0.GPIO1 = 0;
	}
}

/**
 * @brief Inicializar un pin como entrada o salida
 * @param[in] port Puerto del pin a inicializar
 * @param[in] pin Numero del pin a inicializar
 * @param[in] dir Direccion del pin a inicializar. GPIO_DIR_INPUT o GPIO_DIR_OUTPUT
 * @param[in] initial_state Estado inicial del pin en caso de ser configurado como salida
 * @return Estado de inicializacion de pin
 */
int32_t GPIO_set_dir(uint32_t port, uint32_t pin, uint8_t dir, uint8_t initial_state)
{
	if(port > 1)
	{
		return GPIO_SET_DIR_INVALID_PORT;
	}

	if(pin > 31)
	{
		return GPIO_SET_DIR_INVALID_PIN;
	}

	if(port == 1 && pin > 21)
	{
		return GPIO_SET_DIR_INVALID_PORTPIN;
	}

	if(port == 0)
	{
		if(!SYSCON->SYSAHBCLKCTRL0.GPIO0)
		{
			return GPIO_SET_DIR_NOT_CLOCKED;
		}
	}

	if(port == 1)
	{
		if(!SYSCON->SYSAHBCLKCTRL0.GPIO1)
		{
			return GPIO_SET_DIR_NOT_CLOCKED;
		}
	}

	if(dir)
	{
		// Configuracion como salida
		if(port == 0)
		{
			GPIO->DIRSET0.DIRSETP |= (1 << pin);
		}
		else
		{
			GPIO->DIRSET1.DIRSETP |= (1 << pin);
		}

		if(initial_state)
		{
			if(port == 0)
			{
				GPIO->SET0.SETP |= (1 << pin);
			}
			else
			{
				GPIO->SET1.SETP |= (1 << pin);
			}
		}
	}
	else
	{
		// Configuracion como entrada
		if(port == 0)
		{
			GPIO->DIRCLR0.DIRCLRP |= (1 << pin);
		}
		else
		{
			GPIO->DIRCLR1.DIRCLRP |= (1 << pin);
		}
	}

	return GPIO_SET_DIR_SUCCESS;
}

/**
 * @brief Fijar nivel del pin en esatado alto
 * @param[in] port Puerto del pin a accionar
 * @param[in] pin Numero del pin a accionar
 * @return Estado de accionado del pin
 */
int32_t GPIO_set_pin(uint32_t port, uint32_t pin)
{
	if(port > 1)
	{
		return GPIO_SET_PIN_INVALID_PORT;
	}

	if(pin > 31)
	{
		return GPIO_SET_PIN_INVALID_PIN;
	}

	if(port == 1 && pin > 21)
	{
		return GPIO_SET_PIN_INVALID_PORTPIN;
	}

	if(port == 0)
	{
		if(!SYSCON->SYSAHBCLKCTRL0.GPIO0)
		{
			return GPIO_SET_PIN_NOT_CLOCKED;
		}
	}

	if(port == 1)
	{
		if(!SYSCON->SYSAHBCLKCTRL0.GPIO1)
		{
			return GPIO_SET_PIN_NOT_CLOCKED;
		}
	}

	if(port == 0)
	{
		GPIO->SET0.SETP |= (1 << pin);
	}
	else
	{
		GPIO->SET1.SETP |= (1 << pin);
	}

	return GPIO_SET_PIN_SUCCESS;
}

/**
 * @brief Fijar nivel del pin en estado bajo
 * @param[in] port Puerto del pin a accionar
 * @param[in] pin Numero del pin a accionar
 * @return Estado de accionado del pin
 */
int32_t GPIO_clear_pin(uint32_t port, uint32_t pin)
{
	if(port > 1)
	{
		return GPIO_CLEAR_PIN_INVALID_PORT;
	}

	if(pin > 31)
	{
		return GPIO_CLEAR_PIN_INVALID_PIN;
	}

	if(port == 1 && pin > 21)
	{
		return GPIO_CLEAR_PIN_INVALID_PORTPIN;
	}

	if(port == 0)
	{
		if(!SYSCON->SYSAHBCLKCTRL0.GPIO0)
		{
			return GPIO_CLEAR_PIN_NOT_CLOCKED;
		}
	}

	if(port == 1)
	{
		if(!SYSCON->SYSAHBCLKCTRL0.GPIO1)
		{
			return GPIO_CLEAR_PIN_NOT_CLOCKED;
		}
	}

	if(port == 0)
	{
		GPIO->CLR0.CLRP |= (1 << pin);
	}
	else
	{
		GPIO->CLR1.CLRP |= (1 << pin);
	}

	return GPIO_CLEAR_PIN_SUCCESS;
}

/**
 * @brief Invertir estado actual del pin
 * @param[in] port Puerto del pin a accionar
 * @param[in] pin Numero del pin a accionar
 * @return Estado de inversion del pin
 */
int32_t GPIO_toggle_pin(uint32_t port, uint32_t pin)
{
	if(port > 1)
	{
		return GPIO_TOGGLE_PIN_INVALID_PORT;
	}

	if(pin > 31)
	{
		return GPIO_TOGGLE_PIN_INVALID_PIN;
	}

	if(port == 1 && pin > 21)
	{
		return GPIO_TOGGLE_PIN_INVALID_PORTPIN;
	}

	if(port == 0)
	{
		if(!SYSCON->SYSAHBCLKCTRL0.GPIO0)
		{
			return GPIO_TOGGLE_PIN_NOT_CLOCKED;
		}
	}

	if(port == 1)
	{
		if(!SYSCON->SYSAHBCLKCTRL0.GPIO1)
		{
			return GPIO_TOGGLE_PIN_NOT_CLOCKED;
		}
	}

	if(port == 0)
	{
		GPIO->NOT0.NOTP |= (1 << pin);
	}
	else
	{
		GPIO->NOT1.NOTP |= (1 << pin);
	}

	return GPIO_TOGGLE_PIN_SUCCESS;
}

/**
 * @brief Lectura del estado de un pin
 * @param[in] port Puerto del pin a leer
 * @param[in] pin Numero del pin a leer
 * @return Lectura del pin. Puede devolver errores
 */
int32_t GPIO_read_pin(uint32_t port, uint32_t pin)
{
	if(port > 1)
	{
		return GPIO_READ_PIN_INVALID_PORT;
	}

	if(pin > 31)
	{
		return GPIO_READ_PIN_INVALID_PIN;
	}

	if(port == 1 && pin > 21)
	{
		return GPIO_READ_PIN_INVALID_PORTPIN;
	}

	if(port == 0)
	{
		if(!SYSCON->SYSAHBCLKCTRL0.GPIO0)
		{
			return GPIO_READ_PIN_NOT_CLOCKED;
		}
	}

	if(port == 1)
	{
		if(!SYSCON->SYSAHBCLKCTRL0.GPIO1)
		{
			return GPIO_READ_PIN_NOT_CLOCKED;
		}
	}

	if(port == 0)
	{
		if(GPIO->PIN0.PORT & (1 << pin))
			return 1;
		else
			return 0;
	}
	else
	{
		if(GPIO->PIN1.PORT & (1 << pin))
			return 1;
		else
			return 0;
	}
}
