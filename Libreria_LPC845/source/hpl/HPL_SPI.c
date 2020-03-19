/**
 * @file HPL_SPI.c
 * @brief Funciones a nivel de abstraccion de periferico para el SPI (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <stdio.h>
#include <stdint.h>
#include <HPL_SPI.h>
#include <HPL_SYSCON.h>
#include <HRI_SYSCON.h>
#include <HRI_SWM.h>
#include <HRI_NVIC.h>
#include <HRI_SPI.h>

static void (*spi_transfer_ready_callback[])(void) = { //!< Callbacks registrados a la finalizacion de una transferencia por SPI
		NULL,
		NULL
};

/**
 * @brief Inicializacion de un modulo SPI
 * @param config Puntero a la configuracion deseada
 * @return Resultado de la inicializacion
 */
int32_t SPI_init(uint8_t spi_selection, const SPI_config_t * config)
{
	// Habilitacion de la Switch-Matrix
	SYSCON->SYSAHBCLKCTRL0.SWM = 1;

	switch(spi_selection)
	{
	case 0:
	{
		// Seleccion de clock
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_SPI0, config->clock_selection);

		SWM->PINASSIGN3.SPI0_SCK_IO = (config->sck_port * 32) + config->sck_pin;
		SWM->PINASSIGN4.SPI0_MISO_IO = (config->miso_port * 32) + config->miso_pin;
		SWM->PINASSIGN4.SPI0_MOSI_IO = (config->mosi_port * 32) + config->mosi_pin;

		if(config->ssel_mask & (1 << 0))
		{
			SWM->PINASSIGN4.SPI0_SSEL0_IO = (config->ssel_ports[0] * 32) + config->ssel_pins[0];
		}

		if(config->ssel_mask & (1 << 1))
		{
			SWM->PINASSIGN4.SPI0_SSEL1_IO = (config->ssel_ports[1] * 32) + config->ssel_pins[1];
		}

		if(config->ssel_mask & (1 << 2))
		{
			SWM->PINASSIGN5.SPI0_SSEL2_IO = (config->ssel_ports[2] * 32) + config->ssel_pins[2];
		}

		if(config->ssel_mask & (1 << 3))
		{
			SWM->PINASSIGN5.SPI0_SSEL3_IO = (config->ssel_ports[3] * 32) + config->ssel_pins[3];
		}

		break;
	}

	case 1:
	{


		break;
	}

	default:
	{
		// Habilitacion de la Switch-Matrix
		SYSCON->SYSAHBCLKCTRL0.SWM = 0;

		return SPI_INIT_INVALID_SPI;

		break;
	}
	} // End switch(spi_selection)

	// Inhabilitacion de la Switch-Matrix
	SYSCON->SYSAHBCLKCTRL0.SWM = 0;

	// Configuracion del periferico
	SPI[spi_selection]->CFG.MASTER = 1;
	SPI[spi_selection]->CFG.LSBF = config->order;
	SPI[spi_selection]->CFG.CPHA = config->CPHA;
	SPI[spi_selection]->CFG.CPOL = config->CPOL;
	SPI[spi_selection]->CFG.LOOP = config->loop;


}
