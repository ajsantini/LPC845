/**
 * @file HAL_SYSCON.h
 * @brief Declaraciones a nivel de aplicacion del periferico SYSCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HAL_SYSCON_H_
#define HAL_SYSCON_H_

#include <stdint.h>

typedef enum
{
	HAL_SYSCON_CLKOUT_SOURCE_SEL_FRO = 0,
	HAL_SYSCON_CLKOUT_SOURCE_SEL_MAIN_CLOCK,
	HAL_SYSCON_CLKOUT_SOURCE_SEL_SYS_PLL,
	HAL_SYSCON_CLKOUT_SOURCE_SEL_EXT_CLOCK,
	HAL_SYSCON_CLKOUT_SOURCE_SEL_WATCHDOG_OSC
}hal_syscon_clkout_source_sel_en;

typedef enum
{
	HAL_SYSCON_FRG_CLOCK_SEL_FRO = 0,
	HAL_SYSCON_FRG_CLOCK_SEL_MAIN_CLOCK,
	HAL_SYSCON_FRG_CLOCK_SEL_SYS_PLL,
	HAL_SYSCON_FRG_CLOCK_SEL_NONE
}hal_syscon_frg_clock_sel_en;

typedef enum
{
	HAL_SYSCON_PERIPHERAL_SEL_UART0 = 0,
	HAL_SYSCON_PERIPHERAL_SEL_UART1,
	HAL_SYSCON_PERIPHERAL_SEL_UART2,
	HAL_SYSCON_PERIPHERAL_SEL_UART3,
	HAL_SYSCON_PERIPHERAL_SEL_UART4,
	HAL_SYSCON_PERIPHERAL_SEL_IIC0,
	HAL_SYSCON_PERIPHERAL_SEL_IIC1,
	HAL_SYSCON_PERIPHERAL_SEL_IIC2,
	HAL_SYSCON_PERIPHERAL_SEL_IIC3,
	HAL_SYSCON_PERIPHERAL_SEL_SPI0,
	HAL_SYSCON_PERIPHERAL_SEL_SPI1
}hal_syscon_peripheral_sel_en;

typedef enum
{
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRO = 0,
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_MAIN,
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRG0,
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRG1,
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRO_DIV,
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_NONE = 7
}hal_syscon_peripheral_clock_sel_en;

typedef enum
{
	HAL_SYSCON_IOCON_GLITCH_SEL_0 = 0,
	HAL_SYSCON_IOCON_GLITCH_SEL_1,
	HAL_SYSCON_IOCON_GLITCH_SEL_2,
	HAL_SYSCON_IOCON_GLITCH_SEL_3,
	HAL_SYSCON_IOCON_GLITCH_SEL_4,
	HAL_SYSCON_IOCON_GLITCH_SEL_5,
	HAL_SYSCON_IOCON_GLITCH_SEL_6,
	HAL_SYSCON_IOCON_GLITCH_SEL_7
}hal_syscon_iocon_glitch_sel_en;

typedef enum
{
	HAL_SYSCON_PLL_SOURCE_SEL_FRO = 0,
	HAL_SYSCON_PLL_SOURCE_SEL_EXT_CLK,
	HAL_SYSCON_PLL_SOURCE_SEL_WATCHDOG,
	HAL_SYSCON_PLL_SOURCE_SEL_FRO_DIV
}hal_syscon_pll_source_sel_en;

/**
 * @brief Obtener la frecuencia actual del main clock
 * @return Frecuencia del main clock en Hz
 */
uint32_t hal_syscon_get_system_clock(void);

/**
 * @brief Obtener la frecuencia actual del FRO
 * @return Frecuencia del FRO en Hz
 */
uint32_t hal_syscon_get_fro_clock(void);

/**
 * @brief Configurar el ext clock a partir de un cristal externo
 * @param[in] crystal_freq Frecuencia del cristal externo utilizado
 * @param[in] use_as_main Si es distinto de cero, se utilizara el oscilador a cristal como main clock
 */
void hal_syscon_config_external_crystal(uint32_t crystal_freq, uint8_t use_as_main);

/**
 * @brief Configurar el clock FRO
 * @param[in] direct Si es distinto de cero se omite el divisor del FRO
 * @param[in] use_as_main Si es distinto de cero, se utilizara el FRO como main clock
 */
void hal_syscon_config_fro_direct(uint8_t direct, uint8_t use_as_main);

/**
 * @brief Configurar el pin de clock out (salida de clock hacia afuera)
 * @param[in] port Numero de puerto por donde sacar el clock out
 * @param[in] pin Numero de pin por donde sacar el clock out
 * @param[in] clock_source Fuente deseada para la salida clock out
 * @param[in] divider Divisor deseado para la salida clock out
 */
void hal_syscon_config_clkout(uint8_t port, uint8_t pin, hal_syscon_clkout_source_sel_en clock_source, uint8_t divider);

/**
 * @brief Configurar el divisor fraccional
 *
 * El divisor siempre se debe fijar en 256 para estos MCU.
 *
 * @param[in] inst Instancia de FRG a configurar
 * @param[in] clock_source Fuente de clock de entrada para el FRG
 * @param[in] mul Multiplicador deseado
 */
void hal_syscon_config_frg(uint8_t inst, hal_syscon_frg_clock_sel_en clock_source, uint32_t mul);

/**
 * @brief Fijar la fuente de clock de un periferico
 * @param[in] peripheral Periferico deseado
 * @param[in] clock_source Fuente de clock deseada
 */
void hal_syscon_set_peripheral_clock_source(hal_syscon_peripheral_sel_en peripheral, hal_syscon_peripheral_clock_sel_en clock_source);

/**
 * @brief Obtener la frecuencia de clock en Hz configurada para cierto periferico
 * @param[in] peripheral Periferico deseado
 * @return Frecuencia en Hz del clock del periferico
 */
uint32_t hal_syscon_get_peripheral_clock(hal_syscon_peripheral_sel_en peripheral);

/**
 * @brief Configurar divisor para el clock de glitches del IOCON
 * @param[in] sel Seleccion de divisor
 * @param[in] div Valor de division deseado
 */
void hal_syscon_set_iocon_glitch_divider(hal_syscon_iocon_glitch_sel_en sel, uint32_t div);

/**
 * @brief Configurar el PLL
 * @param[in] clock_source Fuente de clock de referencia para el PLL
 * @param[in] freq Frecuencia deseada de salida del PLL
 */
void hal_syscon_config_pll(hal_syscon_pll_source_sel_en clock_source, uint32_t freq);

/**
 * @brief Obtener frecuencia actual configurada del PLL
 * @return Frecuencia actual del PLL en Hz
 */
uint32_t hal_syscon_get_pll_clock(void);

#endif /* HAL_SYSCON_H_ */
