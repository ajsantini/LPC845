/**
 * @file Ejemplo_PININT.c
 * @brief Ejemplo de utilización del \e PININT con la librería (capa de aplicación)
 *
 * # Configuraciones
 *
 * El programa utiliza el clock por default con el que comienza el microcontrolador, es decir, el <em>Free Running
 * Oscillator</em> funcionando a 12MHz
 *
 * El periférico \e PININT será configurado de la siguiente manera:
 * 		- Canal 0 en el pulsador de usuario (Puerto 0 ; Pin 4)
 * 		- Detecciones por nivel (inicialmente nivel bajo, ver descripción de programa)
 * 		.
 *
 * Se configura el pin correspondiente al LED RGB \e Rojo como salida.
 *
 * # Descripción del programa
 *
 * Cuando el usuario accione el pulsador de usuario, se detectará dicho evento mediante el periférico @ref PININT,
 * ejecutando el callback correspondiente. En el mismo, se inhabilitarán las detecciones por <em>nivel bajo</em>
 * en dicho puerto/pin, se encenderá el LED RGB \e Rojo y se disparará el @ref WKT con un tiempo de
 * aproximadamente dos segundos. Una vez transcurrido el tiempo, se volverán a habilitar las detecciones por
 * <em>nivel bajo</em>, y se apagará el LED RBG \e Rojo.
 *
 * Si el usuario mantiene presionado el pulsador de usuario, verá el LED RGB \e Rojo siempre encendido, dado que
 * al volver a habilitar las detecciones, inmediatamente disparará el callback de detección.
 *
 * @author Augusto Santini
 * @date 4/2020
 */

#include <cr_section_macros.h>
#include <stddef.h>
#include <HAL_PININT.h>
#include <HAL_GPIO.h>
#include <HAL_IOCON.h>
#include <HAL_WKT.h>

/** Tiempo de inhibición del pulsador una vez detectado un cambio */
#define		KEY_INHIBIT_USEG			(2e6)

/** Puerto y pin de LED ROJO */
#define 	LED_ROJO					(HAL_GPIO_PORTPIN_1_2)

/** Macro para encender el LED ROJO */
#define		LED_ROJO_ON()				(hal_gpio_clear_pin(LED_ROJO))

/** Macro para apagar el LED ROJO */
#define		LED_ROJO_OFF()				(hal_gpio_set_pin(LED_ROJO))

/** Puerto/pin del switch */
#define		KEY_PORTPINT				(HAL_GPIO_PORTPIN_0_4)

/** Canal del \e PININT para el pulsador de usuario */
#define		KEY_PININT_CHANNEL			(HAL_PININT_CHANNEL_0)

static void pinint_callback(void);

static void wkt_callback(void);

int main(void)
{
	// Inicialización de GPIO
	hal_gpio_init(HAL_GPIO_PORT_0);
	hal_gpio_init(HAL_GPIO_PORT_1);

	hal_gpio_set_dir(LED_ROJO, HAL_GPIO_DIR_OUTPUT, 0);
	LED_ROJO_OFF();

	// Configuración de PININT
	hal_pinint_init();
	hal_pinint_channel_config(KEY_PININT_CHANNEL, KEY_PORTPINT, pinint_callback);
	hal_pinint_level_detections_config(KEY_PININT_CHANNEL, HAL_PININT_LEVEL_DETECTIONS_LOW);

	// Inicialización del WKT
	hal_wkt_init(HAL_WKT_CLOCK_SOURCE_FRO_DIV, 0, wkt_callback);

	while(1)
	{

	}

	return 0;
}

/**
 * @brief Callback a ejecutar cuando se detecte el evento configurado
 */
static void pinint_callback(void)
{
	LED_ROJO_ON();

	// Inhibición de interrupción de PININT
	hal_pinint_level_detections_config(KEY_PININT_CHANNEL, HAL_PININT_LEVEL_DETECTIONS_NONE);

	hal_wkt_start_count(KEY_INHIBIT_USEG);
}

/**
 * @brief Callback a ejecutar cuando termina el conteo del \e WKT
 */
static void wkt_callback(void)
{
	LED_ROJO_OFF();

	// Re-activación de interrupción de PININT
	hal_pinint_level_detections_config(KEY_PININT_CHANNEL, HAL_PININT_LEVEL_DETECTIONS_LOW);
}
