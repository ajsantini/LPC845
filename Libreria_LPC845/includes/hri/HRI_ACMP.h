/**
 * @file HRI_ACMP.h
 * @brief Declaraciones a nivel de registros del ADC (LPC845)
 * @author Esteban E. Chiama
 * @date 4/2020
 * @version 1.0
 */

#ifndef HRI_ACMP_H_
#define HRI_ACMP_H_

#include <stdint.h>

#if defined (__cplusplus)
extern "C" {
#endif

#define			ACMP_BASE			0x40024000 //!< Direccion base del Comparador Analígico

//! Registro de control del comparador analógico
typedef struct
{
	uint32_t : 3; //!< Reservado
	uint32_t EDGESEL : 2; //!< Selección de tipo de flanco que establece el bit COMPEDGE.
	uint32_t : 1; //!< Reservado
	uint32_t COMPSA : 1; //!< Configura salida directa o sincronizada al bus.
	uint32_t : 1; //!< Reservado
	uint32_t COMP_VP_SEL : 3; //!< Selección de entrada para voltaje positivo.
	uint32_t COMP_VM_SEL : 3; //!< Selección de entrada para voltaje negativo.
	uint32_t : 6; //!< Reservado
	uint32_t EDGECLR : 1; //!< Bit de limpieza de interrupción.
	uint32_t COMPSTAT: 1; //!< Refleja el estado de salida de la comparación.
	uint32_t : 1; //!< Reservado
	uint32_t COMPEDGE : 1; //!< Indica si hubo o no detección de flanco.
	uint32_t INTENA : 1; //!< Enable de interrupción.
	uint32_t HYS : 2; //!< Control de histérisis del comparador.
	uint32_t : 5; //!< Reservado
}ACMP_CTRL_reg_t;

//! Registro de habilitación y control de la voltage ladder.
typedef struct
{
	uint32_t LADEN : 1; //!< Enable de voltage ladder.
	uint32_t LADSEL : 5; //!< Valor de la voltage ladder.
	uint32_t LADREF : 1; //!< Selección del voltage de referencia Vref para la voltage ladder.
	uint32_t : 25; //!< Reservado
}ACMP_LAD_reg_t;

typedef struct
{
	ACMP_CTRL_reg_t CTRL;
	ACMP_LAD_reg_t LAD;
}ACMP_per_t;

extern volatile ACMP_per_t * const ACMP; //!< Periferico Comparador Analógico.

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HRI_ACMP_H_ */
