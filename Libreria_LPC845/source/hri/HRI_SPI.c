/**
 * @file HRI_SPI.c
 * @brief Declaración del periférico SPI (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_SPI.h>

volatile SPI_per_t * const SPI[] = { //!< Perifericos SPI
		(SPI_per_t *) SPI0_BASE,
		(SPI_per_t *) SPI1_BASE
};
