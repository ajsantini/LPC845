/**
 * @file HPL_PININT.c
 * @brief Funciones a nivel de abstraccion de periferico para el PININT (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <HRI_PININT.h>

volatile PININT_per_t * const PININT = (PININT_per_t *) PININT_BASE; //!< Periferico PININT
