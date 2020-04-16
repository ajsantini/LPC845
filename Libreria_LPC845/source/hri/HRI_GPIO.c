/**
 * @file HRI_GPIO.c
 * @brief Declaración del periférico GPIO (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <stdint.h>
#include <HRI_GPIO.h>

volatile GPIO_per_t * const GPIO = (GPIO_per_t *) GPIO_BASE; //!< Periferico GPIO
