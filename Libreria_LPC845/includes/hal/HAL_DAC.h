/**
 * @file HAL_DAC.h
 * @brief Declaraciones a nivel de aplicacion del periferico DAC (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @example Ejemplo_DAC.c
 *
 * El programa utiliza el \e DAC para generar una señal tipo rampa en la salida del canal 0 (Puerto 0 Pin 17) el
 * cual está mapeado al pin número 2 del stick de desarrollo.
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
 * El periférico \e Systick se configura para generar interrupciones cada \f$1mseg\f$. En cada tick, se escribe en
 * el \e DAC un valor de una tabla, cuyo indice ira incrementando en cada tick. En la tabla se encuentran guardados
 * los valores de la señal tipo rampa, incializados al comienzo del programa en un lazo \e for.
 *
 * El periférico \e DAC es inicializado con un tiempo de asentamiento de \f$1 \mu seg\f$.
 *
 */

/**
 * @defgroup DAC Conversor digital a analógico (DAC)
 *
 * # Introducción
 *
 * El \e DAC, como su nombre lo indica, convierte un valor digital en una tensión analógica. En el caso de este
 * microcontrolador, tiene dos módulos \e DACs independientes con una resolución de 10 bits cada uno. Se puede
 * calcular la resolución de cada paso de \e DAC como:
 *
 * \f{eqnarray*}{
 * 		DAC_{res} = \frac{V_{ref_{p}}}{2^N}
 * \f}
 *
 * Siendo \f$N\f$ la cantidad de bits de resolución del \e DAC. Esto implica que podemos preveer el valor de salida
 * de tensión analógica conociendo el valor a convertir por el \e DAC mediante:
 *
 * \f{eqnarray*}{
 * 		DAC_{conv} = DAC_{res} * DAC_{val}
 * \f}
 *
 * # Velocidad de conversión
 *
 * El periférico permite dos velocidades de conversión:
 * 		- Frecuencia de conversión de \f$1Mhz\f$ : Implica un mayor consumo de energía
 * 		- Frecuencia de conversión de \f$400KHz\f$ : Implica un menor consumo de energía
 * 		.
 *
 * Existe una relación entre velocidad de conversión y consumo de energía del periférico.
 *
 * # Campos de aplicación típicos
 *
 * - Generación de señales de naturaleza analógica
 * - Generación de señales auxiliares para calibración
 * - Servomecanismos controlados por señales analógicas
 *
 * @{
 */

#ifndef HAL_DAC_H_
#define HAL_DAC_H_

#include <stdint.h>

typedef enum
{
	HAL_DAC_0 = 0,
	HAL_DAC_1
}hal_dac_en;

typedef enum
{
	HAL_DAC_SETTLING_TIME_1US_MAX = 0,
	HAL_DAC_SETTLING_TIME_2_5US_MAX
}hal_dac_settling_time_en;

typedef struct
{
	uint8_t count_enable : 1;
	uint8_t double_buffering : 1;
	uint8_t dma_enable : 1;
	uint8_t dma_request : 1;
}hal_dac_ctrl_config_t;

/**
 * @brief Inicializacion del DAC
 * @param[in] dac Cual de los dos DACs inicializar
 * @param[in] settling_time Velocidad de conversion del DAC
 * @param[in] initial_value Valor inicial del DAC
 */
void hal_dac_init(hal_dac_en dac, hal_dac_settling_time_en settling_time, uint32_t initial_value);

/**
 * @brief Actualizacion del valor actual del DAC
 * @param[in] dac En que DAC actualizar el valor
 * @param[in] new_value Nuevo valor a poner en el DAC
 */
void hal_dac_update_value(hal_dac_en dac, uint16_t new_value);

/**
 * @brief Configuracion del registro de control del DAC
 * @param[in] dac Que DAC configurar
 * @param[in] config Configuracion deseada
 */
void hal_dac_config_ctrl(hal_dac_en dac, hal_dac_ctrl_config_t * config);

#endif /* HAL_DAC_H_ */

/**
 * @}
 */
