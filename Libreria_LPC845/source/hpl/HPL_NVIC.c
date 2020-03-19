/*
 * HPL_NVIC.c
 *
 *  Created on: Mar 19, 2020
 *      Author: augusto
 */

#include <HRI_NVIC.h>

volatile NVIC_per_t * const NVIC = (NVIC_per_t *) NVIC_BASE; //!< Periferico NVIC
