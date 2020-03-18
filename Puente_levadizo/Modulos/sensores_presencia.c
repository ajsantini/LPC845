/*
 * sensores_presencia.c
 *
 *  Created on: Mar 16, 2020
 *      Author: augusto
 */

#include <sensores_presencia.h>
#include <HPL_GPIO.h>
#include <HPL_IOCON.h>

#define		SENSORES_PRESENCIA_AMOUNT		2

#define		SENSORES_PRESENCIA_BOUNCES_MS	20

static const uint8_t sensores_presencia_ports[SENSORES_PRESENCIA_AMOUNT] = //!< Puertos de los sensores de presencia
{
	0, 0
};

static const uint8_t sensores_presencia_pins[SENSORES_PRESENCIA_AMOUNT] = //!< Pines de los sensores de presencia
{
	0, 1
};

static uint8_t sensores_presencia_states[SENSORES_PRESENCIA_AMOUNT] = //!< Estados actuales de los sensores de presencia
{
	0, 0
};

/*!
 * @brief Inicializacion de los pines de los sensores de tope
 */
void sensores_presencia_init_pins(void)
{
	IOCON_config_t pin_config =
	{
		.mode = IOCON_PULL_NONE,
		.hysteresis = 0,
		.invert_input = 1,
		.open_drain = 0,
		.sample_mode = IOCON_SAMPLE_MODE_BYPASS,
		.clk_sel = IOCON_CLK_DIV_0,
		.dac_mode = 0,
	};

	uint8_t i;

	for(i = 0; i < SENSORES_PRESENCIA_AMOUNT; i++)
	{
		GPIO_set_dir(sensores_presencia_ports[i], sensores_presencia_pins[i], GPIO_DIR_INPUT, 0);
	}
}

/*!
 * @brief Obtener el estado de alguno de los sensores
 * @param sensor Seleccion del sensor deseado
 * @return En caso de haber presencia devuelve 1, caso contrario devuelve 0
 */
uint8_t sensores_presencia_get_state(sensor_presencia_selection_en sensor)
{
	return sensores_presencia_states[sensor];
}

/*!
 * @brief Revision del estado de los sensores. Llamar cada 1mseg
 */
void sensores_presencia_check(void)
{
	static uint8_t sensores_presencia_bounces[SENSORES_PRESENCIA_AMOUNT] = //!< Rebotes de los sensores de presencia
	{
		0, 0
	};

	uint8_t i;

	for(i = 0; i < SENSORES_PRESENCIA_AMOUNT; i++)
	{
		if(GPIO_read_pin(sensores_presencia_ports[i], sensores_presencia_pins[i]))
		{
			if(sensores_presencia_bounces[i] < SENSORES_PRESENCIA_BOUNCES_MS)
			{
				sensores_presencia_bounces[i]++;

				if(sensores_presencia_bounces[i] >= SENSORES_PRESENCIA_BOUNCES_MS)
				{
					sensores_presencia_states[i] = 1;
				}
			}
		}
		else
		{
			if(sensores_presencia_bounces[i] > 0)
			{
				sensores_presencia_bounces[i]--;

				if(sensores_presencia_bounces[i] = 0)
				{
					sensores_presencia_states[i] = 0;
				}
			}
		}
	}
}
