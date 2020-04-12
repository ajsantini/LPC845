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
 * y se vuelve a apagar hasta un próximo conteo.
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
 * 		\f$10KHz \pm 40\%\f$. La ventaja de este oscilador es que sigue encendido inclusive en modos de ultra
 * 		bajo consumo, pudiendo despertar al microcontrolador de los mismos.
 * 		- <em>Fuente de clock externo en el pin WKTLCKIN: Ubicado en Puerto 0 ; Pin 28
 * 		.
 *
 * @note La fuente de clock es seleccionada mediante la función @ref hal_wkt_select_clock_source o en la
 * inicialización mediante la función @ref hal_wkt_init.
 *
 * @{
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
 * @param[in] time_useg Tiempo en microsegundos deseado (se redondeará al valor primer posible hacia arriba)
 */
void hal_wkt_start_count(uint32_t time_useg);

/**
 * @brief Iniciar el conteo con el WKT en base a un valor
 *
 * El usuario es responsable de colocar un valor que tenga sentido en base al clock utilizado.
 *
 * @param[in] value Valor deseado a poner en el conteo (útil para una actualización mas rapida)
 */
void hal_wkt_start_count_with_value(uint32_t value);

#endif /* HAL_WKT_H_ */

/**
 * @}
 */
