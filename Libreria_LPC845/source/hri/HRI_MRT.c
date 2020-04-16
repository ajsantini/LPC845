/**
 * @file HRI_MRT.c
 * @brief Declaración del periférico MRT (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_MRT.h>

volatile MRT_per_t * const MRT = (MRT_per_t *) MRT_BASE; //!< Periferico MRT
