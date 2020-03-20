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

#endif /* HAL_SYSCON_H_ */
