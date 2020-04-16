/**
 * @file HRI_NVIC.c
 * @brief Declaración del periférico NVIC (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_NVIC.h>

volatile NVIC_per_t * const NVIC = (NVIC_per_t *) NVIC_BASE; //!< Periferico NVIC
