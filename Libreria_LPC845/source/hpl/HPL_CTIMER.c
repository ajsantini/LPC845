/**
 * @file HPL_CTIMER.c
 * @brief Funciones a nivel de abstraccion del periferico CTIMER (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HRI_CTIMER.h>

volatile CTIMER_per_t * const CTIMER = (volatile CTIMER_per_t *) CTIMER_BASE; //!< Periferico CTIMER
