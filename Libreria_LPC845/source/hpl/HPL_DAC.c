/**
 * @file HPL_DAC.c
 * @brief Funciones a nivel de abstraccion de periferico para el DAC (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdint.h>
#include <HPL_DAC.h>
#include <HRI_DAC.h>
#include <HRI_SYSCON.h>
#include <HRI_SWM.h>

volatile DAC_per_t * const DAC[] = { //!< Perifericos DAC
		(DAC_per_t *) DAC0_BASE,
		(DAC_per_t *) DAC1_BASE
};

/**
 * @brief Inicializacion del DAC
 * @param[in] dac Cual de los dos DACs inicializar
 * @param[in] settling_time Velocidad de conversion del DAC
 * @param[in] initial_value Valor inicial del DAC
 */
void DAC_init(DAC_sel_en dac, DAC_settling_time_sel_en settling_time, uint16_t initial_value)
{
	// Activo Switch-Matrix
	SYSCON->SYSAHBCLKCTRL0.SWM = 1;

	// Encendido de DACs
	if(dac == DAC_SEL_0)
	{
		SYSCON->PDRUNCFG.DAC0 = 0;
		SYSCON->SYSAHBCLKCTRL0.DAC0 = 1;
		SWM->PINENABLE0.DACOUT0 = 0;
	}
	else
	{
		SYSCON->PDRUNCFG.DAC1 = 0;
		SYSCON->SYSAHBCLKCTRL1.DAC1 = 1;
		SWM->PINENABLE0.DACOUT1 = 0;
	}

	// Desactivo Switch-Matrix
	SYSCON->SYSAHBCLKCTRL0.SWM = 0;

	DAC[dac]->CR.BIAS = settling_time;
	DAC[dac]->CR.VALUE = initial_value & 0x3FF;
}

/**
 * @brief Actualizacion del valor actual del DAC
 * @param[in] dac En que DAC actualizar el valor
 * @param[in] new_value Nuevo valor a poner en el DAC
 */
void DAC_update_value(DAC_sel_en dac, uint16_t new_value)
{
	DAC[dac]->CR.VALUE = new_value;
}

/**
 * @brief Configuracion del registro de control del DAC
 * @param[in] dac Que DAC configurar
 * @param[in] config Configuracion deseada
 */
void DAC_config_ctrl(DAC_sel_en dac, DAC_ctrl_config_t * config)
{
	DAC[dac]->CTRL.CNT_ENA = config->count_enable;
	DAC[dac]->CTRL.DBLBUF_ENA = config->double_buffering;
	DAC[dac]->CTRL.DMA_ENA = config->dma_enable;
	DAC[dac]->CTRL.INT_DMA_REQ = config->dma_request;
}
