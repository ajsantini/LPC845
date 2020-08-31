/**
 * @file HAL_DAC.h
 * @brief Declaraciones a nivel de aplicacion del periferico DAC (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @defgroup DAC Conversor digital a analógico (DAC)
 *
 * # Introducción
 *
 * El *DAC*, como su nombre lo indica, convierte un valor digital en una tensión analógica. En el caso de este
 * microcontrolador, tiene dos módulos *DACs* independientes con una resolución de 10 bits cada uno. Se puede
 * calcular la resolución de cada paso de *DAC* como:
 *
 * \f{eqnarray*}{
 * 		DAC_{res} = \frac{V_{ref_{p}}}{2^N}
 * \f}
 *
 * Siendo N la cantidad de bits de resolución del *DAC*. Esto implica que podemos preveer el valor de salida
 * de tensión analógica conociendo el valor a convertir por el *DAC* mediante:
 *
 * \f{eqnarray*}{
 * 		DAC_{conv} = DAC_{res} * DAC_{val}
 * \f}
 *
 * @note Para actualizar el valor de salida del *DAC* se utiliza la función @ref hal_dac_update_value.
 *
 * # Velocidad de conversión
 *
 * El periférico permite dos velocidades de conversión:
 * 		- Frecuencia de conversión de 1Mhz: Implica un mayor consumo de energía
 * 		- Frecuencia de conversión de 400KHz: Implica un menor consumo de energía
 * 		.
 *
 * @note Existe una relación entre velocidad de conversión y consumo de energía del periférico. La velocidad de
 * conversión del periférico, se configura en la función @ref hal_dac_init.
 *
 * # Campos de aplicación típicos
 *
 * - Generación de señales de naturaleza analógica
 * - Generación de señales auxiliares para calibración
 * - Servomecanismos controlados por señales analógicas
 *
 * @{
 */

/**
 * @example Ejemplo_DAC.c
 *
 * El programa utiliza el *DAC* para generar una señal tipo rampa en la salida del canal 0 (Puerto 0 Pin 17) el
 * cual está mapeado al pin número 2 del stick de desarrollo. Ver @ref acerca_del_stick para más información.
 *
 * Para poder tener una visualización del programa de ejemplo, sera necesario un osciloscopio para mayor presición,
 * aunque también se puede utilizar un LED con un resistor en serie.
 *
 * - En caso de utilizar un osciloscopio: Conectar la punta de medición al pin 2 del stick de desarrollo. Debería
 * observar una señal tipo rampa, con un período de aproximadamente un segundo y tensión pico aproximadamente 3.3V.
 * - En caso de utilizar un LED con un resistor en serie: Conectar un extremo del resistor al pin 2 del stick de
 * desarrollo, el otro extremo del resistor conectarlo con el ánodo del LED, y el cátodo del LED conectarlo a la
 * masa del stick de desarrollo. Debería ver como el LED enciende gradualmente hasta apagarse y volver a repetir
 * el ciclo.
 *
 * El periférico *Systick* se configura para generar interrupciones cada 1 milisegundo. En cada tick, se escribe en
 * el *DAC* un valor de una tabla, cuyo indice ira incrementando en cada tick. En la tabla se encuentran guardados
 * los valores de la señal tipo rampa, incializados al comienzo del programa en un lazo "for".
 *
 * El periférico *DAC* es inicializado con un tiempo de asentamiento de 1 microsegundo.
 */

#ifndef HAL_DAC_H_
#define HAL_DAC_H_

#include <stdint.h>

#if defined (__cplusplus)
extern "C" {
#endif

/** Enumeraciones de instancias disponibles de *DAC* */
typedef enum
{
	HAL_DAC_SEL_0 = 0, /**< Instancia 0 */
	HAL_DAC_SEL_1, /**< Instancia 1 */
	HAL_DAC_SEL_AMOUNT /**< Cantidad de instancias */
}hal_dac_sel_en;

/** Tiempos de establecimiento disponibles para el *DAC* */
typedef enum
{
	HAL_DAC_SETTLING_TIME_1US_MAX = 0, /**< Tiempo de establecimiento de 1 microsegundo máximo */
	HAL_DAC_SETTLING_TIME_2_5US_MAX /**< Tiempo de establecimiento de 2.5 microsegundos máximo */
}hal_dac_settling_time_en;

/** Estructura de configuración del *DAC* */
typedef struct
{
	uint8_t count_enable : 1; /**< Habilitación del contador (para modo DMA) */
	uint8_t double_buffering : 1; /**< Doble buffer (para modo DMA) */
	uint8_t dma_enable : 1; /**< Habilitacion de funcionamiento con el DMA */
	uint8_t dma_request : 1; /**< Pedido de DMA */
}hal_dac_ctrl_config_t;

/**
 * @brief Inicialización del DAC
 *
 * Esta función inicializa el @ref IOCON de la forma necesaria para que el pin quede configurado correctamente.
 *
 * @param[in] dac Cual de los dos DACs inicializar
 * @param[in] settling_time Velocidad de conversión del DAC
 * @param[in] initial_value Valor inicial del DAC
 */
void hal_dac_init(hal_dac_sel_en dac, hal_dac_settling_time_en settling_time, uint32_t initial_value);

/**
 * @brief Actualización del valor actual del DAC
 * @param[in] dac En que DAC actualizar el valor
 * @param[in] new_value Nuevo valor a poner en el DAC
 * @pre Haber inicializado el periférico
 */
void hal_dac_update_value(hal_dac_sel_en dac, uint16_t new_value);

/**
 * @brief Configuración del registro de control del DAC
 * @param[in] dac Que DAC configurar
 * @param[in] config Configuración deseada
 * @pre Haber inicializado el periférico
 */
void hal_dac_config_ctrl(hal_dac_sel_en dac, hal_dac_ctrl_config_t * config);

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HAL_DAC_H_ */

/**
 * @}
 */
