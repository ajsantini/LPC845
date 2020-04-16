/**
 * @file HAL_WKT.h
 * @brief Declaraciones a nivel de aplicacion del periferico WKT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @defgroup WKT Wake Up Timer (WKT)
 *
 * # Introducción
 *
 * Este periférico es un contador básico con posibilidad de tres fuentes distintas de clock. El mismo puede
 * ser utilizado para despertar al microcontrolador de modos de bajo consumo siempre y cuando la fuente de
 * clock sea correctamente seleccionada. También se puede utilizar como un timer genérico.
 *
 * Al cargar un valor distinto de cero en la cuenta del periférico, el mismo se enciende automáticamente y
 * comienza el conteo. Una vez transcurrido el conteo, el periférico genera la interrupción correspondiente
 * y se vuelve a apagar hasta un próximo conteo, es decir, es un timer de tipo <em>one-shot</em>.
 *
 * @note El periférico se inicializa mediante la función @ref hal_wkt_init y se dispara un conteo mediante las
 * funciones @ref hal_wkt_start_count o @ref hal_wkt_start_count_with_value dependiendo de la necesidad del
 * usuario.
 *
 * # Fuentes de clock
 *
 * Se puede excitar al periférico con tres fuentes de clock distintas:
 * 		- <em>FRO dividido</em>: FRO dividido por 16
 * 		- <em>Oscilador de bajo consumo</em>: Oscilador siempre encendido con una frecuencia de
 * 		10KHz más/menos 40%. La ventaja de este oscilador es que sigue encendido inclusive en modos de ultra
 * 		bajo consumo, pudiendo despertar al microcontrolador de los mismos.
 * 		- <em>Fuente de clock externo en el pin WKTLCKIN: Ubicado en Puerto 0 ; Pin 28
 * 		.
 *
 * @note La fuente de clock es seleccionada mediante la función @ref hal_wkt_select_clock_source o en la
 * inicialización mediante la función @ref hal_wkt_init.
 *
 * # Nota acerca de la carga del conteo
 *
 * En la librería se implementan dos funciones para generar la carga del contador del periférico, las mismas son
 * @ref hal_wkt_start_count y @ref hal_wkt_start_count_with_value. La diferencia entre una y otra, es que en la
 * primera, la librería pide como argumento un tiempo en microsegundos y hace la cuenta necesaria para aproximarse
 * lo más posible y cargar el conteo adecuado, mientras que la segunda directamente pide el valor a cargar al
 * usuario. Cabe destacar que la primer función, utiliza números con punto flotante, por lo que su ejecución tarda
 * un tiempo considerable, dado que el microcontrolador no tiene unidad de punto flotante de hardware incorporada.
 * El criterio para utilizar una función u otra es:
 * 		- Si los tiempos de conteo son lo suficientemente largos, y/o la presición no es un factor demasiado
 * 		importante (por ejemplo, para salir de un modo de bajo consumo tal vez no sea necesaria una
 * 		presición demasiado alta) se puede utilizar directametne la función @ref hal_wkt_start_count.
 * 		- Si el tiempo de conteo es bajo, o la presición es un factor vital en el conteo (por ejemplo, se utiliza
 * 		el periférico para un delay de corto tiempo preciso) es recomendable utilizar la función
 * 		@ref hal_wkt_start_count_with_value.
 * 		.
 *
 * @{
 */

/**
 * @example Ejemplo_WKT.c
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
 */

#ifndef HAL_WKT_H_
#define HAL_WKT_H_

#include <stdint.h>

/** Selección de fuente de clock para el \e WKT */
typedef enum
{
	HAL_WKT_CLOCK_SOURCE_FRO_DIV = 0, /**< Fuente de clock FRO dividido */
	HAL_WKT_CLOCK_SOURCE_LOW_POWER_OSC, /**< Fuente de clock oscilador de bajo consumo */
	HAL_WKT_CLOCK_SOURCE_EXTERNAL /**< Fuente de clock externa */
}hal_wkt_clock_source_en;

/**
 * @brief Tipo de dato para el callback de interrupción del \e WKT
 * @note Es importante recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que
 * el usuario deberá tener en cuenta todas las consideraciones necesarias a la hora de escribir el mismo.
 */
typedef void (*hal_wkt_callback_t)(void);

/**
 * @brief Inicializar el WKT
 * @param[in] clock_sel Selección de clock deseada para el WKT
 * @param[in] ext_clock_value Valor de clock externo (si la selección es interna, no importa este parámetro)
 * @param[in] callback Callback a ejecutar en la interrupción del WKT
 * @note Es importante recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que
 * el usuario deberá tener en cuenta todas las consideraciones necesarias a la hora de escribir el mismo.
 */
void hal_wkt_init(hal_wkt_clock_source_en clock_sel, uint32_t ext_clock_value, hal_wkt_callback_t callback);

/**
 * @brief Configurar fuente de clock para el WKT
 * @param[in] clock_sel Selección de clock deseada para el WKT
 * @param[in] ext_clock_value Valor de clock externo (si la selección es interna, no importa este parámetro)
 */
void hal_wkt_select_clock_source(hal_wkt_clock_source_en clock_sel, uint32_t ext_clock_value);

/**
 * @brief Registrar un callback para la interrupción del WKT
 * @param[in] new_callback Nuevo callback para la interrupción del WKT
 * @note Es importante recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que
 * el usuario deberá tener en cuenta todas las consideraciones necesarias a la hora de escribir el mismo.
 */
void hal_wkt_register_callback(hal_wkt_callback_t new_callback);

/**
 * @brief Iniciar el conteo con el WKT en base a un tiempo
 *
 * @note Esta función utiliza variables con punto flotante por lo que tarda un tiempo considerable en ejecutarse.
 *
 * @param[in] time_useg Tiempo en microsegundos deseado (se redondeará al valor primer posible hacia arriba)
 */
void hal_wkt_start_count(uint32_t time_useg);

/**
 * @brief Iniciar el conteo con el WKT en base a un valor
 *
 * @note El usuario es responsable de colocar un valor que tenga sentido en base al clock utilizado.
 *
 * @param[in] value Valor deseado a poner en el conteo (útil para una actualización mas rapida)
 */
void hal_wkt_start_count_with_value(uint32_t value);

#endif /* HAL_WKT_H_ */

/**
 * @}
 */
