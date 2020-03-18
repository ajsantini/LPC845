/**
 * @file HPL_ADC.h
 * @brief Declaraciones a nivel de abstraccion de periferico del ADC (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_ADC_H_
#define HPL_ADC_H_

#include <stdint.h>

#define MAX_ADC_CONVERSION_VALUE				0xFFF

typedef struct
{
	uint16_t channels;
	uint8_t burst;
	void (*conversion_ended_callback)(void);
}ADC_conversions_config_t;

typedef struct
{
	uint8_t clock_div;
	uint8_t async_mode : 1;
	uint8_t low_power_mode : 1;
	uint8_t voltage_range : 1;
}ADC_config_t;

typedef enum
{
	ADC_CLOCK_SOURCE_FRO = 0,
	ADC_CLOCK_SOURCE_PLL
}__attribute__((packed)) ADC_clock_source_en;

typedef enum
{
	ADC_SEQUENCE_SEL_A = 0,
	ADC_SEQUENCE_SEL_B
}__attribute__((packed)) ADC_sequence_sel_en;

typedef enum
{
	ADC_THRESHOLD_SEL_0 = 0,
	ADC_THRESHOLD_SEL_1
}__attribute__((packed)) ADC_threshold_sel_en;

typedef enum
{
	ADC_INTERRUPT_MODE_EOC = 0,
	ADC_INTERRUPT_MODE_EOS
}__attribute__((packed)) ADC_interrupt_mode_en;

typedef enum
{
	ADC_THRESHOLD_INTERRUPT_SEL_OUTSIDE = 1,
	ADC_THRESHOLD_INTERRUPT_SEL_CROSSING
}__attribute__((packed)) ADC_threshold_interrupt_sel_en;

typedef struct
{
	uint16_t channels;
	uint8_t trigger;
	uint8_t trigger_polarity;
	uint8_t sync_bypass;
	uint8_t single_step;
	uint8_t low_priority;
	uint8_t interrupt_mode;
}ADC_sequence_config_t;

typedef struct
{
	uint32_t : 4;
	uint32_t RESULT : 12;
	uint32_t THCMPRANGE : 2;
	uint32_t THCMPCROSS : 2;
	uint32_t : 6;
	uint32_t CHANNEL : 4;
	uint32_t OVERRUN : 1;
	uint32_t DATAVALID : 1;
}ADC_global_data_t;

typedef struct
{
	uint32_t : 4;
	uint32_t RESULT : 12;
	uint32_t THCMPRANGE : 2;
	uint32_t THCMPCROSS : 2;
	uint32_t : 6;
	uint32_t CHANNEL : 4;
	uint32_t OVERRUN : 1;
	uint32_t DATAVALID : 1;
}ADC_channel_data_t;

/**
 * @brief Inicializacion del ADC
 * @param[in] clock_source Seleccion de clock para el ADC
 * @param[in] adc_config Configuracion del ADC deseada
 */
void ADC_init(ADC_clock_source_en clock_source, const ADC_config_t *adc_config);

/**
 * @brief Configuracion de una secuencia del ADC
 * @param[in] sequence Que secuencia configurar
 * @param[in] config Configuracion deseada para la secuencia
 */
void ADC_config_sequence(ADC_sequence_sel_en sequence, const ADC_sequence_config_t *config);

/**
 * @brief Iniciar conversiones en modo burst en una secuencia
 * @param[in] sequence En que secuencia iniciar conversiones en modo rafaga
 */
void ADC_start_burst_in_sequence(ADC_sequence_sel_en sequence);

/**
 * @brief Detener conversiones en modo burst en una secuencia
 * @param[in] sequence En que secuencia detener conversiones en modo rafaga
 */
void ADC_stop_burst_in_sequence(ADC_sequence_sel_en sequence);

/**
 * @brief Habilitar secuencia
 * @param[in] sequence Que secuencia habilitar
 */
void ADC_enable_sequence(ADC_sequence_sel_en sequence);

