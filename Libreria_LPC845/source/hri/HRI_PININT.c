/**
 * @file HRI_PININT.c
 * @brief Declaración del periférico PININT (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_PININT.h>

volatile PININT_per_t * const PININT = (PININT_per_t *) PININT_BASE; //!< Periferico PININT
