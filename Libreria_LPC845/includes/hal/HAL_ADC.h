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
#include <HPL_ADC.h>

/**
 * @brief Inicializar el ADC
 *
 * Realiza la calibracion de hardware y fija la frecuencia de sampleo deseada
 *
 * @param[in] sample_freq Frecuencia de sampleo deseada
 * @param[in] clock_source Fuente de clock a utilizar
 */
void hal_adc_init(uint32_t sample_freq, ADC_clock_source_en clock_source);

#endif /* HAL_ADC_H_ */
