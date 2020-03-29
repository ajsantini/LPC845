/**
 * @file HAL_PININT.h
 * @brief Declaraciones a nivel de aplicacion del periferico PININT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HAL_PININT_H_
#define HAL_PININT_H_

#include <stdint.h>
#include <HAL_GPIO.h>

typedef enum
{
	HAL_PININT_CHANNEL_0 = 0,
	HAL_PININT_CHANNEL_1,
	HAL_PININT_CHANNEL_2,
	HAL_PININT_CHANNEL_3,
	HAL_PININT_CHANNEL_4,
	HAL_PININT_CHANNEL_5,
	HAL_PININT_CHANNEL_6,
	HAL_PININT_CHANNEL_7
}hal_pinint_channel_en;

typedef enum
{
	HAL_PININT_INTERRUPT_MODE_EDGE = 0,
	HAL_PININT_INTERRUPT_MODE_LEVEL
}hal_pinint_interrupt_mode_en;

typedef enum
{
	HAL_PININT_LEVEL_INT_HIGH = 0,
	HAL_PININT_LEVEL_INT_LOW
}hal_pinint_level_int_en;

typedef struct
{
	hal_pinint_channel_en channel;
	hal_pinint_interrupt_mode_en mode;
	hal_pinint_level_int_en int_on_level;
	uint8_t int_on_rising_edge;
	uint8_t int_on_falling_edge;
	hal_gpio_portpin_en portpin;
	void (*callback)(void);
}hal_pinint_config_t;

/**
 * @brief Inicializacion del modulo
 */
void hal_pinint_init(void);

/**
 * @brief Configurar interrupciones de pin
 * @param[in] config Configuracion de interrupciones de pin
 */
void hal_pinint_configure_pin_interrupt(const hal_pinint_config_t *config);

/**
 * @brief Registrar callback a llamar en interrupcion de PININTn
 * @param[in] channel Canal al cual registrar el callback
 * @param[in] new_callback Puntero a funcion a ejecutar
 */
void hal_pinint_register_callback(hal_pinint_channel_en channel, void (*new_callback)(void));

#endif /* HAL_PININT_H_ */
