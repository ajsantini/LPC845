/**
 * @file HRI_SYSCON.c
 * @brief Declaración del periférico SYSCON (LPC845)
 * @author Augusto Santini
 * @date 4/20120
 * @version 1.0
 */

#include <HRI_SYSCON.h>

volatile SYSCON_per_t * const SYSCON = (SYSCON_per_t *) SYSCON_BASE; //!< Periferico SYSCON
