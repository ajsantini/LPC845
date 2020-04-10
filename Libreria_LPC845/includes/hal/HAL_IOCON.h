/**
 * @file HAL_IOCON.h
 * @brief Declaraciones a nivel de aplicacion del periferico IOCON (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @defgroup IOCON Control de Entrada/Salida (IOCON)
 *
 * Prueba IOCON.
 *
 * Da da da.
 *
 * @{
 */

#ifndef HAL_IOCON_H_
#define HAL_IOCON_H_

#include <HPL_IOCON.h>
#include <HAL_GPIO.h>

typedef enum
{
	HAL_IOCON_PULL_NONE = 0,
	HAL_IOCON_PULL_DOWN,
	HAL_IOCON_PULL_UP,
	HAL_IOCON_PULL_REPEATER
}hal_iocon_pull_mode_en;

typedef enum
{
	HAL_IOCON_SAMPLE_MODE_BYPASS = 0,
	HAL_IOCON_SAMPLE_MODE_1_CLOCK,
	HAL_IOCON_SAMPLE_MODE_2_CLOCK,
	HAL_IOCON_SAMPLE_MODE_3_CLOCK
}hal_iocon_sample_mode_en;

typedef enum
{
	HAL_IOCON_CLK_DIV_0 = 0,
	HAL_IOCON_CLK_DIV_1,
	HAL_IOCON_CLK_DIV_2,
	HAL_IOCON_CLK_DIV_3,
	HAL_IOCON_CLK_DIV_4,
	HAL_IOCON_CLK_DIV_5,
	HAL_IOCON_CLK_DIV_6
}hal_iocon_clk_sel_en;

typedef enum
{
	HAL_IOCON_IIC_MODE_STANDARD = 0,
	HAL_IOCON_IIC_MODE_GPIO,
	HAL_IOCON_IIC_MODE_FAST_MODE
}hal_iocon_iic_mode_en;

typedef struct
{
	hal_iocon_pull_mode_en pull_mode;
	uint8_t hysteresis;
	uint8_t invert_input;
	uint8_t open_drain;
	hal_iocon_sample_mode_en sample_mode;
	hal_iocon_clk_sel_en clk_sel;
	uint8_t dac_mode;
	hal_iocon_iic_mode_en iic_mode;
}hal_iocon_config_t;

/**
 * @brief Configuracion de un pin
 * @param[in] portpin Puerto/pin a configurar
 * @param[in] pin_config Puntero a estructura de configuracion del pin
 */
void hal_iocon_config_io(hal_gpio_portpin_en portpin, const hal_iocon_config_t *config);

#endif /* HAL_IOCON_H_ */

/**
 * @}
 */
