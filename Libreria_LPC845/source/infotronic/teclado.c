/**
 * @file teclado.c
 * @brief Funciones del periferico teclado (2x3)
 * @author Augusto Santini
 * @date 12/2019
 * @version 1.0
 */

#include <stdint.h>
#include <teclado.h>
#include <HPL_GPIO.h>
#include <HPL_IOCON.h>
#include <HPL_SYSTICK.h>

#define		TECLADO_CANTIDAD_FILAS			2
#define		TECLADO_CANTIDAD_COLUMNAS		3

static const uint8_t teclado_filas_port[TECLADO_CANTIDAD_FILAS] =
{
	0,
	0
};

static const uint8_t teclado_filas_pin[TECLADO_CANTIDAD_FILAS] =
{
	27,
	29
};

static const uint8_t teclado_columnas_port[TECLADO_CANTIDAD_COLUMNAS] =
{
	0,
	0,
	0
};

static const uint8_t teclado_columnas_pin[TECLADO_CANTIDAD_COLUMNAS] =
{
	8,
	31,
	30
};

static uint8_t key_buffer = NO_KEY;

static uint32_t max_bounces;

static uint8_t get_current_key(void);

/**
 * @brief Inicializacion del teclado
 * @param[in] bounces Tiempo en milisegundos para tomar una tecla como valida/invalida
 */
void teclado_init(uint32_t bounces)
{
	uint8_t counter;
	IOCON_config_t pin_config;

	pin_config.clk_sel = IOCON_CLK_DIV_0;
	pin_config.dac_mode = 0;
	pin_config.hysteresis = 0;
	pin_config.iic_mode = 0;
	pin_config.invert_input = 0;
	pin_config.mode = PULL_DOWN;
	pin_config.open_drain = 0;
	pin_config.sample_mode = SAMPLE_MODE_BYPASS;

	// Filas como entradas con pull-down
	for(counter = 0; counter < TECLADO_CANTIDAD_FILAS; counter++)
	{
		GPIO_set_dir(teclado_filas_port[counter], teclado_filas_pin[counter], GPIO_DIR_INPUT, 0);

		IOCON_config_io(teclado_filas_port[counter], teclado_filas_pin[counter], &pin_config);
	}

	// Columnas como salidas
	for(counter = 0; counter < TECLADO_CANTIDAD_COLUMNAS; counter++)
	{
		GPIO_set_dir(teclado_columnas_port[counter], teclado_columnas_pin[counter], GPIO_DIR_OUTPUT, 0);
	}

	// Rebotes para tomar una tecla como valida/invalida
	max_bounces = bounces;
}

/**
 * @brief Obtener tecla pulsada
 * @return Tecla pulsada (0 ~ 5 o NO_KEY), una vez leida, se limpia el buffer hasta que se presione la proxima tecla
 */
uint8_t get_key(void)
{
	uint8_t ret = key_buffer;

	key_buffer = NO_KEY;

	return ret;
}
/**
 * @brief Callback para revisar el teclado periodicamente
 */
void teclado_check(void)
{
	static uint8_t last_key = NO_KEY;
	static uint32_t current_bounces = 0;

	uint8_t this_key = get_current_key();

	if(this_key != last_key)
	{
		current_bounces = 0;
		last_key = this_key;
	}
	else
	{
		if(current_bounces < max_bounces)
		{
			current_bounces++;

			if(current_bounces >= max_bounces)
			{
				key_buffer = this_key;
			}
		}
	}
}

/**
 * @brief Funcion para obtener la tecla que actualmente esta siendo presionada
 * @return Tecla actualmente presionada (0 ~ 5 o NO_KEY)
 */
static uint8_t get_current_key(void)
{
	uint8_t counter_filas;
	uint8_t counter_columnas;

	for(counter_columnas = 0; counter_columnas < TECLADO_CANTIDAD_COLUMNAS; counter_columnas++)
	{
		GPIO_set_pin(teclado_columnas_port[counter_columnas], teclado_columnas_pin[counter_columnas]);
		GPIO_set_pin(teclado_columnas_port[counter_columnas], teclado_columnas_pin[counter_columnas]);
		GPIO_set_pin(teclado_columnas_port[counter_columnas], teclado_columnas_pin[counter_columnas]);

		for(counter_filas = 0; counter_filas < TECLADO_CANTIDAD_FILAS; counter_filas++)
		{
			if(GPIO_read_pin(teclado_filas_port[counter_filas], teclado_filas_pin[counter_filas]) == 1)
			{
				GPIO_clear_pin(teclado_columnas_port[counter_columnas], teclado_columnas_pin[counter_columnas]);
				GPIO_clear_pin(teclado_columnas_port[counter_columnas], teclado_columnas_pin[counter_columnas]);
				GPIO_clear_pin(teclado_columnas_port[counter_columnas], teclado_columnas_pin[counter_columnas]);

				return ((counter_columnas * TECLADO_CANTIDAD_FILAS) + counter_filas);
			}
		}

		GPIO_clear_pin(teclado_columnas_port[counter_columnas], teclado_columnas_pin[counter_columnas]);
		GPIO_clear_pin(teclado_columnas_port[counter_columnas], teclado_columnas_pin[counter_columnas]);
		GPIO_clear_pin(teclado_columnas_port[counter_columnas], teclado_columnas_pin[counter_columnas]);
	}

	return NO_KEY;
}