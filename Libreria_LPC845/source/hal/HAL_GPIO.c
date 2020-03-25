/**
 * @file HAL_GPIO.c
 * @brief Funciones a nivel de aplicacion del periferico GPIO (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_GPIO.h>
#include <HPL_SYSCON.h>
#include <HPL_GPIO.h>

/**
 * @brief Inicializar un puerto
 * @param[in] port Puerto a inicializar
 */
void hal_gpio_init(hal_gpio_port_en port)
{
	if(port == HAL_GPIO_PORT_0)
	{
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_GPIO0);
	}
	else
	{
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_GPIO1);
	}
}

/**
 * @brief Fijar direccion de una GPIO
 * @param[in] portpin Numero de puerto/pin a configurar
 * @param[in] dir Direccion deseada
 * @param[in] initial_state Estado inicial (aplica para salidas nada mas)
 */
void hal_gpio_set_dir(hal_gpio_portpin_en portpin, hal_gpio_dir_en dir, uint8_t initial_state)
{
	if(dir == HAL_GPIO_DIR_INPUT)
	{
		GPIO_write_dir_clear(portpin / 32, 1 << (portpin % 32));
	}
	else
	{
		GPIO_write_dir_set(portpin / 32, 1 << (portpin % 32));

		if(initial_state)
		{
			GPIO_write_set(portpin / 32, 1 << (portpin % 32));
		}
	}
}

/**
 * @brief Fijar estado activo de una GPIO
 * @param[in] portpin Numero de puerto/pin a accionar
 */
void hal_gpio_set_pin(hal_gpio_portpin_en portpin)
{
	GPIO_write_set(portpin / 32, 1 << (portpin % 32));
}

/**
 * @brief Fijar estado inactivo de una GPIO
 * @param[in] portpin Numero de puerto/pin a accionar
 */
void hal_gpio_clear_pin(hal_gpio_portpin_en portpin)
{
	GPIO_write_clear(portpin / 32, 1 << (portpin % 32));
}

/**
 * @brief Invertir estado de una GPIO
 * @param[in] portpin Numero de puerto/pin a accionar
 */
void hal_gpio_toggle_pin(hal_gpio_portpin_en portpin)
{
	GPIO_write_toggle(portpin / 32, 1 << (portpin % 32));
}

/**
 * @brief Leer el estado de una GPIO
 * @param[in] portpin Numero de puerto/pin a accionar
 * @return Estado actual de la GPIO
 */
uint8_t hal_gpio_read_pin(hal_gpio_portpin_en portpin)
{
	if(GPIO_read_masked_portpin(portpin / 32))
	{
		return 1;
	}
	else
	{
		return 0;
	}

	return 0;
}
