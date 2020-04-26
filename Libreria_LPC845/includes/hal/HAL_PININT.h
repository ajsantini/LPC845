/**
 * @file HAL_PININT.h
 * @brief Declaraciones a nivel de aplicacion del periferico PININT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @defgroup PININT Interrupciones de pin/Motor de patrones (PININT)
 *
 * # Introducción
 *
 * El periférico *PININT* puede funcionar tanto como *Interrupciones de pin* como *Motor de
 * detección de patrones*.
 *
 * # Funcionamiento como *Interrupciones de pin*
 *
 * El periférico dispone de 8 canales configurables para detección de distintos eventos externos al
 * microcontrolador. Cada uno de estos canales es configurable para detectar cambios de nivel o de flanco
 * en el pin asociado, pudiendo así también detectar flancos ascendentes/descendentes como nivel alto/bajo. Cada
 * canal tiene su propia configuración independiente de los demás.
 *
 * # Funcionamiento como *Motor de detección de patrones*
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
 * # Configuraciones
 *
 * El programa utiliza el clock por default con el que comienza el microcontrolador, es decir, el *Free Running
 * Oscillator* funcionando a 12MHz
 *
 * El periférico *PININT* será configurado de la siguiente manera:
 * 		- Canal 0 en el pulsador de usuario (Puerto 0 ; Pin 4)
 * 		- Detecciones por nivel (inicialmente nivel bajo, ver descripción de programa)
 * 		.
 *
 * Se configura el pin correspondiente al LED RGB *Rojo* como salida.
 */

#ifndef HAL_PININT_H_
#define HAL_PININT_H_

#include <stdint.h>
#include <HAL_GPIO.h>

/** Seleccion de canal de *PININT* */
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

/** Selección de flancos a detectar */
typedef enum
{
	HAL_PININT_EDGE_DETECTIONS_NONE = 0, /**< Inhabiltar detecciones por flanco */
	HAL_PININT_EDGE_DETECTIONS_RISING, /**< Detección de flancos ascendentes */
	HAL_PININT_EDGE_DETECTIONS_FALLING, /**< Detección de flancos descendentes */
	HAL_PININT_EDGE_DETECTIONS_BOTH /**< Detección de ambos flancos */
}hal_pinint_edge_detections_en;

/** Selección de polaridad para detecciones por nivel */
typedef enum
{
	HAL_PININT_LEVEL_DETECTIONS_NONE = 0, /**< Inhabilitar detecciones por nivel */
	HAL_PININT_LEVEL_DETECTIONS_HIGH, /**< Polaridad positiva */
	HAL_PININT_LEVEL_DETECTIONS_LOW /**< Polaridad negativa */
}hal_pinint_level_detections_en;

/**
 * @brief Tipo de dato para callback de *PININT*
 *
 * @note Cabe recordar que estos callbacks se ejecutan bajo el contexto de una interrupción, por lo que el
 * mismo deberá tener todas las consideraciones necesarias
 */
typedef void (*hal_pinint_callback_t)(void);

/**
 * @brief Inicialización del periférico
 */
void hal_pinint_init(void);

/**
 * @brief De-Inicialización del periférico
 */
void hal_pinint_deinit(void);

/**
 * @brief Configuración de canal de *PININT*
 *
 * @note Esta función no configura el modo de detección. Ver: @ref hal_pinint_edge_detections_config y
 * @ref hal_pinint_level_detections_config
 *
 * @param[in] channel Canal a configurar
 * @param[in] portpin Puerto/pin en donde configurar el canal
 * @param[in] callback Callback a ejecutar en detección
 * @pre Haber inicializado el periférico
 */
void hal_pinint_channel_config(hal_pinint_channel_en channel, hal_gpio_portpin_en portpin, hal_pinint_callback_t callback);

/**
 * @brief Configurar detecciones por flanco
 * @param[in] channel Canal a configurar
 * @param[in] edge Flancos a detectar
 * @pre Haber inicializado el periférico
 */
void hal_pinint_edge_detections_config(hal_pinint_channel_en channel, hal_pinint_edge_detections_en edge);

/**
 * @brief Configurar detecciones por nivel
 * @param[in] channel Canal a configurar
 * @param[in] level Nivel a detectar
 * @pre Haber inicializado el periférico
 */
void hal_pinint_level_detections_config(hal_pinint_channel_en channel, hal_pinint_level_detections_en level);

#endif /* HAL_PININT_H_ */

/**
 * @}
 */
