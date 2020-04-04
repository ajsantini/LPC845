/**
 * @file HAL_WKT.h
 * @brief Declaraciones a nivel de aplicacion del periferico WKT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HAL_WKT_H_
#define HAL_WKT_H_

#include <stdint.h>

typedef enum
{
	HAL_WKT_CLOCK_SOURCE_FRO_DIV = 0,
	HAL_WKT_CLOCK_SOURCE_LOW_POWER_OSC,
	HAL_WKT_CLOCK_SOURCE_EXTERNAL
}hal_wkt_clock_source_en;

/**
 * @brief Inicializar el WKT
 * @param[in] clock_sel Seleccion de clock deseada para el WKT
 * @param[in] ext_clock_value Valor de clock externo (si la seleccion es interna, no importa este parametro)
 * @param[in] callback Callback a ejecutar en la interrupcion del WKT
 */
void hal_wkt_init(hal_wkt_clock_source_en clock_sel, uint32_t ext_clock_value, void (*callback)(void));

/*
 * @brief Configurar fuente de clock para el WKT
 * @param[in] clock_sel Seleccion de clock deseada para el WKT
 * @param[in] ext_clock_value Valor de clock externo (si la seleccion es interna, no importa este parametro)
 */
void hal_wkt_select_clock_source(hal_wkt_clock_source_en clock_sel, uint32_t ext_clock_value);

/**
 * @brief Registrar un callback para la interrupcion del WKT
 * @param[in] new_callback Nuevo callback para la interrupcion del WKT
 */
void hal_wkt_register_callback(void (*new_callback)(void));

/*
 * @brief Iniciar el conteo con el WKT en base a un tiempo
 * @param[in] time_useg Tiempo en microsegundos deseado (se redondeara al valor primer posible hacia arriba)
 */
void hal_wkt_start_count(uint32_t time_useg);

/*
 * @brief Iniciar el conteo con el WKT en base a un valor
 *
 * El usuario es responsable de colocar un valor que tenga sentido en base al clock utilizado.
 *
 * @param[in] value Valor deseado a poner en el conteo (util para una actualizacion mas rapida)
 */
void hal_wkt_start_count_with_value(uint32_t value);

#endif /* HAL_WKT_H_ */
