/**
 * @file Ejemplo_DAC.c
 * @brief Ejemplo de utilización del \e DAC con la librería (capa de aplicación)
 *
 * El programa utiliza el \e DAC para generar una señal tipo rampa en la salida del canal 0 (Puerto 0 Pin 17) el
 * cual está mapeado al pin número 2 del stick de desarrollo. Ver @ref acerca_del_stick para más información.
 *
 * Para poder tener una visualización del programa de ejemplo, sera necesario un osciloscopio para mayor presición,
 * aunque también se puede utilizar un LED con un resistor en serie.
 *
 * - En caso de utilizar un osciloscopio: Conectar la punta de medición al pin 2 del stick de desarrollo. Debería
 * observar una señal tipo rampa, con un período de aproximadamente un segundo y tensión pico aproximadamente 3.3V.
 * - En caso de utilizar un LED con un resistor en serie: Conectar un extremo del resistor al pin 2 del stick de
 * desarrollo, el otro extremo del resistor conectarlo con el ánodo del LED, y el cátodo del LED conectarlo a la
 * masa del stick de desarrollo. Debería ver como el LED enciende gradualmente hasta apagarse y volver a repetir
 * el ciclo.
 *
 * El periférico \e Systick se configura para generar interrupciones cada 1 milisegundo. En cada tick, se escribe en
 * el \e DAC un valor de una tabla, cuyo indice ira incrementando en cada tick. En la tabla se encuentran guardados
 * los valores de la señal tipo rampa, incializados al comienzo del programa en un lazo \e for.
 *
 * El periférico \e DAC es inicializado con un tiempo de asentamiento de 1 microsegundo.
 *
 * @author Augusto Santini
 * @date 4/2020
 */

#include <cr_section_macros.h>
#include <HAL_DAC.h>
#include <HAL_SYSTICK.h>

/** Tiempo de interrupción del \e Systick en \b microsegundos */
#define		TICK_TIME_USEG				(1000)

/** Canal de \e DAC a utilizar en el programa */
#define		CANAL_DAC					(HAL_DAC_0)

/** Cantidad de muestras definidas para la tabla de muestreo */
#define		CANTIDAD_MUESTRAS		(1000)

/** Máximo valor que se puede escribir en el \e DAC */
#define		VALOR_MAXIMO_DAC		((1 << 10) - 1)

static void tick_callback(void);

/** Tabla a inicializar al comienzo del programa */
static uint16_t tabla_rampa[CANTIDAD_MUESTRAS];

int main(void)
{
	uint32_t contador;

	// Definicion de los valores para la señal tipo rampa
	for(contador = 0; contador < CANTIDAD_MUESTRAS; contador++)
	{
		tabla_rampa[contador] = contador * (VALOR_MAXIMO_DAC / CANTIDAD_MUESTRAS);
	}

	// Inicializacion del \e DAC
	hal_dac_init(CANAL_DAC, HAL_DAC_SETTLING_TIME_1US_MAX, 0);

	// Inicialización del \e Systick con el tiempo de tick adecuado
	hal_systick_init(TICK_TIME_USEG, tick_callback);

	while(1)
	{

	}

	return 0;
}

/**
 * @brief Callback a ejecutar en cada tick del \e Systick
 */
static void tick_callback(void)
{
	static uint32_t contador = 0;

	hal_dac_update_value(CANAL_DAC, tabla_rampa[contador++]);
	contador %= CANTIDAD_MUESTRAS;
}
