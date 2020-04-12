/**
 * @file HRI_WKT.c
 * @brief Declaración del periférico WKT (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_WKT.h>

volatile WKT_per_t * const WKT = (volatile WKT_per_t *) WKT_BASE; //!< Periferico WKT
