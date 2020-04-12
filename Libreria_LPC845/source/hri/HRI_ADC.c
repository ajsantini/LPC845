/**
 * @file HRI_ADC.c
 * @brief Declaración del periférico ADC (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_ADC.h>

volatile ADC_per_t * const ADC = (ADC_per_t *) ADC_BASE; //!< Periferico ADC
