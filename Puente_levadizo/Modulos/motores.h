/*
 * motores.h
 *
 *  Created on: Mar 14, 2020
 *      Author: augusto
 */

#ifndef MOTORES_H_
#define MOTORES_H_

//! Seleccion de motores posibles
typedef enum
{
	MOTOR_1 = 0,
	MOTOR_2
}motor_selection_en;

//! Seleccion de sentido posibles
typedef enum
{
	MOTOR_ABRIR = 0,
	MOTOR_CERRAR,
	MOTOR_FRENAR,
	MOTOR_LIBERAR
}motor_accion_en;

/*!
 * @brief Inicializacion de los pines de los motores
 */
void motores_init_pins(void);

/*!
 * @brief Accionar algun motor
 * @param motor Cual de los motores accionar
 * @param accion Accion del motor a realizar
 */
void motores_action(motor_selection_en motor, motor_accion_en accion);

/*!
 * @brief Obtener el estado de algun motor
 * @param motor Cual de los motores consultar
 * @return Accion del motor actual
 */
motor_accion_en motores_get_state(motor_selection_en motor);

/*!
 * @brief Manejo de PWM en caso de estar habilitado de los motores. Llamar cada 1mseg
 */
void motores_check(void);

#endif /* MOTORES_H_ */
