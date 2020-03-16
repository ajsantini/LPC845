/*
 * semaforos.h
 *
 *  Created on: Mar 14, 2020
 *      Author: augusto
 */

#ifndef SEMAFOROS_H_
#define SEMAFOROS_H_

#include <stdint.h>

typedef enum
{
	SEMAFOROS_SELECCION_1 = 0,
	SEMAFOROS_SELECCION_2
}semaforos_seleccion_en;

//! Estructura para un comando del semaforo
typedef struct
{
	uint8_t verde : 1;
	uint8_t amarillo : 1;
	uint8_t rojo : 1;
}semaforos_cmd_t;

/*!
 * @brief Inicializacion de los pines correspondientes al semaforo
 */
void semaforos_init(void);

/*!
 * @brief Fijar colores del semaforo
 * @param semaforo Sobre que semaforo accionar
 * @param comando Colores a fijar en el semaforo
 */
void semaforos_set(semaforos_seleccion_en semaforo, semaforos_cmd_t comando);

#endif /* SEMAFOROS_H_ */
