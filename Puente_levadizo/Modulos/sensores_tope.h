/*
 * sensores_tope.h
 *
 *  Created on: Mar 14, 2020
 *      Author: augusto
 */

#ifndef SENSORES_TOPE_H_
#define SENSORES_TOPE_H_

#include <stdint.h>

//! Seleccion de sensores posibles
typedef enum
{
	SENSOR_1 = 0,
	SENSOR_2
}sensor_number_selection_en;

typedef enum
{
	SENSOR_ABIERTO = 0,
	SENSOR_CERRADO
}sensor_zone_selection_en;

/*!
 * @brief Inicializacion de los pines de los sensores de tope
 */
void sensores_tope_init_pins(void);

/*!
 * @brief Obtener el estado de alguno de los sensores
 * @param sensor Seleccion del sensor deseado
 * @param zona Seleccion de sensor abierto o cerrado
 * @return En caso de estar accionado devuelve 1, caso contrario devuelve 0
 */
uint8_t sensores_tope_get_state(sensor_number_selection_en sensor, sensor_zone_selection_en zona);

/*!
 * @brief Revision del estado de los sensores. Llamar cada 1mseg
 */
void sensores_tope_check(void);

#endif /* SENSORES_TOPE_H_ */
