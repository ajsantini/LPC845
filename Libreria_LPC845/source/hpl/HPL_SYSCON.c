/**
 * @file HPL_SYSCON.c
 * @brief Funciones a nivel de abstraccion de periferico para el SYSCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdint.h>
#include <HPL_SYSCON.h>
#include <HRI_SYSCON.h>

volatile SYSCON_per_t * const SYSCON = (SYSCON_per_t *) SYSCON_BASE; //!< Periferico SYSCON

static uint32_t current_clock = 12000000; //!< Frecuencia del MAINCLK actual (12MHz en startup)

/**
 * @brief Obtencion de la frecuencia de clock actual
 * @return Frecuencia del clock actual en Hz
 */
uint32_t SYSCON_get_system_clock(void)
{
	return current_clock;
}
