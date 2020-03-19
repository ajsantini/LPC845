/**
 * @file HPL_DAC.h
 * @brief Declaraciones a nivel de abstraccion de periferico del DAC (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_DAC_H_
#define HPL_DAC_H_

#include <stdint.h>

typedef enum
{
	DAC_SEL_0 = 0,
	DAC_SEL_1
}DAC_sel_en;

typedef enum
{
	DAC_SETTLING_TIME_SEL_1US_MAX = 0,
	DAC_SETTLING_TIME_SEL_2_5US_MAX
}DAC_settling_time_sel_en;

typedef struct
{
	uint8_t dma_request : 1;
	uint8_t double_buffering : 1;
	uint8_t count_enable : 1;
	uint8_t dma_enable : 1;
}DAC_ctrl_config_t;

/**
 * @brief Inicializacion del DAC
 * @param[in] dac Cual de los dos DACs inicializar
 * @param[in] settling_time Velocidad de conversion del DAC
 * @param[in] initial_value Valor inicial del DAC
 */
void DAC_init(DAC_sel_en dac, DAC_settling_time_sel_en settling_time, uint16_t initial_value);

/**
 * @brief Deinicializacion del DAC
 * @param[in] dac Cual de los dos DACs deinicializar
 */
void DAC_deinit(DAC_sel_en dac);

/**
 * @brief Actualizacion del valor actual del DAC
 * @param[in] dac En que DAC actualizar el valor
 * @param[in] new_value Nuevo valor a poner en el DAC
 */
void DAC_update_value(DAC_sel_en dac, uint16_t new_value);

/**
 * @brief Configuracion del registro de control del DAC
 * @param[in] dac Que DAC configurar
 * @param[in] config Configuracion deseada
 */
void DAC_config_ctrl(DAC_sel_en dac, DAC_ctrl_config_t * config);

#endif /* HPL_DAC_H_ */
