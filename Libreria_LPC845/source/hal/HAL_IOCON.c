/**
 * @file HAL_IOCON.c
 * @brief Funciones a nivel de aplicacion del periferico IOCON (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_IOCON.h>
#include <HPL_IOCON.h>
#include <HPL_SYSCON.h>

/**
 * @brief Configuracion de un pin
 *
 * Nótese que las configuraciones de modo analógico no están en la estructura de configuración
 * @ref hal_iocon_config_t. Esto es debido a que dichas configuraciones se realizarán en el periférico
 * correspondiente, sea el @ref ADC o @ref DAC.
 *
 * @param[in] portpin Puerto/pin a configurar
 * @param[in] config Puntero a estructura de configuracion del pin
 * @pre Configuración de divisores de clock de bancos de filtro de glitches en caso de ser necesario.
 */
void hal_iocon_config_io(hal_gpio_portpin_en portpin, const hal_iocon_config_t *config)
{
	uint8_t port = HAL_GPIO_PORTPIN_TO_PORT(portpin);
	uint8_t pin = HAL_GPIO_PORTPIN_TO_PIN(portpin);

	IOCON_init();

	if(portpin == HAL_GPIO_PORTPIN_0_11)
	{
		// Este pin tiene I2C
		IOCON->PIO0_11.I2CMODE = config->iic_mode;
	}
	else if(portpin == HAL_GPIO_PORTPIN_0_10)
	{
		// Este pin tiene I2C
		IOCON->PIO0_10.I2CMODE = config->iic_mode;
	}
	else
	{
		// Los pines que no tienen I2C, tienen MODE
		IOCON_PIN_TABLE[port][pin]->MODE = config->pull_mode;
	}

	IOCON_PIN_TABLE[port][pin]->HYS = config->hysteresis;
	IOCON_PIN_TABLE[port][pin]->INV = config->invert_input;
	IOCON_PIN_TABLE[port][pin]->OD = config->open_drain;
	IOCON_PIN_TABLE[port][pin]->S_MODE = config->sample_mode;
	IOCON_PIN_TABLE[port][pin]->CLK_DIV = config->clk_sel;

	IOCON_deinit();
}
