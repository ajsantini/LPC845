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
#include <HRI_DAC.h>

extern volatile DAC_per_t * const DAC[]; //!< Perifericos DAC

typedef enum
{
	DAC_SEL_0 = 0,
	DAC_SEL_1
}DAC_sel_en;

typedef enum
{
	DAC_SETTLING_TIME_SEL_1US_MAX = 0,
	DAC_SETTLING_TIME_SEL_2_5US_MAX
}DAC_settling_time_en;

/**
 * @brief Actualizacion del valor actual del DAC
 * @param[in] dac Instancia a actualizar
 * @param[in] new_value Nuevo valor a poner en el DAC
 */
static inline void DAC_write(DAC_sel_en dac, uint16_t new_value)
{
	DAC[dac]->CR.VALUE = new_value;
}

/**
 * @brief Configuracion del settling time del DAC
 * @param[in] dac Instancia a configurar
 * @param[in] settling_time Configuracion deseada
 */
static inline void DAC_config_settling_time(DAC_sel_en dac, DAC_settling_time_en settling_time)
{
	DAC[dac]->CR.BIAS = settling_time;
}

/**
 * @brief Habilitar interrupcion de DMA cuando el timer tiemoutea
 * @param[in] dac Instancia a configurar
 */
static inline void DAC_enable_DMA_request(DAC_sel_en dac)
{
	DAC[dac]->CTRL.INT_DMA_REQ = 1;
}

/**
 * @brief Inhabilitar interrupcion de DMA cuando el timer tiemoutea
 * @param[in] dac Instancia a configurar
 */
static inline void DAC_disable_DMA_request(DAC_sel_en dac)
{
	DAC[dac]->CTRL.INT_DMA_REQ = 0;
}

/**
 * @brief Habilitar double buffering
 * @param[in] dac Instancia a configurar
 */
static inline void DAC_enable_double_buffer(DAC_sel_en dac)
{
	DAC[dac]->CTRL.DBLBUF_ENA = 1;
}

/**
 * @brief Inhabilitar double buffering
 * @param[in] dac Instancia a configurar
 */
static inline void DAC_disable_double_buffer(DAC_sel_en dac)
{
	DAC[dac]->CTRL.DBLBUF_ENA = 0;
}

/**
 * @brief Habilitar operacion del timer
 * @param[in] dac Instancia a configurar
 */
static inline void DAC_enable_timer(DAC_sel_en dac)
{
	DAC[dac]->CTRL.CNT_ENA = 1;
}

/**
 * @brief Inhabilitar operacion del timer
 * @param[in] dac Instancia a configurar
 */
static inline void DAC_disable_timer(DAC_sel_en dac)
{
	DAC[dac]->CTRL.CNT_ENA = 0;
}

/**
 * @brief Habilitar DMA request asociada al DAC
 * @param[in] dac Instancia a configurar
 */
static inline void DAC_enable_DMA(DAC_sel_en dac)
{
	DAC[dac]->CTRL.DMA_ENA = 1;
}

/**
 * @brief Inhabilitar DMA request asociada al DAC
 * @param[in] dac Instancia a configurar
 */
static inline void DAC_disable_DMA(DAC_sel_en dac)
{
	DAC[dac]->CTRL.DMA_ENA = 0;
}

/**
 * @brief Escribir valor a recargar para el timer de DMA
 * @param[in] dac Instancia a configurar
 * @param[in] value Valor deseado
 */
static inline void DAC_write_reaload_value(DAC_sel_en dac, uint16_t value)
{
	DAC[dac]->CNTVAL.VALUE = value;
}

#endif /* HPL_DAC_H_ */
