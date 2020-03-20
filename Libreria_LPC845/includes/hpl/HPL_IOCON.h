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
#include <HPL_SYSCON.h>

typedef enum
{
	IOCON_PULL_NONE = 0,
	IOCON_PULL_DOWN,
	IOCON_PULL_UP,
	IOCON_PULL_REPEATER
}IOCON_pull_mode_en;

typedef enum
{
	IOCON_SAMPLE_MODE_BYPASS = 0,
	IOCON_SAMPLE_MODE_1_CLOCK,
	IOCON_SAMPLE_MODE_2_CLOCK,
	IOCON_SAMPLE_MODE_3_CLOCK
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
}IOCON_clk_div_en;

typedef enum
{
	IOCON_IIC_MODE_STANDARD = 0,
	IOCON_IIC_MODE_GPIO,
	IOCON_IIC_MODE_FAST_MODE
}IOCON_iic_mode_en;

typedef struct
{
	IOCON_pull_mode_en pull_mode;
	uint8_t hysteresis;
	uint8_t invert_input;
	uint8_t open_drain;
	IOCON_sample_mode_en sample_mode;
	IOCON_clk_div_en clk_sel;
	uint8_t dac_mode;
	IOCON_iic_mode_en iic_mode;
}IOCON_config_t;

/**
 * @brief Inicializacion del modulo IOCON
 *
 * Unicamente habilita el clock del modulo
 */
static inline void IOCON_init(void)
{
	SYSCON_enable_clock(SYSCON_ENABLE_DISABLE_CLOCK_SEL_IOCON);
}

/**
 * @brief Inhabilitacion del modulo IOCON
 *
 * Unicamente inhabilita el clock del modulo
 */
static inline void IOCON_deinit(void)
{
	SYSCON_disable_clock(SYSCON_ENABLE_DISABLE_CLOCK_SEL_IOCON);
}

/**
 * @brief Configuracion de un pin
 * @param[in] port Puerto del pin a configurar
 * @param[in] pin Numero del pin a configurar
 * @param[in] pin_config Puntero a estructura de configuracion del pin
 */
void IOCON_config_io(uint8_t port, uint8_t pin, const IOCON_config_t *config);

#endif /* HPL_IOCON_H_ */
