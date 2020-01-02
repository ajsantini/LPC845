/**
 * @file HPL_NVIC.c
 * @brief Funciones a nivel de abstraccion de periferico para el NVIC (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <HRI_NVIC.h>

volatile NVIC_per_t * const NVIC = (NVIC_per_t *) NVIC_BASE; //!< Periferico NVIC
