/**
 * @file HPL_ADC.c
 * @brief Funciones a nivel de abstraccion de periferico para el ADC (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <HRI_ADC.h>

volatile ADC_per_t * const ADC = (ADC_per_t *) ADC_BASE; //!< Periferico ADC
