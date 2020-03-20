/**
 * @file HAL_SYSTICK.h
 * @brief Declaraciones a nivel de aplicacion del periferico SYSICK (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HAL_SYSTICK_H_
#define HAL_SYSTICK_H_

#include <stdint.h>

/**
 * @brief Inicializacion del SYSTICK
 * @param[in] tick_us Tiempo en microsegundos deseado para el tick
 * @param[in] callback Funcion a llamar en cada tick
 */
void hal_systick_init(uint32_t tick_us, void (*callback)(void));

/**
 * @brief Actualizar callback del SYSTICK
 * @param[in] callback Nuevo callback a ejecutar en cada tick
 */
void hal_systick_update_callback(void (*callback)(void));

#endif /* HAL_SYSTICK_H_ */
