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

#endif /* HAL_SYSCON_H_ */
