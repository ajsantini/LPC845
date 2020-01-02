/**
 * @file HPL_SYSTICK.h
 * @brief Declaraciones a nivel de abstraccion de periferico del SYSICK (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_SYSTICK_H_
#define HPL_SYSTICK_H_

#include <stdint.h>

/**
 * @brief Inicializar SYSTICK
 * @param[in] tick_us Tiempo de cada tick en microsegundos (usegs)
 * @param[in] systick_hook Puntero a funcion a ejecutar en la interrupcion de SYSTICK
 */
void SYSTICK_init(uint32_t tick_us, void (*systick_hook)(void));

/**
 * @brief Registrar un callback para llamar en la interrupcion de SYSTICK
 * @param[in] new_callback Puntero a funcion a ser ejecutado en la interrupcion de SYSTICK
 */
void SYSTICK_update_callback(void (*new_callback)(void));

#endif /* HPL_SYSTICK_H_ */
