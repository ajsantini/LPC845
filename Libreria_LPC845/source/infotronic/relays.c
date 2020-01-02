/**
 * @file relays.c
 * @brief Funciones para el manejo de los reles
 * @author Augusto Santini
 * @date 12/2019
 * @version 1.0
 */

#include <stdint.h>
#include <relays.h>
#include <HPL_GPIO.h>

#define		CANTIDAD_RELAYS		4

static const uint8_t relays_port[CANTIDAD_RELAYS] =
{
	1,
	1,
	1,
	0
};

static const uint8_t relays_pin[CANTIDAD_RELAYS] =
{
	0,
	1,
	2,
	1
};

static uint8_t relays_state = 0;

/**
 * @brief Inicializacion de los reles
 */
void relays_init(void)
{
	uint8_t counter;

	for(counter = 0; counter < CANTIDAD_RELAYS; counter++)
	{
		GPIO_set_dir(relays_port[counter], relays_pin[counter], GPIO_DIR_OUTPUT, 0);
	}
}

/**
 * @brief Cerrar un rele
 * @param[in] numero_rele Rele a cerrar (0 ~ 3)
 */
void relays_close(uint8_t numero_rele)
{
	relays_state |= (1 << numero_rele);
}

/**
 * @brief Abrir un rele
 * @param[in] numero_rele Rele a abrir (0 ~ 3)
 */
void relays_open(uint8_t numero_rele)
{
	relays_state &= ~(1 << numero_rele);
}

/**
 * @brief Invertir estado de un rele
 * @param[in] numero_rele Rele a invertir (0 ~ 3)
 */
void relays_toggle(uint8_t numero_rele)
{
	relays_state ^= (1 << numero_rele);
}

/**
 * @brief Callback para actualizar los reles periodicamente
 */
void relays_check(void)
{
	static uint8_t last_state = 0;

	if(last_state != relays_state)
	{
		uint8_t counter;

		for(counter = 0; counter < CANTIDAD_RELAYS; counter++)
		{
			if(relays_state & (1 << counter))
			{
				GPIO_set_pin(relays_port[counter], relays_pin[counter]);
			}
			else
			{
				GPIO_clear_pin(relays_port[counter], relays_pin[counter]);
			}
		}

		last_state = relays_state;
	}
}
