/*
 * motores.c
 *
 *  Created on: Mar 14, 2020
 *      Author: augusto
 */

#include <motores.h>
#include <HPL_GPIO.h>

#define		MOTORES_USE_PWM

#define		MOTORES_AMOUNT			2
#define		MOTORES_PIN_AMOUNT		2

#define		MOTORES_OPEN_IDX		0
#define		MOTORES_CLOSE_IDX		1

#define		MOTORES_PWM_MAX			20
#define		MOTORES_PWM_OPEN		3
#define		MOTORES_PWM_CLOSE		2

static const uint8_t motores_ports[MOTORES_AMOUNT][MOTORES_PIN_AMOUNT] = //!< Puertos de los pines de los motores
{
	{ 0, 0 },
	{ 0, 0 }
};

static const uint8_t motores_pins[MOTORES_AMOUNT][MOTORES_PIN_AMOUNT] = //!< Pines de los pines de los motores
{
	{ 29, 28 },
	{ 27, 26 }
};

static motor_accion_en motores_actions[MOTORES_AMOUNT] = //!< Acciones actuales de los motores
{
	MOTOR_LIBERAR,
	MOTOR_LIBERAR
};

#ifdef		MOTORES_USE_PWM
static uint32_t motores_pwm_value[MOTORES_AMOUNT] = //!< Valores actuales de PWM de los motores
{
	0, 0
};
#endif

static void motor_open(motor_selection_en motor);
static void motor_close(motor_selection_en motor);
static void motor_break(motor_selection_en motor);
static void motor_release(motor_selection_en motor);

/*!
 * @brief Inicializacion de los pines de los motores
 */
void motores_init_pins(void)
{
	uint8_t i, j;

	GPIO_init(0);

	for(i = 0; i < MOTORES_AMOUNT; i++)
	{
		for(j = 0; j < MOTORES_PIN_AMOUNT; j++)
		{
			GPIO_set_dir(motores_ports[i][j], motores_pins[i][j], GPIO_DIR_OUTPUT, 0);
		}
	}
}

/*!
 * @brief Accionar algun motor
 * @param motor Cual de los motores accionar
 * @param accion Accion del motor a realizar
 */
void motores_action(motor_selection_en motor, motor_accion_en accion)
{
	switch(accion)
	{
	case MOTOR_ABRIR:
	{
		motor_open(motor);
		motores_actions[motor] = MOTOR_ABRIR;

		break;
	}

	case MOTOR_CERRAR:
	{
		motor_close(motor);
		motores_actions[motor] = MOTOR_CERRAR;

		break;
	}

	case MOTOR_FRENAR:
	{
		motor_break(motor);
		motores_actions[motor] = MOTOR_FRENAR;

		break;
	}

	case MOTOR_LIBERAR:
	{
		motor_release(motor);
		motores_actions[motor] = MOTOR_LIBERAR;

		break;
	}
	}
}

/*!
 * @brief Obtener el estado de algun motor
 * @param motor Cual de los motores consultar
 * @return Accion del motor actual
 */
motor_accion_en motores_get_state(motor_selection_en motor)
{
	return motores_actions[motor];
}

/*!
 * @brief Manejo de PWM en caso de estar habilitado de los motores. Llamar cada 1mseg
 */
void motores_check(void)
{
#ifdef		MOTORES_USE_PWM
	static uint32_t motores_step_counter;
	uint8_t counter;

	motores_step_counter = (motores_step_counter + 1) % MOTORES_PWM_MAX;

	for(counter = 0; counter < MOTORES_AMOUNT; counter++)
	{
		switch(motores_actions[counter])
		{
		case MOTOR_ABRIR:
		{
			if(motores_step_counter == 0)
			{
				if(motores_pwm_value[counter] > 0)
				{
					GPIO_set_pin(motores_ports[counter][MOTORES_OPEN_IDX], motores_pins[counter][MOTORES_OPEN_IDX]);
				}
			}
			else
			{
				if(motores_pwm_value[counter] == motores_step_counter)
				{
					GPIO_clear_pin(motores_ports[counter][MOTORES_OPEN_IDX], motores_pins[counter][MOTORES_OPEN_IDX]);
				}
			}

			break;
		}

		case MOTOR_CERRAR:
		{
			if(motores_step_counter == 0)
			{
				if(motores_pwm_value[counter] > 0)
				{
					GPIO_set_pin(motores_ports[counter][MOTORES_CLOSE_IDX], motores_pins[counter][MOTORES_CLOSE_IDX]);
				}
			}
			else
			{
				if(motores_pwm_value[counter] == motores_step_counter)
				{
					GPIO_clear_pin(motores_ports[counter][MOTORES_CLOSE_IDX], motores_pins[counter][MOTORES_CLOSE_IDX]);
				}
			}

			break;
		}

		case MOTOR_LIBERAR: break;
		case MOTOR_FRENAR: break;
		}
	}
#endif
}

