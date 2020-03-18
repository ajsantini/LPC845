/*
 * sensores_tope.c
 *
 *  Created on: Mar 14, 2020
 *      Author: augusto
 */

#include <sensores_tope.h>
#include <HPL_GPIO.h>
#include <HPL_IOCON.h>

#define		SENSORES_AMOUNT			2
#define		SENSORES_ZONAS			2

#define		SENSORES_BOUNCES_MS		100

static const uint8_t sensores_tope_ports[SENSORES_AMOUNT][SENSORES_ZONAS] = //!< Puertos de los sensores
{
	{ 0, 0 },
	{ 0, 0 }
};

static const uint8_t sensores_tope_pins[SENSORES_AMOUNT][SENSORES_ZONAS] = //!< Pines de los sensores
{
	{ 16, 17 },
	{ 18, 19 }
};

static uint8_t sensores_tope_states[SENSORES_AMOUNT][SENSORES_ZONAS] = //!< Estado actual de los sensores
{
	{ 0, 0 },
	{ 0, 0 }
};

/*!
 * @brief Inicializacion de los pines de los sensores de tope
 */
void sensores_tope_init_pins(void)
{
	IOCON_config_t pin_config =
	{
		.mode = IOCON_PULL_UP,
		.hysteresis = 0,
		.invert_input = 1,
		.open_drain = 0,
		.sample_mode = SAMPLE_MODE_BYPASS,
		.clk_sel = IOCON_CLK_DIV_0,
		.dac_mode = 0,
	};

	uint8_t i, j;

	IOCON_init();

	for(i = 0; i < SENSORES_AMOUNT; i++)
	{
		for(j = 0; j < SENSORES_ZONAS; j++)
		{
			GPIO_init(sensores_tope_ports[i][j]);

			IOCON_config_io(sensores_tope_ports[i][j], sensores_tope_pins[i][j], &pin_config);
			GPIO_set_dir(sensores_tope_ports[i][j], sensores_tope_pins[i][j], GPIO_DIR_INPUT, 0);
		}
	}
}

/*!
 * @brief Obtener el estado de alguno de los sensores
 * @param sensor Seleccion del sensor deseado
 * @param zona Seleccion de sensor abierto o cerrado
 * @return En caso de estar accionado devuelve 1, caso contrario devuelve 0
 */
uint8_t sensores_tope_get_state(sensor_tope_selection_en sensor, sensor_tope_zona_en zona)
{
	return sensores_tope_states[sensor][zona];
}

/*!
 * @brief Revision del estado de los sensores. Llamar cada 1mseg
 */
void sensores_tope_check(void)
{
	static uint8_t sensores_tope_bounces[SENSORES_AMOUNT][SENSORES_ZONAS] = //!< Rebotes actuales de los sensores
	{
		{ 0, 0 },
		{ 0, 0 }
	};

	uint8_t i, j;

	for(i = 0; i < SENSORES_AMOUNT; i++)
	{
		for(j = 0; j < SENSORES_ZONAS; j++)
		{
			if(GPIO_read_pin(sensores_tope_ports[i][j], sensores_tope_pins[i][j]))
			{
				if(sensores_tope_bounces[i][j] < SENSORES_BOUNCES_MS)
				{
					sensores_tope_bounces[i][j]++;

					if(sensores_tope_bounces[i][j] >= SENSORES_BOUNCES_MS)
					{
						sensores_tope_states[i][j] = 1;
					}
				}
			}
			else
			{
				if(sensores_tope_bounces[i][j] > 0)
				{
					sensores_tope_bounces[i][j]--;

					if(sensores_tope_bounces[i][j] == 0)
					{
						sensores_tope_states[i][j] = 0;
					}
				}
			}
		}
	}
}
