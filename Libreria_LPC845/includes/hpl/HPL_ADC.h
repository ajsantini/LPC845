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

#define	ADC_INIT_SUCCESS						0
#define	ADC_INIT_CLK_OVERFLOW					-1
#define	ADC_INIT_CLK_UNDERFLOW					-2

#define	ADC_CONFIG_CONVERSIONS_SUCCESS			0
#define	ADC_CONFIG_CONVERSIONS_NOT_POWERED		-1
#define	ADC_CONFIG_CONVERSIONS_NOT_CLOCKED		-2
#define	ADC_CONFIG_CONVERSIONS_INVALID_CHANNELS	-3

#define	ADC_START_CONVERSIONS_SUCCESS			0
#define	ADC_START_CONVERSIONS_BURST_MODE		-1
#define	ADC_START_CONVERSIONS_NOT_POWERED		-2
#define	ADC_START_CONVERSIONS_NOT_CLOCKED		-3

#define	ADC_GET_CONVERSION_SUCCESS				0
#define	ADC_GET_CONVERSION_NOT_POWERED			-1
#define ADC_GET_CONVERSION_NOT_CLOCKED			-2
#define	ADC_GET_CONVERSION_INVALID_CHANNEL		-3
#define	ADC_GET_CONVERSION_NOT_FINISHED			-4

typedef struct
{
	uint16_t channels;
	uint8_t burst;
	void (*conversion_ended_callback)(void);
}ADC_conversions_config_t;

typedef enum
{
	ADC_CLOCK_SOURCE_FRO = 0,
	ADC_CLOCK_SOURCE_PLL
}ADC_clock_source_en;

/**
 * @brief Inicializacion del ADC
 *
 * Inicializa el ADC. Se ocupa de encender el periferico, clockear el mismo, la calibracion
 * de hardware, y fijar el clock del mismo para la frecuencia deseada.
 *
 * @param[in] adc_freq Frecuencia del ADC deseada
 * @param[in] clock_source Seleccion de clock para el ADC
 * @return Estado de inicializacion del ADC
 */
int32_t ADC_init(uint32_t adc_freq, ADC_clock_source_en clock_source);

/**
 * @brief Configuracion de las conversiones del ADC
 *
 * Configura el ADC con las siguientes salvedades:
 * -) Solo se permiten conversiones disparadas por software
 * -) Un disparo de conversion, convertira todos los canales habilitados
 * -) Las interrupciones seran una vez que termine la conversion de toda la secuencia
 *
 * @param[in] conversions_config Configuracion deseada para las conversiones
 * @return Estado de la configuracion del ADC
 */
int32_t ADC_config_conversions(const ADC_conversions_config_t * const conversions_config);

/**
 * @brief Iniciar conversiones de ADC
 *
 * El ADC debe haber sido previamente configurado correctamente.
 *
 * @return Estado del inicio de conversiones del ADC
 */
int32_t ADC_start_conversions(void);

/**
 * @brief Obtener resultado de conversiones de ADC de un canal en particular
 * @param[in] channel Canal del cual obtener el resultado
 * @param[out] conversion Resultado de la conversion de ADC del canal asociado
 * @return Estado del pedido de resultado de conversion
 */
int32_t ADC_get_conversion(uint8_t channel, uint32_t *conversion);

/**
 * @brief Registrar un callback a llamar en la funcion de interrupcion cuando termina la secuencia de conversiones
 * @param new_callback Puntero a funcion a llamar una vez terminada la secuencia de conversiones
 */
void ADC_register_callback(void (*new_callback)(void));

void ADC_SEQB_IRQHandler(void);

#endif /* HPL_ADC_H_ */
