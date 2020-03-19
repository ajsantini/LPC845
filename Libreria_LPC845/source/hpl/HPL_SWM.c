/*
 * HPL_SWM.c
 *
 *  Created on: Mar 19, 2020
 *      Author: augusto
 */

#include <HPL_SWM.h>
#include <HRI_SWM.h>

volatile SWM_per_t * const SWM = (SWM_per_t *) SWM_BASE; //!< Periferico SWM
