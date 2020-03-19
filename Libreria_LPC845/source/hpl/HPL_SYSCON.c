/**
 * @file HPL_SYSCON.c
 * @brief Funciones a nivel de abstraccion de periferico para el SYSCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <HRI_SYSCON.h>

volatile SYSCON_per_t * const SYSCON = (SYSCON_per_t *) SYSCON_BASE; //!< Periferico SYSCON
