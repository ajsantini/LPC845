/*!
 *
 */

#include <HPL_SYSCON.h>
#include <HPL_SYSTICK.h>
#include <motores.h>
#include <sensores_tope.h>
#include <semaforos.h>
#include <sensores_presencia.h>

#define		TICK_US				1000
#define		WAIT_TIME_SECS		5

#define		MS_TO_SECS(x)		(x * 1000)

typedef enum
{
	STATES_RESET = 0,
	STATES_WAITING_SENSOR_PRESENCIA,
	STATES_WAITING_OPENING,
	STATES_WAITING_PASS,
	STATES_WAITING_CLOSING
}states_en;

static void tick_callback(void);

int main(void)
{
	SYSCON_set_fro_direct(); // FRO sin divisor previo (FRO = 24MHz)
	SYSCON_set_system_clock_source(SYSTEM_CLOCK_SEL_FRO);

	motores_init_pins();
	sensores_tope_init_pins();
	semaforos_init();

	semaforos_set(SEMAFOROS_SELECCION_1, (semaforos_cmd_t) {.verde = 0, .amarillo = 1, .rojo = 0});

	SYSTICK_init(TICK_US, tick_callback);

	motores_action(MOTOR_1, MOTOR_ABRIR);

	while(1)
	{
		static states_en state = STATES_OPENING;

		switch(state)
		{
		case STATES_RESET:
		{
			if(sensores_tope_get_state(SENSOR_TOPE_1, SENSOR_TOPE_PUENTE_ABIERTO))
			{
				motores_action(MOTOR_1, MOTOR_FRENAR);
				semaforos_set(SEMAFOROS_SELECCION_1, (semaforos_cmd_t) {.verde = 0, .amarillo = 0, .rojo = 1});


			}

			state = STATES_WAITING_OPEN;

			break;
		}

		case STATES_WAITING_OPEN:
		{
			if(waiting_counter == 0)
			{
				motores_action(MOTOR_1, MOTOR_CERRAR);
				semaforos_set(SEMAFOROS_SELECCION_1, (semaforos_cmd_t) {.verde = 0, .amarillo = 1, .rojo = 0});

				state = STATES_CLOSING;
			}

			break;
		}

		case STATES_CLOSING:
		{
			if(sensores_tope_get_state(SENSOR_TOPE_1, SENSOR_TOPE_PUENTE_CERRADO))
			{
				motores_action(MOTOR_1, MOTOR_FRENAR);
				semaforos_set(SEMAFOROS_SELECCION_1, (semaforos_cmd_t) {.verde = 1, .amarillo = 0, .rojo = 0});

				state = STATES_WAITING_CLOSE;
			}

			break;
		}

		case STATES_WAITING_CLOSE:
		{
			if(waiting_counter == 0)
			{
				motores_action(MOTOR_1, MOTOR_ABRIR);
				semaforos_set(SEMAFOROS_SELECCION_1, (semaforos_cmd_t) {.verde = 0, .amarillo = 1, .rojo = 0});

				state = STATES_OPENING;
			}

			break;
		}
		}
	}

	return 0;
}

/*!
 * @brief Callback para el systick
 */
static void tick_callback(void)
{
	sensores_tope_check();
	motores_check();
	sensores_presencia_check();
}
