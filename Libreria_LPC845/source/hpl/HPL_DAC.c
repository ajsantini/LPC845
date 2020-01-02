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
 * @param[in] dac_selection Cual de los dos DACS inicializar.
 * @param[in] settling_time Velocidad de conversion del DAC. Cero es velocidad baja, cualquier otra cosa es velocidad rapida.
 * @param[in] initial_value Valor inicial del DAC
 * @return Estado de inicializacion del DAC
 */
int32_t DAC_init(uint8_t dac_selection, uint8_t settling_time, uint32_t initial_value)
{
	if(dac_selection > 1)
	{
		return DAC_INIT_INVALID_DAC;
	}

	// Activo Switch-Matrix
	SYSCON->SYSAHBCLKCTRL0.SWM = 1;

	// Enciendo DACs
	if(dac_selection == 0)
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

	if(settling_time)
	{
		settling_time = 1;
	}

	DAC[dac_selection]->CR.BIAS = settling_time;
	DAC[dac_selection]->CR.VALUE = initial_value & 0x3FF;

	return DAC_INIT_SUCCESS;
}

/**
 * @brief Actualizacion del valor actual del DAC
 * @param[in] dac_selection DAC en el cual actualizar el valor
 * @param[in] new_value Nuevo valor a poner en el DAC
 * @return Resultado de la actualizacion
 */
int32_t DAC_update_value(uint8_t dac_selection, uint32_t new_value)
{
	if(dac_selection > 1)
	{
		return DAC_UPDATE_INVALID_DAC;
	}

	DAC[dac_selection]->CR.VALUE = new_value & 0x3FF;

	return DAC_UPDATE_SUCCESS;
}
