/**
 * @file HAL_DAC.h
 * @brief Declaraciones a nivel de aplicacion del periferico DAC (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
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

#endif /* HAL_DAC_H_ */
