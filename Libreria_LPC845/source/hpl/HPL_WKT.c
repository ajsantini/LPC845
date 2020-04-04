/**
 * @file HPL_WKT.c
 * @brief Funciones a nivel de abstraccion de periferico para el WKT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HRI_WKT.h>

volatile WKT_per_t * const WKT = (volatile WKT_per_t *) WKT_BASE; //!< Periferico WKT
