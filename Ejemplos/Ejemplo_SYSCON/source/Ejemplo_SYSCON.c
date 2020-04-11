/**
 * @file Ejemplo_SYSCON.c
 * @brief Ejemplo de utilización del \e SYSCON con la librería (capa de aplicación)
 *
 * El programa configura el FRO para funcionar sin divisor y con la frecuencia configurada por default, obteniendo
 * una frecuencia de funcionamiento del mismo de 24MHz. Además, selecciona el FRO como clock principal de sistema.
 *
 * Luego configura la salida CLKOUT para salir en el Puerto 0 ; Pin 18 mediante un divisor por 24, obteniendo una
 * frecuencia de salida en el pin Puerto 0 ; Pin 18 de 1MHz.
 *
 * La única forma de medir dicha salida es mediante un osciloscopio.
 *
 * @author Augusto Santini
 * @date 4/2020
 */

#include <cr_section_macros.h>
#include <HAL_SYSCON.h>
#include <HAL_GPIO.h>

/** Puerto/pin de salida para el CLKOUT */
#define		CLKOUT_PORTPIN		(HAL_GPIO_PORTPIN_0_18)

/** Divisor para el CLKOUT */
#define		CLKOUT_DIVISOR		(24)

/**
 * @brief Punto de entrada del programa
 * @return Nunca deberia terminar esta función
 */
int main(void)
{
	// Configuración del FRO para que esté sin divisor y que sea el clock principal
	hal_syscon_config_fro_direct(1, 1);

	// Clock principal en un pin (utilizando un divisor)
	hal_syscon_config_clkout(CLKOUT_PORTPIN, HAL_SYSCON_CLKOUT_SOURCE_SEL_MAIN_CLOCK, CLKOUT_DIVISOR);

	while(1)
	{

	}

	return 0;
}
