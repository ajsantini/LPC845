/**
 * @file HRI_SYSTICK.c
 * @brief Declaración del periférico SYSTICK (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_SYSTICK.h>

volatile SYSTICK_reg_t * const SYSTICK = (SYSTICK_reg_t *) SYSTICK_BASE; //!< Periferico SYSTICK