/*!
 * @brief Abrir un motor
 * @param motor Motor a abrir
 */
static void motor_open(motor_selection_en motor)
{
#ifndef		MOTORES_USE_PWM
	GPIO_clear_pin(motores_ports[motor][MOTORES_CLOSE_IDX], motores_pins[motor][MOTORES_CLOSE_IDX]);
	GPIO_set_pin(motores_ports[motor][MOTORES_OPEN_IDX], motores_pins[motor][MOTORES_OPEN_IDX]);
#else
	motores_pwm_value[motor] = MOTORES_PWM_OPEN;
	GPIO_clear_pin(motores_ports[motor][MOTORES_OPEN_IDX], motores_pins[motor][MOTORES_OPEN_IDX]);
	GPIO_clear_pin(motores_ports[motor][MOTORES_CLOSE_IDX], motores_pins[motor][MOTORES_CLOSE_IDX]);
	// En el proximo ciclo de PWM se normalizan los estados
#endif
}

/*!
 * @brief Cerrar un motor
 * @param motor Motor a cerrar
 */
static void motor_close(motor_selection_en motor)
{
#ifndef 	MOTORES_USE_PWM
	GPIO_clear_pin(motores_ports[motor][MOTORES_OPEN_IDX], motores_pins[motor][MOTORES_OPEN_IDX]);
	GPIO_set_pin(motores_ports[motor][MOTORES_CLOSE_IDX], motores_pins[motor][MOTORES_CLOSE_IDX]);
#else
	motores_pwm_value[motor] = MOTORES_PWM_CLOSE;
	GPIO_clear_pin(motores_ports[motor][MOTORES_OPEN_IDX], motores_pins[motor][MOTORES_OPEN_IDX]);
	GPIO_clear_pin(motores_ports[motor][MOTORES_CLOSE_IDX], motores_pins[motor][MOTORES_CLOSE_IDX]);
	// En el proximo ciclo de PWM se normalizan los estados
#endif
}

/*!
 * @brief Frenar un motor
 * @param motor Motor a frenar
 */
static void motor_break(motor_selection_en motor)
{
#ifndef		MOTORES_USE_PWM
	GPIO_set_pin(motores_ports[motor][MOTORES_OPEN_IDX], motores_pins[motor][MOTORES_OPEN_IDX]);
	GPIO_set_pin(motores_ports[motor][MOTORES_CLOSE_IDX], motores_pins[motor][MOTORES_CLOSE_IDX]);
#else
	motores_pwm_value[motor] = 0;
	GPIO_set_pin(motores_ports[motor][MOTORES_OPEN_IDX], motores_pins[motor][MOTORES_OPEN_IDX]);
	GPIO_set_pin(motores_ports[motor][MOTORES_CLOSE_IDX], motores_pins[motor][MOTORES_CLOSE_IDX]);
#endif
}

/*!
 * @brief Liberar un motor
 * @param motor Motor a liberar
 */
static void motor_release(motor_selection_en motor)
{
#ifndef		MOTORES_USE_PWM
	GPIO_clear_pin(motores_ports[motor][MOTORES_OPEN_IDX], motores_pins[motor][MOTORES_OPEN_IDX]);
	GPIO_clear_pin(motores_ports[motor][MOTORES_CLOSE_IDX], motores_pins[motor][MOTORES_CLOSE_IDX]);
#else
	motores_pwm_value[motor] = 0;
	GPIO_clear_pin(motores_ports[motor][MOTORES_OPEN_IDX], motores_pins[motor][MOTORES_OPEN_IDX]);
	GPIO_clear_pin(motores_ports[motor][MOTORES_CLOSE_IDX], motores_pins[motor][MOTORES_CLOSE_IDX]);
#endif
}
