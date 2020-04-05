/**
 * @file HAL_ADC.h
 * @brief Declaraciones a nivel de aplicacion del periferico ADC (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include <stdint.h>

typedef enum
{
	HAL_ADC_CLOCK_SOURCE_FRO = 0,
	HAL_ADC_CLOCK_SYS_PLL
}hal_adc_clock_source_en;

typedef enum
{
	HAL_ADC_OPERATION_MODE_SYNCHRONOUS = 0,
	HAL_ADC_OPERATION_MODE_ASYNCHRONOUS
}hal_adc_operation_mode_en;

typedef enum
{
	HAL_ADC_LOW_POWER_MODE_DISABLED = 0,
	HAL_ADC_LOW_POWER_MODE_ENABLED
}hal_adc_low_power_mode_en;

typedef enum
{
	HAL_ADC_SEQUENCE_SEL_A = 0,
	HAL_ADC_SEQUENCE_SEL_B
}hal_adc_sequence_sel_en;

typedef enum
{
	HAL_ADC_TRIGGER_SEL_NONE = 0,
	HAL_ADC_TRIGGER_SEL_PININT0_IRQ,
	HAL_ADC_TRIGGER_SEL_PININT1_IRQ,
	HAL_ADC_TRIGGER_SEL_SCT0_OUT3,
	HAL_ADC_TRIGGER_SEL_SCT0_OUT4,
	HAL_ADC_TRIGGER_SEL_T0_MAT3,
	HAL_ADC_TRIGGER_SEL_CMP0_OUT_ADC,
	HAL_ADC_TRIGGER_SEL_GPIO_INT_BMAT,
	HAL_ADC_TRIGGER_SEL_ARM_TXEV
}hal_adc_trigger_sel_en;

typedef enum
{
	HAL_ADC_TRIGGER_POL_SEL_NEGATIVE_EDGE = 0,
	HAL_ADC_TRIGGER_POL_SEL_POSITIVE_EDGE
}hal_adc_trigger_pol_sel_en;

typedef enum
{
	HAL_ADC_SYNC_SEL_ENABLE_SYNC = 0,
	HAL_ADC_SYNC_SEL_BYPASS_SYNC
}hal_adc_sync_sel_en;

typedef enum
{
	HAL_ADC_INTERRUPT_MODE_EOC = 0,
	HAL_ADC_INTERRUPT_MODE_EOS
}hal_adc_interrupt_mode_en;

typedef enum
{
	HAL_ADC_RESULT_CHANNEL_0 = 0,
	HAL_ADC_RESULT_CHANNEL_1,
	HAL_ADC_RESULT_CHANNEL_2,
	HAL_ADC_RESULT_CHANNEL_3,
	HAL_ADC_RESULT_CHANNEL_4,
	HAL_ADC_RESULT_CHANNEL_5,
	HAL_ADC_RESULT_CHANNEL_6,
	HAL_ADC_RESULT_CHANNEL_7,
	HAL_ADC_RESULT_CHANNEL_8,
	HAL_ADC_RESULT_CHANNEL_9,
	HAL_ADC_RESULT_CHANNEL_10,
	HAL_ADC_RESULT_CHANNEL_11,
	HAL_ADC_RESULT_CHANNEL_GLOBAL
}hal_adc_result_channel_en;

typedef enum
{
	HAL_ADC_SEQUENCE_RESULT_VALID = 0,
	HAL_ADC_SEQUENCE_RESULT_INVALID
}hal_adc_sequence_result_en;

typedef struct
{
	uint16_t channels;
	hal_adc_trigger_sel_en trigger;
	hal_adc_trigger_pol_sel_en trigger_pol;
	hal_adc_sync_sel_en sync_bypass;
	hal_adc_interrupt_mode_en mode;
	uint8_t burst;
	uint8_t single_step;
	uint8_t low_priority;
	void (*callback)(void);
}hal_adc_sequence_config_t;

typedef struct
{
	hal_adc_result_channel_en channel;
	uint16_t result;
}hal_adc_sequence_result_t;

/**
 * @brief Inicializar el ADC
 *
 * Realiza la calibracion de hardware y fija la frecuencia de sampleo deseada.
 *
 * @param[in] sample_freq Frecuencia de sampleo deseada
 * @param[in] div Divisor para la logica del ADC (tambien afecta a la frecuencia de sampleo en modo sincronico)
 * @param[in] clock_source Fuente de clock para el ADC (solo importa para modo asincronico)
 * @param[in] mode Seleccion de modo de operacion, sincronico o asincronico
 * @param[in] low_power Seleccion de modo de bajo consumo
 */
void hal_adc_init(uint32_t sample_freq, uint8_t div, hal_adc_clock_source_en clock_source, hal_adc_operation_mode_en mode, hal_adc_low_power_mode_en low_power);

/**
 * @brief Configurar una secuencia de conversion
 *
 * Esta funcion no habilita la secuencia, al menos que el parametro burst este activo
 *
 * @param[in] sequence Seleccion de secuencia a configurar
 * @param[in] config Configuracion deseada para la secuencia
 */
void hal_adc_config_sequence(hal_adc_sequence_sel_en sequence, const hal_adc_sequence_config_t *config);

/**
 * @brief Habilitar una secuencia
 * @param[in] sequence Secuencia a habilitar
 */
void hal_adc_enable_sequence(hal_adc_sequence_sel_en sequence);

/**
 * @brief Disparar conversiones en una secuencia
 *
 * La configuracion de la secuencia, en particular el parametro single_step, influye
 * en si esta funcion dispara una secuencia entera o un paso de la misma.
 *
 * @param[in] sequence Secuencia a disparar
 */
void hal_adc_start_sequence(hal_adc_sequence_sel_en sequence);

/**
 * @brief Obtener resultado de la secuencia
 *
 * El comportamiento de esta funcion depende de la configuracion de la secuencia, en particular
 * de la configuracion <b>MODE</b>. En caso de estar configurada para interrumpir al final de cada
 * conversion, la funcion unicamente guardara el resultado de la conversion en el primer lugar
 * del parametro <e>result</e>, caso contrario, guardara la cantidad de canales habilitados en la
 * conversion en los distintos lugares del parametro <e>result</e>.
 *
 * @param[in] sequence Secuencia de la cual obtener el resultado
 * @param[out] result Lugares donde guardar los resultados de la secuencia
 * @return Resultado de la funcion
 */
hal_adc_sequence_result_en hal_adc_get_sequence_result(hal_adc_sequence_sel_en sequence, hal_adc_sequence_result_t *result[]);

#endif /* HAL_ADC_H_ */
