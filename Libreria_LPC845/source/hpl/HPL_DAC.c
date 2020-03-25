/**
 * @file HPL_DAC.c
 * @brief Funciones a nivel de abstraccion de periferico para el DAC (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdint.h>
#include <HRI_DAC.h>

volatile DAC_per_t * const DAC[] = { //!< Perifericos DAC
		(DAC_per_t *) DAC0_BASE,
		(DAC_per_t *) DAC1_BASE
};
