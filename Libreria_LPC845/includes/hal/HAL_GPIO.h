/**
 * @file HAL_GPIO.h
 * @brief Declaraciones a nivel de aplicacion del periferico GPIO (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

#include <stdint.h>

#define		HAL_GPIO_PORTPIN_TO_PORT(x)		(x / 32)
#define		HAL_GPIO_PORTPIN_TO_PIN(x)		(x % 32)

typedef enum
{
	HAL_GPIO_PORT_0 = 0,
	HAL_GPIO_PORT_1
}hal_gpio_port_en;

typedef enum
{
	HAL_GPIO_PORTPIN_0_0 = 0,
	HAL_GPIO_PORTPIN_0_1,
	HAL_GPIO_PORTPIN_0_2,
	HAL_GPIO_PORTPIN_0_3,
	HAL_GPIO_PORTPIN_0_4,
	HAL_GPIO_PORTPIN_0_5,
	HAL_GPIO_PORTPIN_0_6,
	HAL_GPIO_PORTPIN_0_7,
	HAL_GPIO_PORTPIN_0_8,
	HAL_GPIO_PORTPIN_0_9,
	HAL_GPIO_PORTPIN_0_10,
	HAL_GPIO_PORTPIN_0_11,
	HAL_GPIO_PORTPIN_0_12,
	HAL_GPIO_PORTPIN_0_13,
	HAL_GPIO_PORTPIN_0_14,
	HAL_GPIO_PORTPIN_0_15,
	HAL_GPIO_PORTPIN_0_16,
	HAL_GPIO_PORTPIN_0_17,
	HAL_GPIO_PORTPIN_0_18,
	HAL_GPIO_PORTPIN_0_19,
	HAL_GPIO_PORTPIN_0_20,
	HAL_GPIO_PORTPIN_0_21,
	HAL_GPIO_PORTPIN_0_22,
	HAL_GPIO_PORTPIN_0_23,
	HAL_GPIO_PORTPIN_0_24,
	HAL_GPIO_PORTPIN_0_25,
	HAL_GPIO_PORTPIN_0_26,
	HAL_GPIO_PORTPIN_0_27,
	HAL_GPIO_PORTPIN_0_28,
	HAL_GPIO_PORTPIN_0_29,
	HAL_GPIO_PORTPIN_0_30,
	HAL_GPIO_PORTPIN_0_31,
	HAL_GPIO_PORTPIN_1_0,
	HAL_GPIO_PORTPIN_1_1,
	HAL_GPIO_PORTPIN_1_2,
	HAL_GPIO_PORTPIN_1_3,
	HAL_GPIO_PORTPIN_1_4,
	HAL_GPIO_PORTPIN_1_5,
	HAL_GPIO_PORTPIN_1_6,
	HAL_GPIO_PORTPIN_1_7,
	HAL_GPIO_PORTPIN_1_8,
	HAL_GPIO_PORTPIN_1_9,
	HAL_GPIO_PORTPIN_1_10,
	HAL_GPIO_PORTPIN_1_11,
	HAL_GPIO_PORTPIN_1_12,
	HAL_GPIO_PORTPIN_1_13,
	HAL_GPIO_PORTPIN_1_14,
	HAL_GPIO_PORTPIN_1_15,
	HAL_GPIO_PORTPIN_1_16,
	HAL_GPIO_PORTPIN_1_17,
	HAL_GPIO_PORTPIN_1_18,
	HAL_GPIO_PORTPIN_1_19,
	HAL_GPIO_PORTPIN_1_20,
	HAL_GPIO_PORTPIN_1_21,
	HAL_GPIO_PORTPIN_NOT_USED
}hal_gpio_portpin_en;

typedef enum
{
	HAL_GPIO_DIR_INPUT = 0,
	HAL_GPIO_DIR_OUTPUT
}hal_gpio_dir_en;

/**
 * @brief Inicializar un puerto
 * @param[in] port Puerto a inicializar
 */
void hal_gpio_init(hal_gpio_port_en port);

/**
 * @brief Fijar direccion de una GPIO
 * @param[in] portpin Numero de puerto/pin a configurar
 * @param[in] dir Direccion deseada
 * @param[in] initial_state Estado inicial (aplica para salidas nada mas)
 */
void hal_gpio_set_dir(hal_gpio_portpin_en portpin, hal_gpio_dir_en dir, uint8_t initial_state);

/**
 * @brief Fijar estado activo de una GPIO
 * @param[in] portpin Numero de puerto/pin a accionar
 */
void hal_gpio_set_pin(hal_gpio_portpin_en portpin);

/**
 * @brief Fijar estado inactivo de una GPIO
 * @param[in] portpin Numero de puerto/pin a accionar
 */
void hal_gpio_clear_pin(hal_gpio_portpin_en portpin);

/**
 * @brief Invertir estado de una GPIO
 * @param[in] portpin Numero de puerto/pin a accionar
 */
void hal_gpio_toggle_pin(hal_gpio_portpin_en portpin);

/**
 * @brief Leer el estado de una GPIO
 * @param[in] portpin Numero de puerto/pin a accionar
 * @return Estado actual de la GPIO
 */
uint8_t hal_gpio_read_pin(hal_gpio_portpin_en portpin);

#endif /* HAL_GPIO_H_ */
