/**
 * @file HAL_GPIO.c
 * @brief Funciones a nivel de aplicacion del periferico GPIO (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_DAC.h>
#include <HPL_DAC.h>
#include <HPL_SYSCON.h>
#include <HPL_SWM.h>
#include <HPL_IOCON.h>

/**
 * @brief Inicialización del DAC
 * @param[in] dac Cual de los dos DACs inicializar
 * @param[in] settling_time Velocidad de conversión del DAC
 * @param[in] initial_value Valor inicial del DAC
 */
void hal_dac_init(hal_dac_en dac, hal_dac_settling_time_en settling_time, uint32_t initial_value)
{
	SWM_init();
	IOCON_init();
	if(dac == HAL_DAC_0)
	{
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_DAC0);
		SYSCON_power_up_peripheral(SYSCON_POWER_SEL_DAC0);
		SWM_enable_DAC(dac, SWM_ENABLE);
		IOCON_enable_dac0();
	}
	else
	{
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_DAC1);
		SYSCON_power_up_peripheral(SYSCON_POWER_SEL_DAC1);
		SWM_enable_DAC(dac, SWM_ENABLE);
		IOCON_enable_dac1();
	}
	IOCON_deinit();
	SWM_deinit();

	DAC_config_settling_time(dac, settling_time);
	DAC_write(dac, initial_value);
}

/**
 * @brief Actualización del valor actual del DAC
 * @param[in] dac En que DAC actualizar el valor
 * @param[in] new_value Nuevo valor a poner en el DAC
 */
void hal_dac_update_value(hal_dac_en dac, uint16_t new_value)
{
	DAC_write(dac, new_value);
}

/**
 * @brief Configuración del registro de control del DAC
 * @param[in] dac Que DAC configurar
 * @param[in] config Configuración deseada
 */
void hal_dac_config_ctrl(hal_dac_en dac, hal_dac_ctrl_config_t * config)
{
	if(config->count_enable)
	{
		DAC_enable_timer(dac);
	}
	else
	{
		DAC_disable_timer(dac);
	}

	if(config->double_buffering)
	{
		DAC_enable_double_buffer(dac);
	}
	else
	{
		DAC_disable_double_buffer(dac);
	}

	if(config->dma_enable)
	{
		DAC_enable_DMA(dac);
	}
	else
	{
		DAC_disable_DMA(dac);
	}

	if(config->dma_request)
	{
		DAC_enable_DMA_request(dac);
	}
	else
	{
		DAC_disable_DMA_request(dac);
	}
}
