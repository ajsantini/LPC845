/**
 * @file HAL_ACMP.h
 * @brief Declaraciones a nivel de aplicación del periférico ADC (LPC845)
 * @author Esteban E. Chiama
 * @date 4/2020
 * @version 1.0
 */

/**
 * @defgroup ACMP Comparador analógico (ACMP)
 *
 * # Introducción
 *
 * Este periférico compara 2 señales analógicas y responde con una salida digital que indica cuál de ellas es la mayor. Es posible
 * conectar la señal de salida del comparador a un pin digital de salida.
 *
 * El comparador tiene 2 entradas, una considerada positiva y otra negativa.
 * - Si la señal conectada a la entrada positiva es mayor, la salida del comparador estará en estado alto.
 * - Si la señal conectada a la entrada negativa es mayor, la salida del comparador estará en estado bajo.
 *
 * Este periférico cuenta además con la posibilidad de generar un pedido de interrupción
 * según el tipo de flanco generado por un cambio a la salida del comparador.
 *
 * # Selección de las entradas analógicas al comparador
 *
 * Para este microcontrolador ambas entradas del comparador son seleccionables entre entradas analógicas externas
 * y algunas señales internas del microcontrolador.
 *
 * Las entradas seleccionables son:
 * - Tensión de salida de la <em>Voltage Ladder</em>.
 * - 5 entradas analógicas externas.
 * - Tensión de referencia interna <em>Bandgap</em>.
 * - Salida del conversor digital a analógico <em>DAC</em>.
 *
 * # Voltage Ladder
 *
 * La <em>Voltage Ladder</em> puede ser utilizada para generar una tensión determinada a partir de una tensión externa,
 *  o de la propia alimentación del microcontrolador V_{DD}.
 *
 * La tensión de salida es configurable por el usuario y responde a la siguiente ecuación
 * (en función de la tensión de referencia elegida):
 *
 * \f{eqnarray*}{
 * 		V_{LAD} = \frac{n * V_{ref}}{31}
 * \f}
 *
 * Donde:
 * - n: Entero entre 0 y 31.
 * - V_{ref}: Tensión de referencia de la <em>Voltage Ladder</em>.
 *
 * # Salida del comparador como trigger de hardware de otros periféricos
 *
 * Es posible utilizar la salida del comparador como trigger por hardware para otros periféricos
 * para que estos lleven a cabo alguna funcionalidad sin la necesidad de intervención por software del microcontrolador .
 *
 * De hecho, este es uno de los triggers por hardware de disparo de conversiones del periférico <em>ADC</em>.
 *
 * # Campos de aplicación típicos
 *
 * Loren Ipsum
 *
 * @{
 */

#ifndef HAL_ACMP_H_
#define HAL_ACMP_H_

#include <HAL_GPIO.h>

/** Selección de sincronismo de la señal digital de salida del comparador analógico.*/
typedef enum
{
	HAL_ACMP_OUTPUT_DIRECT = 0, /**< Los cambios se reflejarán instantáneamente.*/
	HAL_ACMP_OUTPUT_SYNC /**< Los cambios serán sincronizados con el clock de bus.*/
}hal_acmp_output_control_en;

/** Posible umbral de histéresis para cambios a la salida del comparador analógico.*/
typedef enum
{
	HAL_ACMP_HYSTERESIS_NONE = 0, /**< Sin histéresis.*/
	HAL_ACMP_HYSTERESIS_5mV, /**< Umbral de 5mV.*/
	HAL_ACMP_HYSTERESIS_10mV, /**< Umbral de 10mV. */
	HAL_ACMP_HYSTERESIS_20mV /**< Umbral de 20mV. */
}hal_acmp_hysteresis_sel_en;

/** Posibles tensiones de referencia para la Voltage Ladder del comparador analógico.*/
typedef enum
{
	HAL_ACMP_LADDER_VREF_VDD = 0, /**< Tensión de referencia VDD.*/
	HAL_ACMP_LADDER_VREF_VDDCMP_PIN /**< Tensión de referencia tomada del pin externo con función analógica VDDCMP.*/
}hal_acmp_ladder_vref_sel_en;

/** Selección de tipo de flanco de la señal de salida
 * del comparador analógico que será trigger de interrupción. */
typedef enum
{
	HAL_ACMP_EDGE_FALLING = 0, /**< Flanco descendente.*/
	HAL_ACMP_EDGE_RISING, /**< Flanco ascendente.*/
	HAL_ACMP_EDGE_BOTH /**< Ambos tipos de flanco.*/
}hal_acmp_edge_sel_en;

