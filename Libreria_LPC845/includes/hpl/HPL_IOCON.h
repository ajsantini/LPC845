/**
 * @file HPL_IOCON.h
 * @brief Declaraciones a nivel de abstraccion de periferico del IOCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_IOCON_H_
#define HPL_IOCON_H_

#include <HPL_SYSCON.h>
#include <HRI_IOCON.h>

extern volatile IOCON_per_t * const IOCON; //!< Periferico IOCON
extern volatile IOCON_PIO_reg_t * const IOCON_PIN_TABLE[2][32];//!< Tabla de registros de configuracion

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
}IOCON_clk_sel_en;

typedef enum
{
	IOCON_IIC_MODE_STANDARD = 0,
	IOCON_IIC_MODE_GPIO,
	IOCON_IIC_MODE_FAST_MODE
}IOCON_iic_mode_en;

/**
 * @brief Inicializacion del modulo IOCON
 *
 * Unicamente habilita el clock del modulo
 */
static inline void IOCON_init(void)
{
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_IOCON);
}

/**
 * @brief Inhabilitacion del modulo IOCON
 *
 * Unicamente inhabilita el clock del modulo
 */
static inline void IOCON_deinit(void)
{
	SYSCON_disable_clock(SYSCON_ENABLE_CLOCK_SEL_IOCON);
}

/**
 * @brief Configurar modo de funcionamiento (pull up, pull down, etc) en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 * @param[in] pull_mode Modo de funcionamiento
 */
static inline void IOCON_config_pull_mode(uint8_t port, uint8_t pin, IOCON_pull_mode_en pull_mode)
{
	IOCON_PIN_TABLE[port][pin]->MODE = pull_mode;
}

/**
 * @brief Habilitar histeresis en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
static inline void IOCON_enable_hysteresis(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->HYS = 1;
}

/**
 * @brief Inhabilitar histeresis en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
static inline void IOCON_disable_hysteresis(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->HYS = 0;
}

/**
 * @brief Habilitar inversion en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
static inline void IOCON_enable_invert(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->INV = 1;
}

/**
 * @brief Inhabilitar inversion en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
static inline void IOCON_disable_invert(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->INV = 0;
}

/**
 * @brief Habilitar modo open drain en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
static inline void IOCON_enable_open_drain(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->OD = 1;
}

/**
 * @brief Inhabilitar modo open drain en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 */
static inline void IOCON_disable_open_drain(uint8_t port, uint8_t pin)
{
	IOCON_PIN_TABLE[port][pin]->OD = 0;
}

/**
 * @brief Configurar modo de sampleo en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 * @param[in] sample_mode Modo de sampleo
 */
static inline void IOCON_config_sample_mode(uint8_t port, uint8_t pin, IOCON_sample_mode_en sample_mode)
{
	IOCON_PIN_TABLE[port][pin]->S_MODE = sample_mode;
}

/**
 * @brief Configurar fuente utilizada para el sampleo en un pin
 * @param[in] port Numero de puerto
 * @param[in] pin Numero de pin
 * @param[in] clock_source Fuente de clock deseada
 */
static inline void IOCON_config_clock_source(uint8_t port, uint8_t pin, IOCON_clk_sel_en clock_source)
{
	IOCON_PIN_TABLE[port][pin]->CLK_DIV = clock_source;
}

/**
 * @brief Inhabilitar pull-up/pull-down en entrada analógica
 * @param[in] channel Canal de \e ADC
 */
static inline void IOCON_disable_pullup_adc(uint8_t channel)
{
	switch(channel)
	{
	case 0: { IOCON_PIN_TABLE[0][7]->MODE = IOCON_PULL_NONE; break; }
	case 1: { IOCON_PIN_TABLE[0][6]->MODE = IOCON_PULL_NONE; break; }
	case 2: { IOCON_PIN_TABLE[0][14]->MODE = IOCON_PULL_NONE; break; }
	case 3: { IOCON_PIN_TABLE[0][23]->MODE = IOCON_PULL_NONE; break; }
	case 4: { IOCON_PIN_TABLE[0][22]->MODE = IOCON_PULL_NONE; break; }
	case 5: { IOCON_PIN_TABLE[0][21]->MODE = IOCON_PULL_NONE; break; }
	case 6: { IOCON_PIN_TABLE[0][20]->MODE = IOCON_PULL_NONE; break; }
	case 7: { IOCON_PIN_TABLE[0][19]->MODE = IOCON_PULL_NONE; break; }
	case 8: { IOCON_PIN_TABLE[0][18]->MODE = IOCON_PULL_NONE; break; }
	case 9: { IOCON_PIN_TABLE[0][17]->MODE = IOCON_PULL_NONE; break; }
	case 10: { IOCON_PIN_TABLE[0][13]->MODE = IOCON_PULL_NONE; break; }
	case 11: { IOCON_PIN_TABLE[0][4]->MODE = IOCON_PULL_NONE; break; }
	}
}

/**
 * @brief Habilitar DAC0 en PIO0_17
 */
static inline void IOCON_enable_dac0(void)
{
	IOCON_PIN_TABLE[0][17]->MODE = IOCON_PULL_NONE;
	IOCON_PIN_TABLE[0][17]->DACMODE = 1;
}

/**
 * @brief Habilitar DAC1 en PIO0_29
 */
static inline void IOCON_enable_dac1(void)
{
	IOCON_PIN_TABLE[0][29]->MODE = IOCON_PULL_NONE;
	IOCON_PIN_TABLE[0][29]->DACMODE = 1;
}

/**
 * @brief Inhabilitar DAC0 en PIO0_17
 */
static inline void IOCON_disable_dac0(void)
{
	IOCON_PIN_TABLE[0][17]->DACMODE = 0;
}

/**
 * @brief Inhabilitar DAC1 en PIO0_29
 */
static inline void IOCON_disable_dac1(void)
{
	IOCON_PIN_TABLE[0][29]->DACMODE = 0;
}

/**
 * @brief Habilitar IIC0_SCL en PIO0_10
 * @param[in] iic_mode Modo de IIC
 */
static inline void IOCON_select_iic0_scl(IOCON_iic_mode_en iic_mode)
{
	IOCON_PIN_TABLE[0][10]->I2CMODE = iic_mode;
}

/**
 * @brief Habilitar IIC SDA en PIO0_11
 * @param[in] iic_mode Modo de IIC
 */
static inline void IOCON_select_iic0_sda(IOCON_iic_mode_en iic_mode)
{
	IOCON_PIN_TABLE[0][11]->I2CMODE = iic_mode;
}

#endif /* HPL_IOCON_H_ */
