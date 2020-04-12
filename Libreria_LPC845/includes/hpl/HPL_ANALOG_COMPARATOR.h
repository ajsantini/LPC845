/*
 * HPL_ANALOG_COMPARATOR.h
 *
 *  Created on: Apr 12, 2020
 *      Author: stv
 */

#ifndef HPL_ANALOG_COMPARATOR_H_
#define HPL_ANALOG_COMPARATOR_H_

#include <HRI_ANALOG_COMPARATOR.h>

extern volatile ANALOG_COMPARATOR_per_t * const ANALOG_COMPARATOR; //!< Periferico ANALOG COMPARATOR

typedef enum
{
	ANALOG_COMPARATOR_EDGE_FALLING = 0,
	ANALOG_COMPARATOR_EDGE_RISING,
	ANALOG_COMPARATOR_EDGE_BOTH
}ANALOG_COMPARATOR_edge_sel_en;

typedef enum
{
	ANALOG_COMPARATOR_OUTPUT_DIRECT = 0,
	ANALOG_COMPARATOR_OUTPUT_SYNC,
}ANALOG_COMPARATOR_output_control_en;

typedef enum
{
	ANALOG_COMPARATOR_INPUT_VOLTAGE_VLADDER_OUT = 0,
	ANALOG_COMPARATOR_INPUT_VOLTAGE_ACMP_I1,
	ANALOG_COMPARATOR_INPUT_VOLTAGE_ACMP_I2,
	ANALOG_COMPARATOR_INPUT_VOLTAGE_ACMP_I3,
	ANALOG_COMPARATOR_INPUT_VOLTAGE_ACMP_I4,
	ANALOG_COMPARATOR_INPUT_VOLTAGE_ACMP_I5,
	ANALOG_COMPARATOR_INPUT_VOLTAGE_BANDGAP,
	ANALOG_COMPARATOR_INPUT_VOLTAGE_DACOUT0
}ANALOG_COMPARATOR_input_voltage_sel_en;

typedef enum
{
	ANALOG_COMPARATOR_HYSTERESIS_NONE = 0,
	ANALOG_COMPARATOR_HYSTERESIS_5mV,
	ANALOG_COMPARATOR_HYSTERESIS_10mV,
	ANALOG_COMPARATOR_HYSTERESIS_20mV
}ANALOG_COMPARATOR_hysteresis_sel_en;

typedef enum
{
	ANALOG_COMPARATOR_LADDER_VREF_VDD = 0,
	ANALOG_COMPARATOR_LADDER_VREF_VDDCMP
}ANALOG_COMPARATOR_ladder_vref_sel_en;

static inline void ANALOG_COMPARATOR_control_config(ANALOG_COMPARATOR_edge_sel_en edge_sel,
		ANALOG_COMPARATOR_output_control_en output_ctrl, ANALOG_COMPARATOR_hysteresis_sel_en hyst_sel)
{
	ANALOG_COMPARATOR->CTRL.EDGESEL = edge_sel;
	ANALOG_COMPARATOR->CTRL.COMPSA = output_ctrl;
	ANALOG_COMPARATOR->CTRL.HYS = hyst_sel;
}

static inline void ANALOG_COMPARATOR_voltage_input_select(ANALOG_COMPARATOR_input_voltage_sel_en in_positive,
		ANALOG_COMPARATOR_input_voltage_sel_en in_negative)
{
	ANALOG_COMPARATOR->CTRL.COMP_VP_SEL = in_positive;
	ANALOG_COMPARATOR->CTRL.COMP_VM_SEL = in_negative;
}

static inline void ANALOG_COMPARATOR_interrupt_clear(void)
{
	ANALOG_COMPARATOR->CTRL.EDGECLR = 1;
	ANALOG_COMPARATOR->CTRL.EDGECLR = 0;
}

static inline uint8_t ANALOG_COMPARATOR_interrupt_flag_get(void)
{
	return ANALOG_COMPARATOR->CTRL.EDGECLR;
}

static inline uint8_t ANALOG_COMPARATOR_output_status_get(void)
{
	return ANALOG_COMPARATOR->CTRL.COMPSTAT;
}

static inline void ANALOG_COMPARATOR_interrupt_enable(void)
{
	ANALOG_COMPARATOR->CTRL.INTENA = 1;
}

static inline void ANALOG_COMPARATOR_interrupt_disable(void)
{
	ANALOG_COMPARATOR->CTRL.INTENA = 0;
}

static inline uint8_t ANALOG_COMPARATOR_edge_detected_get(void)
{
	return ANALOG_COMPARATOR->CTRL.COMPEDGE;
}

static inline void ANALOG_COMPARATOR_ladder_enable(void)
{
	ANALOG_COMPARATOR->LAD.LADEN = 1;
}

static inline void ANALOG_COMPARATOR_ladder_disable(void)
{
	ANALOG_COMPARATOR->LAD.LADEN = 0;
}

// PARA ESTAS 2 USO uint8_t O HAGO UN ENUM ??
// ===========================================
static inline void ANALOG_COMPARATOR_ladder_value_set(uint8_t)
{
	#warning hacer la función. No sé que tipo de dato usar.
}

// ídem arriba
static inline uint8_t ANALOG_COMPARATOR_ladder_value_get(void)
{
	return 0;
}

static inline void ANALOG_COMPARATOR_voltage_ladder_vref_set(ANALOG_COMPARATOR_ladder_vref_sel_en vref)
{
	ANALOG_COMPARATOR->LAD.LADREF = vref;
}

static inline uint8_t ANALOG_COMPARATOR_voltage_ladder_vref_get(void)
{
	return ANALOG_COMPARATOR->LAD.LADREF;
}


#endif /* HPL_ANALOG_COMPARATOR_H_ */
