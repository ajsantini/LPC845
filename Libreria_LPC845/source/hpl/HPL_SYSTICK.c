/**
 * @file HPL_SYSTICK.c
 * @brief Funciones a nivel de abstraccion de periferico para el SYSTICK (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <HRI_SYSTICK.h>

volatile SYSTICK_reg_t * const SYSTICK = (SYSTICK_reg_t *) SYSTICK_BASE; //!< Periferico SYSTICK


