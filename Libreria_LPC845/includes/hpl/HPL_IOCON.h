/**
 * @file HPL_IOCON.h
 * @brief Declaraciones a nivel de abstraccion de periferico del IOCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_IOCON_H_
#define HPL_IOCON_H_

#include <stdint.h>

#define	IOCON_CONFIG_SUCCESS				0
#define	IOCON_CONFIG_IOCON_NOT_CLOCKED		-1
#define	IOCON_CONFIG_INVALID_PORT			-2
#define	IOCON_CONFIG_INVALID_PIN			-3
#define	IOCON_CONFIG_INVALID_PORTPIN		-4

typedef enum
{
	PULL_NONE = 0,
	PULL_DOWN,
	PULL_UP,
	PULL_REPEATER
}IOCON_pull_mode_en;

typedef enum
{
	SAMPLE_MODE_BYPASS = 0,
	SAMPLE_MODE_1_CLOCK,
	SAMPLE_MODE_2_CLOCK,
	SAMPLE_MODE_3_CLOCK
}IOCON_sample_mode_en;

typedef enum
{
	IOCON_CLK_DIV_0 = 0,
	IOCON_CLK_DIV_1,
	IOCON_CLK_DIV_2,
	IOCON_CLK_DIV_3,
	IOCON_CLK_DIV_4,
	IOCON_CLK_DIV_5,
	IOCON_CLK_DIV_6
}IOCON_clk_sel_en;

typedef enum
{
	IIC_MODE_STANDARD = 0,
	IIC_MODE_GPIO,
	IIC_MODE_FAST_MODE
}IOCON_iic_mode_en;

typedef struct
{
	IOCON_pull_mode_en mode;
	uint8_t hysteresis;
	uint8_t invert_input;
	uint8_t open_drain;
	IOCON_sample_mode_en sample_mode;
	IOCON_clk_sel_en clk_sel;
	uint8_t dac_mode;
	IOCON_iic_mode_en iic_mode;
}IOCON_config_t;

/**
 * @brief Inicializacion del modulo IOCON
 *
 * Unicamente habilita el clock del modulo
 */
void IOCON_init(void);

/**
 * @brief Inhabilitacion del modulo IOCON
 *
 * Unicamente inhabilita el clock del modulo
 */
void IOCON_deinit(void);

/**
 * @brief Configuracion de un pin
 * @param[in] port Puerto del pin a configurar
 * @param[in] pin Numero del pin a configurar
 * @param[in] pin_config Puntero a estructura de configuracion del pin
 * @return Estado de configuracion del pin
 */
int32_t IOCON_config_io(uint32_t port, uint32_t pin, IOCON_config_t *pin_config);

#endif /* HPL_IOCON_H_ */
