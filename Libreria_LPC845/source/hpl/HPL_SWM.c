/**
 * @file HPL_SWM.c
 * @brief Funciones a nivel de abstraccion de periferico para el SWM (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdint.h>
#include <HRI_SWM.h>

volatile SWM_per_t * const SWM = (SWM_per_t *) SWM_BASE; //!< Periferico SWM
