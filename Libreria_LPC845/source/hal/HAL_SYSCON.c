/**
 * @file HAL_SYSCON.c
 * @brief Funciones a nivel de aplicacion para el SYSCON (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_SYSCON.h>
#include <HPL_SYSCON.h>

static uint32_t current_main_clock = 12e6; //!< Frecuencia actual del main clock
static uint32_t current_fro_clock = 12e6; //!< Frecuencia actual del FRO

/**
 * @brief Obtener la frecuencia actual del main clock
 * @return Frecuencia del main clock en Hz
 */
uint32_t hal_syscon_get_system_clock(void)
{
	return current_main_clock;
}

/**
 * @brief Obtener la frecuencia actual del FRO
 * @return Frecuencia del FRO en Hz
 */
uint32_t hal_syscon_get_fro_clock(void)
{
	return current_fro_clock;
}
