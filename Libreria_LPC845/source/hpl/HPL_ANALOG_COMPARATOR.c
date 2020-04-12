/**
 * @file HPL_ANALOG_COMPARATOR.c
 * @brief Funciones a nivel de abstraccion de periferico para el ADC (LPC845)
 * @author Esteban E. Chiama
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_ANALOG_COMPARATOR.h>

volatile ANALOG_COMPARATOR_per_t * const ANALOG_COMPARATOR =
		(ANALOG_COMPARATOR_per_t *) ANALOG_COMPARATOR_BASE; //!< Periferico ANALOG COMPARATOR
