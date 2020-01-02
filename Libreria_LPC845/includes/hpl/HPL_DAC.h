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

#define	DAC_INIT_SUCCESS			0
#define	DAC_INIT_INVALID_DAC		-1

#define	DAC_UPDATE_SUCCESS			0
#define	DAC_UPDATE_INVALID_DAC		-1

/**
 * @brief Inicializacion del DAC
 * @param[in] dac_selection Cual de los dos DACS inicializar.
 * @param[in] settling_time Velocidad de conversion del DAC. Cero es velocidad baja, cualquier otra cosa es velocidad rapida.
 * @param[in] initial_value Valor inicial del DAC
 * @return Estado de inicializacion del DAC
 */
int32_t DAC_init(uint8_t dac_selection, uint8_t settling_time, uint32_t initial_value);

/**
 * @brief Actualizacion del valor actual del DAC
 * @param[in] dac_selection DAC en el cual actualizar el valor
 * @param[in] new_value Nuevo valor a poner en el DAC
 * @return Resultado de la actualizacion
 */
int32_t DAC_update_value(uint8_t dac_selection, uint32_t new_value);

#endif /* HPL_DAC_H_ */