/** Posibles entradas al comparador analógico, válidas tanto para la positiva como la negativa.*/
typedef enum
{
	HAL_ACMP_INPUT_VOLTAGE_VLADDER_OUT = 0, /**< Tensión interna de salida de la Voltage Ladder.*/
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I1, /**< Pin externo con función analógica ACMP_I1.*/
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I2, /**< Pin externo con función analógica ACMP_I2.*/
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I3, /**< Pin externo con función analógica ACMP_I3.*/
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I4, /**< Pin externo con función analógica ACMP_I4.*/
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I5, /**< Pin externo con función analógica ACMP_I5.*/
	HAL_ACMP_INPUT_VOLTAGE_BANDGAP, /**< Tensión interna de referencia 'Bandgap'.*/
	HAL_ACMP_INPUT_VOLTAGE_DACOUT0 /**< Señal interna, salida 0 del periférico DAC.*/
}hal_acmp_input_voltage_sel_en;

/** Estructura de configuración de características generales del comparador analógico.*/
typedef struct
{
	hal_acmp_output_control_en output_control; /**< Control de sincronismo de la señal de salida del comparador.*/
	hal_acmp_hysteresis_sel_en hysteresis; /**< Configura umbral de histérisis para reflejar cambios en la salida del comparador.*/
	uint8_t interrupt_enable; /**< Habilita/deshabilita interrupciones generadas por la salida del comparador.*/
	hal_acmp_edge_sel_en edge_sel; /**< Configura el tipo de flanco de la señal de salida del comparador que genera un pedido de interrupción.*/
}hal_acpm_config_t;

/** Estructura de configuración de la Voltage Ladder del comparador analógico.*/
typedef struct
{
	uint8_t enable; /**< Habilitación/deshabilitación de la Voltage Ladder.*/
	hal_acmp_ladder_vref_sel_en vref_sel; /**< Selección de tensión de referencia de la Voltage Ladder.*/
	uint8_t step; /**< Configura qué fracción de la tensión de referencia estará será la tensión de salida de la Voltage Ladder.*/
}hal_acmp_ladder_config_t;

/**
 * @brief Inicialización del periférico Comparador Analógico.
 * @see hal_acmp_deinit
 * @see hal_acmp_config
 */
void hal_acmp_init(void);

/**
 * @brief De-inicialización del periferico Comparador Analógico.
 *
 * Además de la usual deinicialización de un periférico,
 * libera todos los pines posiblemente utilizados por el comparador de su función analógica.
 */
void hal_acmp_deinit(void);

/**
 * @brief Configuración de parámetros generales del comparador analógico.
 *
 * Esta función configura las siguientes características del comparador:
 *
 * - Sincronización de la señal de salida del comparador con clock de bus.
 * - Rango de histérisis sobre la comparación para validar un cambio de la salida del comparador.
 * - Habilitación o deshabilitación de interrupciones, y según qué tipo de flanco se quiera
 * de la salida del comparador.
 *
 * @param[in] acmp_config Puntero a estructura con parámetros a configurar.
 * @see hal_acpm_config_t
 * @see hal_acmp_ladder_config
 * @see hal_acmp_input_select
 */
void hal_acmp_config(const hal_acpm_config_t *acmp_config);

/**
 * @brief Configuración de la Voltage Ladder del comparador analógico.
 *
 * Configura las siguientes características de la Voltage Ladder del comparador:
 *
 * - Habilitación o no.
 * - Tensión de referencia.
 * - Fracción ('step') utilizada de dicha tensión de referencia.
 *
 * @param[in] config Puntero a estructura con parámetros de configuración deseados de la Voltage Ladder.
 * @see hal_acmp_ladder_config_t
 * @see hal_acmp_config
 * @see hal_acmp_ladder_config
 * @see hal_acmp_input_select
 * @see hal_acmp_deinit
 */
void hal_acmp_ladder_config(const hal_acmp_ladder_config_t *config);

/**
 * @brief Selecciona las entradas positiva y negativa deseadas para el comparador.
 *
 * Para entradas analógicas además realiza la configuración necesaria del pin externo.
 *
 * @param[in] positive_input Selección de entrada para la entrada positiva del comparador analógico.
 * @param[in] negative_input Selección de entrada para la entrada negativa del comparador analógico.
 * @see hal_acmp_input_voltage_sel_en
 * @see hal_acmp_ladder_config
 * @see hal_acmp_deinit
 */
void hal_acmp_input_select(hal_acmp_input_voltage_sel_en positive_input, hal_acmp_input_voltage_sel_en negative_input);

/** @brief Asigna la salida del comparador a un pin externo del microcontrolador.
 *
 *	@note: No realiza ningún tipo de configuración del pin respecto a sus resistores.
 *
 *  @param[in] port_pin Indica puerto y pin deseado.
 *  @see hal_acmp_output_pin_clear
 */
void hal_acmp_output_pin_set(hal_gpio_portpin_en port_pin);

/**
 * @brief Deshace la asignación de la salida del comparador a un pin externo del microcontrolador.
 * @see hal_acmp_output_pin_set
 */
void hal_acmp_output_pin_clear();

#endif /* HAL_ACMP_H_ */

/**
 * @}
 */
