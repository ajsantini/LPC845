/**
 * @file HPL_PININT.c
 * @brief Funciones a nivel de abstraccion de periferico para el PININT (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdio.h>
#include <stdint.h>
#include <HPL_PININT.h>
#include <HRI_SYSCON.h>
#include <HRI_PININT.h>
#include <HRI_NVIC.h>
#include <HRI_SWM.h>
#include <HRI_UART.h>

volatile PININT_per_t * const PININT = (PININT_per_t *) PININT_BASE; //!< Periferico PININT
