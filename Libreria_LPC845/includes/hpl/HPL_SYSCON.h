/**
 * @file HPL_SYSCON.h
 * @brief Declaraciones a nivel de abstraccion de periferico del SYSCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_SYSCON_H_
#define HPL_SYSCON_H_

#include <stdint.h>

/**
 * @brief Obtencion de la frecuencia de clock actual
 * @return Frecuencia del clock actual en Hz
 */
uint32_t SYSCON_get_system_clock(void);

#endif /* HPL_SYSCON_H_ */
