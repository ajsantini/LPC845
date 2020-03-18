/**
 * @file HPL_SWM.c
 * @brief Funciones a nivel de abstraccion de periferico para el SWM (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdint.h>
#include <HRI_SWM.h>
#include <HRI_SYSCON.h>

volatile SWM_per_t * const SWM = (SWM_per_t *) SWM_BASE; //!< Periferico SWM

/**
 * @brief Inicializacion de la Switch Matrix
 */
void SWM_init(void)
{
	SYSCON->SYSAHBCLKCTRL0.SWM = 1;
}

/**
 * @brief Deinicializacion de la Switch Matrix
 */
void SWM_deinit(void)
{
	SYSCON->SYSAHBCLKCTRL0.SWM = 0;
}