/**
 * @brief Inhabilitar secuencia
 * @param[in] sequence Que secuencia inhabilitar
 */
void ADC_disable_sequence(ADC_sequence_sel_en sequence);

/**
 * @brief Generar un software start en una secuencia
 * @param[in] sequence Sobre que secuencia generar el software start
 */
void ADC_software_start_sequence(ADC_sequence_sel_en sequence);

/**
 * @brief Fijar valor de umbral de comparacion (parte baja)
 * @param[in] threshold_selection Que threshold fijar
 * @param[in] threshold_value Valor de threshold a fijar
 */
void ADC_set_compare_low_threshold(ADC_threshold_sel_en threshold_selection, uint16_t threshold_value);

/**
 * @brief Fijar valor de umbral de comparacion (parte alta)
 * @param[in] threshold_selection Que threshold fijar
 * @param[in] threshold_value Valor de threshold a fijar
 */
void ADC_set_compare_high_threshold(ADC_threshold_sel_en threshold_selection, uint16_t threshold_value);

/**
 * @brief Fijar contra que umbral de comparacion se compara el canal
 * @param[in] channel Numero de canal
 * @param[in] threshold_selection Contra que umbral de comparacion comparar
 */
void ADC_set_channel_threshold(uint8_t channel, ADC_threshold_sel_en threshold_selection);

/**
 * @brief Habilitacion de interrupcion de secuencia
 * @param[in] sequence Sobre que secuencia habilitar la interrupcion
 */
void ADC_enable_sequence_interrupt(ADC_sequence_sel_en sequence);

/**
 * @brief Inhabilitacion de interrupcion de secuencia
 * @param[in] sequence Sobre que secuencia inhabilitar la interrupcion
 */
void ADC_disable_sequence_interrupt(ADC_sequence_sel_en sequence);

/**
 * @brief Configurar un callback para cuando se genera una interrupcion de secuencia
 * @param[in] sequence Sobre que secuencia configurar el callback
 * @param[in] callback Puntero a la funcion a ejecutar cuando ocurra la interrupcion
 */
void ADC_set_sequence_callback(ADC_sequence_sel_en sequence, void (*callback)(void));

/**
 * @brief Habilitacion de interrupcion de overrun
 */
void ADC_enable_overrun_interrupt(void);

/**
 * @brief Inhabilitacion de interrupcion de overrun
 */
void ADC_disable_overrun_interrupt(void);

/**
 * @brief Configurar un callback para cuando se genera una interrupcion de overrun
 * @param[in] callback Puntero a la funcion a ejecutar cuando ocurra la interrupcion
 */
void ADC_set_overrun_callback(void (*callback)(void));

/**
 * @brief Habilitacion de interrupcion de threshold
 * @param[in] channel Sobre que canal habilitar la interrupcion
 * @param[in] mode Modo de interrupcion
 */
void ADC_enable_threshold_interrupt(uint8_t channel, ADC_threshold_interrupt_sel_en mode);

/**
 * @brief Inhabilitacion de interrupcion de threshold
 * @param[in] channel Sobre que canal inhabilitar la interrupcion
 */
void ADC_disable_threshold_interrupt(uint8_t channel);

/**
 * @brief Configurar un callback para cuando se genera una interrupcion de threshold
 * @param[in] callback Puntero a la funcion a ejecutar cuando ocurra la interrupcion
 */
void ADC_set_threshold_callback(void (*callback)(void));

/**
 * @brief Leer alguno de los registros globales de resultado de conversion
 * @param[in] seequence De que secuencia leer el resultado global de conversion
 * @param[out] data Contenido del registro
 */
void ADC_get_global_data(ADC_sequence_sel_en sequence, ADC_global_data_t *data);

/**
 * @brief Leer alguno de los registros de canal de resultado de conversion
 * @param[in] channel De que canal leer el resultado de canal de conversion
 * @param[out] data Contenido del registro
 */
void ADC_get_channel_data(uint8_t channel, ADC_channel_data_t *data);

#endif /* HPL_ADC_H_ */
