/**
 * @file HPL_SPI.h
 * @brief Declaraciones a nivel de abstraccion de periferico del SPI (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HPL_SPI_H_
#define HPL_SPI_H_

#include <stdint.h>
#include <HPL_SYSCON.h>

#define		SPI_INIT_OK					0
#define		SPI_INIT_INVALID_SPI		-1


typedef enum
{
	SPI_ORDER_MSBF = 0,
	SPI_ORDER_LSBF
}SPI_order_select_en;

typedef enum
{
	SPI_CPHA_CHANGE = 0,
	SPI_CPHA_CAPTURE
}SPI_CPHA_select_en;

typedef enum
{
	SPI_CPOL_LOW = 0,
	SPI_CPOL_HIGH
}SPI_CPOL_select_en;

typedef struct
{
	SPI_order_select_en order;
	SPI_CPHA_select_en CPHA;
	SPI_CPOL_select_en CPOL;
	uint8_t loop;
	peripheral_clock_selection_en clock_selection;
	uint32_t baudrate;
	uint8_t sck_port;
	uint8_t sck_pin;
	uint8_t mosi_port;
	uint8_t mosi_pin;
	uint8_t miso_port;
	uint8_t miso_pin;
	uint8_t ssel_mask;
	uint8_t ssel_ports[4];
	uint8_t ssel_pins[4];
	void (*transfer_ready_callback)(void);
}SPI_config_t;

/**
 * @brief Inicializacion de un modulo SPI
 * @param config Puntero a la configuracion deseada
 * @return Resultado de la inicializacion
 */
int32_t SPI_init(uint8_t spi_selection, const SPI_config_t * config);

#endif /* HPL_SPI_H_ */
