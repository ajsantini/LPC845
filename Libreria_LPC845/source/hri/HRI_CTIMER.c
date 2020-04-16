/**
 * @file HRI_CTIMER.c
 * @brief Declaración del periférico CTIMER (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_CTIMER.h>

volatile CTIMER_per_t * const CTIMER = (volatile CTIMER_per_t *) CTIMER_BASE; //!< Periferico CTIMER
