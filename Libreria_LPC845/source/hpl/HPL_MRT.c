/**
 * @file HPL_MRT.c
 * @brief Funciones a nivel de abstraccion de periferico para el MRT (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_MRT.h>

volatile MRT_per_t * const MRT = (MRT_per_t *) MRT_BASE; //!< Periferico MRT
