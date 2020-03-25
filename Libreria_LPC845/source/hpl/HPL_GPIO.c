/**
 * @file HPL_GPIO.c
 * @brief Funciones a nivel de abstraccion de periferico para el GPIO (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdint.h>
#include <HRI_GPIO.h>

volatile GPIO_per_t * const GPIO = (GPIO_per_t *) GPIO_BASE; //!< Periferico GPIO
