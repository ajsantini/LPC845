/*
 * sensores_presencia.h
 *
 *  Created on: Mar 16, 2020
 *      Author: augusto
 */

#ifndef SENSORES_PRESENCIA_H_
#define SENSORES_PRESENCIA_H_

#include <stdint.h>

//! Seleccion de sensores posibles
typedef enum
{
	SENSOR_PRESENCIA_1 = 0,
	SENSOR_PRESENCIA_2
}sensor_presencia_selection_en;

/*!
 * @brief Inicializacion de los pines de los sensores de tope
 */
void sensores_presencia_init_pins(void);

/*!
 * @brief Obtener el estado de alguno de los sensores
 * @param sensor Seleccion del sensor deseado
 * @return En caso de haber presencia devuelve 1, caso contrario devuelve 0
 */
uint8_t sensores_presencia_get_state(sensor_presencia_selection_en sensor);

/*!
 * @brief Revision del estado de los sensores. Llamar cada 1mseg
 */
void sensores_presencia_check(void);

#endif /* SENSORES_PRESENCIA_H_ */
