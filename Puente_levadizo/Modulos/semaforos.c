/*
 * semaforos.c
 *
 *  Created on: Mar 14, 2020
 *      Author: augusto
 */

#include <semaforos.h>
#include <HPL_GPIO.h>

#define		SEMAFOROS_AMOUNT		2
#define		SEMAFOROS_PIN_AMOUNT	3

typedef enum
{
	SEMAFOROS_COLOR_ROJO = 0,
	SEMAFOROS_COLOR_AMARILLO,
	SEMAFOROS_COLOR_VERDE
}semaforos_color_en;

static const uint8_t semaforos_ports[SEMAFOROS_AMOUNT][SEMAFOROS_PIN_AMOUNT] = //!< Puertos de los semaforo
{
	{ 0, 0, 0 },
	{ 0, 0, 0 }
};

static const uint8_t semaforos_pins[SEMAFOROS_AMOUNT][SEMAFOROS_PIN_AMOUNT] = //!< Pines de los semaforo
{
	{ 15, 14, 13 },
	{ 11, 10, 9 }
};

/*!
 * @brief Inicializacion de los pines correspondientes al semaforo
 */
void semaforos_init(void)
{
	uint8_t i, j;

	GPIO_init(0);

	for(i = 0; i < SEMAFOROS_AMOUNT; i++)
	{
		for(j = 0; j < SEMAFOROS_PIN_AMOUNT; j++)
		{
			GPIO_set_dir(semaforos_ports[i][j], semaforos_pins[i][j], GPIO_DIR_OUTPUT, 0);
		}
	}
}

/*!
 * @brief Fijar colores del semaforo
 * @param semaforo Sobre que semaforo accionar
 * @param comando Colores a fijar en el semaforo
 */
void semaforos_set(semaforos_seleccion_en semaforo, semaforos_cmd_t comando)
{
	if(comando.verde)
	{
		GPIO_set_pin(semaforos_ports[semaforo][SEMAFOROS_COLOR_VERDE], semaforos_pins[semaforo][SEMAFOROS_COLOR_VERDE]);
	}
	else
	{
		GPIO_clear_pin(semaforos_ports[semaforo][SEMAFOROS_COLOR_VERDE], semaforos_pins[semaforo][SEMAFOROS_COLOR_VERDE]);
	}

	if(comando.amarillo)
	{
		GPIO_set_pin(semaforos_ports[semaforo][SEMAFOROS_COLOR_AMARILLO], semaforos_pins[semaforo][SEMAFOROS_COLOR_AMARILLO]);
	}
	else
	{
		GPIO_clear_pin(semaforos_ports[semaforo][SEMAFOROS_COLOR_AMARILLO], semaforos_pins[semaforo][SEMAFOROS_COLOR_AMARILLO]);
	}

	if(comando.rojo)
	{
		GPIO_set_pin(semaforos_ports[semaforo][SEMAFOROS_COLOR_ROJO], semaforos_pins[semaforo][SEMAFOROS_COLOR_ROJO]);
	}
	else
	{
		GPIO_clear_pin(semaforos_ports[semaforo][SEMAFOROS_COLOR_ROJO], semaforos_pins[semaforo][SEMAFOROS_COLOR_ROJO]);
	}
}
