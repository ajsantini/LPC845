/**
 * @file HAL_SPI.h
 * @brief Declaraciones a nivel de aplicacion del periferico SPI (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HAL_SPI_H_
#define HAL_SPI_H_

#include <stdint.h>
#include <HAL_SYSCON.h>
#include <HAL_GPIO.h>

#define		HAL_SPI_DUMMY_BYTE		(0xFF)

typedef enum
{
	HAL_SPI_0 = 0,
	HAL_SPI_1
}hal_spi_sel_en;

typedef enum
{
	HAL_SPI_DATA_LENGTH_1_BIT = 0,
	HAL_SPI_DATA_LENGTH_2_BIT,
	HAL_SPI_DATA_LENGTH_3_BIT,
	HAL_SPI_DATA_LENGTH_4_BIT,
	HAL_SPI_DATA_LENGTH_5_BIT,
	HAL_SPI_DATA_LENGTH_6_BIT,
	HAL_SPI_DATA_LENGTH_7_BIT,
	HAL_SPI_DATA_LENGTH_8_BIT,
	HAL_SPI_DATA_LENGTH_9_BIT,
	HAL_SPI_DATA_LENGTH_10_BIT,
	HAL_SPI_DATA_LENGTH_11_BIT,
	HAL_SPI_DATA_LENGTH_12_BIT,
	HAL_SPI_DATA_LENGTH_13_BIT,
	HAL_SPI_DATA_LENGTH_14_BIT,
	HAL_SPI_DATA_LENGTH_15_BIT,
	HAL_SPI_DATA_LENGTH_16_BIT
}hal_spi_data_length_en;

typedef enum
{
	HAL_SPI_CLOCK_MODE_0 = 0,
	HAL_SPI_CLOCK_MODE_1,
	HAL_SPI_CLOCK_MODE_2,
	HAL_SPI_CLOCK_MODE_3
}hal_spi_clock_mode_en;

typedef enum
{
	HAL_SPI_SSEL_POLARITY_LOW = 0,
	HAL_SPI_SSEL_POLARITY_HIGH
}hal_spi_ssel_polarity_en;

typedef enum
{
	HAL_SPI_SSEL_SELECTION_0 = 0,
	HAL_SPI_SSEL_SELECTION_1,
	HAL_SPI_SSEL_SELECTION_2,
	HAL_SPI_SSEL_SELECTION_3,
	HAL_SPI_SSEL_SELECTION_OTHER
}hal_spi_ssel_sel_en;

typedef struct
{
	hal_syscon_peripheral_clock_sel_en clock_source;
	uint8_t pre_delay;
	uint8_t post_delay;
	uint8_t frame_delay;
	uint8_t transfer_delay;
	hal_gpio_portpin_en sck_portpin;
	hal_gpio_portpin_en miso_portpin;
	hal_gpio_portpin_en mosi_portpin;
	hal_gpio_portpin_en ssel_portpin[4];
	hal_spi_ssel_polarity_en ssel_polarity[4];
	void (*tx_free_callback)(void);
	void (*rx_ready_callback)(void);
}hal_spi_master_mode_config_t;

typedef struct
{
	hal_spi_clock_mode_en clock_mode;
	uint16_t clock_div;
}hal_spi_master_mode_tx_config_t;

typedef struct
{
	uint32_t data : 16;
	uint32_t ssel0_n : 1;
	uint32_t ssel1_n : 1;
	uint32_t ssel2_n : 1;
	uint32_t ssel3_n : 1;
	uint32_t eot : 1;
	uint32_t eof : 1;
	uint32_t rxignore : 1;
	uint32_t : 1;
	uint32_t data_length : 4;
	uint32_t : 4;
}hal_spi_master_mode_tx_data_t;

/**
 * @brief Inicializar SPI en modo master
 * @param[in] inst Instancia de SPI a inicializar
 * @param[in] config Configuracion deseada
 */
void hal_spi_master_mode_init(hal_spi_sel_en inst, const hal_spi_master_mode_config_t *config);

/**
 * @brief Leer el dato recibido
 * @param[in] inst Instancia a consultar
 * @return Dato recibido
 */
uint16_t hal_spi_master_mode_rx_data(hal_spi_sel_en inst);

/**
 * @brief Configurar la transmision
 * @param[in] inst Instancia a configurar
 * @param[in] config Configuracion para la transmision deseada
 */
void hal_spi_master_mode_config_tx(hal_spi_sel_en inst, const hal_spi_master_mode_tx_config_t *config);

/**
 * @brief Transmitir dato
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir, con controles asociados
 */
void hal_spi_master_mode_tx_data(hal_spi_sel_en inst, const hal_spi_master_mode_tx_data_t *data);

#endif /* HAL_SPI_H_ */
