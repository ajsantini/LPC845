/**
 * @file Ejemplo_WKT.c
 * @brief Ejemplo de utilización del \e WKT con la librería (capa de aplicación)
 *
 * # Configuraciones
 *
 * El programa utiliza la configuración del clock por default, es decir, FRO como fuente de clock principal,
 * y el mismo con una frecuencia de 12MHz.
 *
 * El periférico \e WKT es configurado para utilizar como fuente de clock el oscilador de bajo consumo, o el
 * FRO dividido, dependiendo de un define ubicado en el código. De esta forma, se puede probar la presición de
 * ambas fuentes de clock, y se puede observar la gran incertidumbre en el clock de bajo consumo.
 *
 * # Funcionamiento del programa
 *
 * En el callback de interrupción del \e WKT configurado, se genera una inversión en el estado del pin del LED
 * RGB \e verde y se vuelve a disparar el conteo. Esto se realiza cada aproximadamente 1 milisegundo.
 *
 * Nótese que la carga del valor de conteo es realizada mediante la función @ref hal_wkt_start_count_with_value
 * y no con @ref hal_wkt_start_count. Esto se debe a que el tiempo que queremos disparar es conocido, y es un
 * tiempo relativamente corto para las frecuencias de clock que se utilizan. Para más información, referirise a
 * @ref WKT.
 *
 * @author Augusto Santini
 * @date 4/2020
 */

#include <cr_section_macros.h>
#include <HAL_WKT.h>
#include <HAL_GPIO.h>

/** Valor de tick para el \e WKT en microsegundos */
#define		WKT_TICK_USEG				(1000)

/** Puerto/pin del LED RGB \e verde */
#define		LED_RGB_GREEN_PORTPIN		(HAL_GPIO_PORTPIN_1_0)

/** Si este define queda sin comentar, se utilizará el oscilador de bajo consumo como fuente de clock del \e WKT */
#define		OSC_LOW_POWER

static void wkt_callback(void);

#ifdef		OSC_LOW_POWER
static const uint32_t wkt_reload_val = 10;
#else
static const uint32_t wkt_reload_val = 750;
#endif

int main(void)
{
	// Inicialización de GPIO
	hal_gpio_init(HAL_GPIO_PORT_1);
	hal_gpio_set_dir(LED_RGB_GREEN_PORTPIN, HAL_GPIO_DIR_OUTPUT, 0);

	// Inicialización del WKT
#ifdef		OSC_LOW_POWER
	hal_wkt_init(HAL_WKT_CLOCK_SOURCE_LOW_POWER_OSC, 0, wkt_callback);
#else
	hal_wkt_init(HAL_WKT_CLOCK_SOURCE_FRO_DIV, 0, wkt_callback);
#endif

	hal_wkt_start_count_with_value(wkt_reload_val);

	while(1)
	{

	}

	return 0;
}

/**
 * @brief Callback a ejecutar en interrupción del WKT
 */
static void wkt_callback(void)
{
	// Toggle del LED RGB verde
	hal_gpio_toggle_pin(LED_RGB_GREEN_PORTPIN);

	// Re-disparo del WKT
	hal_wkt_start_count_with_value(wkt_reload_val);
}
