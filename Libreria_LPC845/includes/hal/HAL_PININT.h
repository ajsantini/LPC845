/**
 * @file HAL_PININT.h
 * @brief Declaraciones a nivel de aplicacion del periferico PININT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @defgroup PININT Interrupciones de pin / Motor de patrones (PININT)
 *
 * # Introducción
 *
 * El periférico \e PININT puede funcionar tanto como <em>Interrupciones de pin</em> como <em>Motor de
 * detección de patrones</em>.
 *
 * # Funcionamiento como <em>Interrupciones de pin</em>
 *
 * El periférico dispone de 8 canales configurables para detección de distintos eventos externos al
 * microcontrolador. Cada uno de estos canales es configurable para detectar cambios de nivel o de flanco
 * en el pin asociado, pudiendo así también detectar flancos ascendentes/descendentes como nivel alto/bajo. Cada
 * canal tiene su propia configuración independiente de los demás.
 *
 * # Funcionamiento como <em>Motor de detección de patrones</em>
 *
 * @warning Falta desarrollar!
 *
 * # Campos de aplicación típicos
 *
 * - Detección de eventos externos que necesiten prioridad
 * - Utilización como interfaz para disparar otros periféricos, como el @ref ADC, mediante señales externas
 * - Detección de patrones en múltiples pines externos
 * .
 *
 * @{
 */

/**
 * @example Ejemplo_PININT.c
 *
 *
 */

#ifndef HAL_PININT_H_
#define HAL_PININT_H_

#include <stdint.h>
#include <HAL_GPIO.h>

/** Seleccion de canal de \e PININT */
typedef enum
{
	HAL_PININT_CHANNEL_0 = 0, /**< Canal 0 */
	HAL_PININT_CHANNEL_1, /**< Canal 1 */
	HAL_PININT_CHANNEL_2, /**< Canal 2 */
	HAL_PININT_CHANNEL_3, /**< Canal 3 */
	HAL_PININT_CHANNEL_4, /**< Canal 4 */
	HAL_PININT_CHANNEL_5, /**< Canal 5 */
	HAL_PININT_CHANNEL_6, /**< Canal 6 */
	HAL_PININT_CHANNEL_7 /**< Canal 7 */
}hal_pinint_channel_en;

/** Selección de modo de detección de \e PININT */
typedef enum
{
	HAL_PININT_INTERRUPT_MODE_EDGE = 0, /**< Detección por flanco */
	HAL_PININT_INTERRUPT_MODE_LEVEL /**< Detección por nivel */
}hal_pinint_interrupt_mode_en;

/** Selección de polaridad para detecciones por nivel */
typedef enum
{
	HAL_PININT_LEVEL_INT_HIGH = 0, /**< Polaridad positiva */
	HAL_PININT_LEVEL_INT_LOW /**< Polaridad negativa */
}hal_pinint_level_int_en;

/**
 * @brief Tipo de dato para callback de \e PININT
 *
 * @note Cabe recordar que estos callbacks se ejecutan bajo el contexto de una interrupción, por lo que el
 * mismo deberá tener todas las consideraciones necesarias
 */
typedef void (*hal_pinint_callback_t)(void);

/** Estructura de configuración para un canal de \e PININT */
typedef struct
{
	hal_pinint_channel_en channel; /**< Canal asociado */
	hal_pinint_interrupt_mode_en mode; /**< Modo de detección */
	hal_pinint_level_int_en int_on_level; /**< Selección de polaridad (aplica para detección por nivel) */
	uint8_t int_on_rising_edge; /**< Habilitar detección en flanco ascendente (aplica para detección por flanco) */
	uint8_t int_on_falling_edge; /**< Habilitar detección en flanco descendente (aplica para detección por flanco) */
	hal_gpio_portpin_en portpin; /**< Puerto/pin asociado */
	hal_pinint_callback_t callback; /**< Callback a ejecutar en la detección */
}hal_pinint_config_t;

/**
 * @brief Inicialización del periférico
 */
void hal_pinint_init(void);

/**
 * @brief De-Inicialización del periférico
 */
void hal_pinint_deinit(void);

/**
 * @brief Configurar interrupciones de pin
 * @param[in] config Configuración de interrupciones de pin
 * @see hal_pinint_config_t
 */
void hal_pinint_pin_interrupt_config(const hal_pinint_config_t *config);

/**
 * @brief Registrar callback a llamar en interrupción de PININTn
 *
 * Si el callback pasado en la configuración es \e NULL, no se habilitará la interrupción correspondiente.
 *
 * @param[in] channel Canal al cual registrar el callback
 * @param[in] new_callback Puntero a función a ejecutar
 *
 * @note Cabe recordar que estos callbacks se ejecutan bajo el contexto de una interrupción, por lo que el
 * mismo deberá tener todas las consideraciones necesarias
 *
 * @see hal_pinint_channel_en
 * @see hal_pinint_callback_t
 */
void hal_pinint_register_callback(hal_pinint_channel_en channel, hal_pinint_callback_t new_callback);

#endif /* HAL_PININT_H_ */

/**
 * @}
 */
