/**
 * @file HPL_SYSTICK.c
 * @brief Funciones a nivel de abstraccion de periferico para el SYSTICK (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdio.h>
#include <stdint.h>
#include <HPL_SYSTICK.h>
#include <HPL_SYSCON.h>
#include <HRI_SYSTICK.h>

volatile SYSTICK_reg_t * const SYSTICK = (SYSTICK_reg_t *) SYSTICK_BASE; //!< Periferico SYSTICK

static void (*systick_callback)(void) = NULL; //!< Callback a llamar en la interrupcion

/**
 * @brief Inicializar SYSTICK
 * @param[in] tick_us Tiempo de cada tick en microsegundos (usegs)
 * @param[in] systick_hook Puntero a funcion a ejecutar en la interrupcion de SYSTICK
 */
void SYSTICK_init(uint32_t tick_us, void (*systick_hook)(void))
{
	uint32_t aux;

	// En base a los us deseados calculo el valor de STRELOAD
	aux = SYSCON_get_system_clock() / 10;
	aux *= tick_us;
	aux /= 100000; // La cuenta hecha asi aumenta la presicion sin generar nunca un overflow

	aux--;

	if(aux >= (1 << 24))
	{
		// En este caso habria overflow. Lo fijo al maximo.
		aux = (1 << 24) - 1;
	}

	*((uint32_t *)(&SYSTICK->RVR)) = 0; // Error de implementacion rarisimo. Hay que dejarlo.

	SYSTICK->RVR.RELOAD = aux;

	// Fijo el clock del systick como el clock de la CPU directamente
	SYSTICK->CSR.CLKSOURCE = 1;

	SYSTICK->CVR.CURRENT = 0x00; // Limpio current value

	// Habilito clock del systick
	SYSTICK->CSR.ENABLE = 1;

	// Registro callback
	SYSTICK_update_callback(systick_hook);

	// Habilito interrupcion de systick
	SYSTICK->CSR.TICKINT = 1;
}

/**
 * @brief Interrupcion de SYSTICK
 */
void SysTick_Handler(void)
{
	// Hook para el usuario
	if(systick_callback != NULL)
		systick_callback();
}

/**
 * @brief Registrar un callback para llamar en la interrupcion de SYSTICK
 * @param[in] new_callback Puntero a funcion a ser ejecutado en la interrupcion de SYSTICK
 */
void SYSTICK_update_callback(void (*new_callback)(void))
{
	systick_callback = new_callback;
}
