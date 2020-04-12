/**
 * @file HRI_DAC.c
 * @brief Declaración del periférico DAC (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <stdint.h>
#include <HRI_DAC.h>

volatile DAC_per_t * const DAC[] = { //!< Perifericos DAC
		(DAC_per_t *) DAC0_BASE,
		(DAC_per_t *) DAC1_BASE
};
