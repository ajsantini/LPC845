/**
 * @file HRI_SWM.c
 * @brief Declaración del periférico SWM (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HPL_SWM.h>
#include <HRI_SWM.h>

volatile SWM_per_t * const SWM = (SWM_per_t *) SWM_BASE; //!< Periferico SWM
