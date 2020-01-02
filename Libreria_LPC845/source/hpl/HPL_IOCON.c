/**
 * @file HPL_IOCON.c
 * @brief Funciones a nivel de abstraccion de periferico para el IOCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdint.h>
#include <HPL_IOCON.h>
#include <HRI_IOCON.h>
#include <HRI_SYSCON.h>

volatile IOCON_per_t * const IOCON = (IOCON_per_t *) IOCON_BASE; //!< Periferico IOCON

/**
 * @brief Inicializacion del modulo IOCON
 *
 * Unicamente habilita el clock del modulo
 */
void IOCON_init(void)
{
	SYSCON->SYSAHBCLKCTRL0.IOCON = 1;
}

/**
 * @brief Inhabilitacion del modulo IOCON
 *
 * Unicamente inhabilita el clock del modulo
 */
void IOCON_deinit(void)
{
	SYSCON->SYSAHBCLKCTRL0.IOCON = 0;
}

/**
 * @brief Configuracion de un pin
 * @param[in] port Puerto del pin a configurar
 * @param[in] pin Numero del pin a configurar
 * @param[in] pin_config Puntero a estructura de configuracion del pin
 * @return Estado de configuracion del pin
 */
int32_t IOCON_config_io(uint32_t port, uint32_t pin, IOCON_config_t *pin_config)
{
	IOCON_PIO_reg_t *pio_reg;
	IOCON_PIO_reg_t pio_reg_to_write;

	if(port > 1)
	{
		return IOCON_CONFIG_INVALID_PORT;
	}

	if(pin > 31)
	{
		return IOCON_CONFIG_INVALID_PIN;
	}

	if(port == 1 && pin > 21)
	{
		return IOCON_CONFIG_INVALID_PORTPIN;
	}

	if(!SYSCON->SYSAHBCLKCTRL0.IOCON)
	{
		return IOCON_CONFIG_IOCON_NOT_CLOCKED;
	}

	// Clipeo valores de parametros
	if(pin_config->hysteresis)
		pin_config->hysteresis = 1;

	if(pin_config->invert_input)
		pin_config->invert_input = 1;

	if(pin_config->open_drain)
		pin_config->open_drain = 1;

	if(pin_config->dac_mode)
		pin_config->dac_mode = 1;

	/*
	 * Hay pocos pines que tienen DAC o I2C.
	 * Los considero primero, si no son, son pines normales.
	 */

	if(port == 0 && pin == 17)
	{
		// Este pin tiene configuracion DAC
		IOCON->PIO0_17.DACMODE = pin_config->dac_mode;
	}
	else if(port == 0 && pin == 11)
	{
		// Este pin tiene I2C
		IOCON->PIO0_11.I2CMODE = pin_config->iic_mode;
	}
	else if(port == 0 && pin == 10)
	{
		// Este pin tiene I2C
		IOCON->PIO0_10.I2CMODE = pin_config->iic_mode;
	}

	// Mas alla de las funciones especificas de los pines de arriba
	// configuro lo que todos tienen
	if(port == 0)
	{
		// Port 0
		switch(pin)
		{
		case 0:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_0;
			break;
		case 1:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_1;
			break;
		case 2:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_2;
			break;
		case 3:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_3;
			break;
		case 4:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_4;
			break;
		case 5:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_5;
			break;
		case 6:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_6;
			break;
		case 7:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_7;
			break;
		case 8:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_8;
			break;
		case 9:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_9;
			break;
		case 10:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_10;
			break;
		case 11:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_11;
			break;
		case 12:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_12;
			break;
		case 13:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_13;
			break;
		case 14:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_14;
			break;
		case 15:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_15;
			break;
		case 16:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_16;
			break;
		case 17:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_17;
			break;
		case 18:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_18;
			break;
		case 19:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_19;
			break;
		case 20:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_20;
			break;
		case 21:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_21;
			break;
		case 22:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_22;
			break;
		case 23:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_23;
			break;
		case 24:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_24;
			break;
		case 25:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_25;
			break;
		case 26:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_26;
			break;
		case 27:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_27;
			break;
		case 28:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_28;
			break;
		case 29:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_29;
			break;
		case 30:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_30;
			break;
		case 31:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_31;
			break;
		default:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO0_0;
			break;
		} // End switch
	}
	else
	{
		// Port 1
		switch(pin)
		{
		case 0:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_0;
			break;
		case 1:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_1;
			break;
		case 2:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_2;
			break;
		case 3:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_3;
			break;
		case 4:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_4;
			break;
		case 5:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_5;
			break;
		case 6:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_6;
			break;
		case 7:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_7;
			break;
		case 8:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_8;
			break;
		case 9:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_9;
			break;
		default:
			pio_reg = (IOCON_PIO_reg_t *) &IOCON->PIO1_0;
			break;
		} // End switch
	}

	// Mantengo configuraciones de DAC/IIC de haberlas
	pio_reg_to_write = *pio_reg;

	// Los pines que tienen I2C no tienen MODE
	if(!(port == 0 && (pin == 10 || pin == 11)))
	{
		pio_reg_to_write.MODE = pin_config->mode & 0x03;
	}

	pio_reg_to_write.HYS = pin_config->hysteresis;
	pio_reg_to_write.INV = pin_config->invert_input;
	pio_reg_to_write.OD = pin_config->open_drain;
	pio_reg_to_write.S_MODE = pin_config->sample_mode;
	pio_reg_to_write.CLK_DIV = pin_config->clk_sel;

	*pio_reg = pio_reg_to_write;

	return IOCON_CONFIG_SUCCESS;
}
