/*
 * HPL_SYSCON.c
 *
 *  Created on: Mar 19, 2020
 *      Author: augusto
 */

#include <HRI_SYSCON.h>

volatile SYSCON_per_t * const SYSCON = (SYSCON_per_t *) SYSCON_BASE; //!< Periferico SYSCON
