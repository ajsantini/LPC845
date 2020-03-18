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
	ADC_INTERRUPT_MODE_EOC = 0,
	ADC_INTERRUPT_MODE_EOS
}__attribute__((packed)) ADC_interrupt_mode_en;

typedef struct
{
	uint16_t channels;
	uint8_t trigger;
	uint8_t trigger_polarity;
	uint8_t sync_bypass;
	uint8_t burst;
	uint8_t single_step;
	uint8_t low_priority;
	uint8_t interrupt_mode;
}ADC_sequence_config_t;

/**
 * @brief Inicializacion del ADC
 * @param[in] clock_source Seleccion de clock para el ADC
 * @param[in] adc_config Configuracion del ADC deseada
 */
void ADC_init(ADC_clock_source_en clock_source, const ADC_config_t *adc_config);

void ADC_config_sequence(ADC_sequence_sel_en, )

/**
 * @brief Configuracion de las conversiones del ADC
 *
 * Configura el ADC con las siguientes salvedades:
 * -) Solo se permiten conversiones disparadas por software
 * -) Un disparo de conversion, convertira todos los canales habilitados
 * -) Las interrupciones seran una vez que termine la conversion de toda la secuencia
 *
 * @param[in] conversions_config Configuracion deseada para las conversiones
 */
void ADC_config_conversions(const ADC_conversions_config_t * const conversions_config);

/**
 * @brief Iniciar conversiones de ADC
 *
 * El ADC debe haber sido previamente configurado correctamente.
 */
void ADC_start_conversions(void);

/**
 * @brief Obtener resultado de conversiones de ADC de un canal en particular
 * @param[in] channel Canal del cual obtener el resultado
 * @param[out] conversion Resultado de la conversion de ADC del canal asociado
 */
void ADC_get_conversion(uint8_t channel, uint32_t *conversion);

/**
 * @brief Registrar un callback a llamar en la funcion de interrupcion cuando termina la secuencia de conversiones
 * @param new_callback Puntero a funcion a llamar una vez terminada la secuencia de conversiones
 */
void ADC_register_callback(void (*new_callback)(void));

#endif /* HPL_ADC_H_ */
