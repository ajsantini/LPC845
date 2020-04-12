/**
 * @file HRI_ACMP.c
 * @brief Funciones a nivel de abstraccion de periferico para el ADC (LPC845)
 * @author Esteban E. Chiama
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_ACMP.h>

volatile ACMP_per_t * const ACMP = (ACMP_per_t *) ACMP_BASE; //!< Periferico ANALOG COMPARATOR
