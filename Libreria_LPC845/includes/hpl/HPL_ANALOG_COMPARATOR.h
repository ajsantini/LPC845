/**
 * @file HPL_ACMP.h
 * @brief Declaraciones a nivel de abstraccion de periferico del ADC (LPC845)
 * @author Esteban E. Chiama
 * @date 4/2020
 * @version 1.0
 */

#ifndef HPL_ANALOG_COMPARATOR_
#define HPL_ANALOG_COMPARATOR_

#include <HRI_ANALOG_COMPARATOR.h>

extern volatile ACMP_per_t * const ACMP; //!< Periferico ANALOG COMPARATOR

typedef enum
{
	ACMP_EDGE_FALLING = 0,
	ACMP_EDGE_RISING,
	ACMP_EDGE_BOTH
}ACMP_edge_sel_en;

typedef enum
{
	ACMP_OUTPUT_DIRECT = 0,
	ACMP_OUTPUT_SYNC,
}ACMP_output_control_en;

typedef enum
{
	ACMP_INPUT_VOLTAGE_VLADDER_OUT = 0,
	ACMP_INPUT_VOLTAGE_ACMP_I1,
	ACMP_INPUT_VOLTAGE_ACMP_I2,
	ACMP_INPUT_VOLTAGE_ACMP_I3,
	ACMP_INPUT_VOLTAGE_ACMP_I4,
	ACMP_INPUT_VOLTAGE_ACMP_I5,
	ACMP_INPUT_VOLTAGE_BANDGAP,
	ACMP_INPUT_VOLTAGE_DACOUT0
}ACMP_input_voltage_sel_en;

typedef enum
{
	ACMP_HYSTERESIS_NONE = 0,
	ACMP_HYSTERESIS_5mV,
	ACMP_HYSTERESIS_10mV,
	ACMP_HYSTERESIS_20mV
}ACMP_hysteresis_sel_en;

typedef enum
{
	ACMP_LADDER_VREF_VDD = 0,
	ACMP_LADDER_VREF_VDDCMP
}ACMP_ladder_vref_sel_en;

static inline void ACMP_control_config(ACMP_edge_sel_en edge_sel, ACMP_output_control_en output_ctrl,
		ACMP_hysteresis_sel_en hyst_sel)
{
	ACMP->CTRL.EDGESEL = edge_sel;
	ACMP->CTRL.COMPSA = output_ctrl;
	ACMP->CTRL.HYS = hyst_sel;
}

static inline void ACMP_voltage_input_select(ACMP_input_voltage_sel_en in_positive,
		ACMP_input_voltage_sel_en in_negative)
{
	ACMP->CTRL.COMP_VP_SEL = in_positive;
	ACMP->CTRL.COMP_VM_SEL = in_negative;
}

static inline void ACMP_interrupt_clear(void)
{
	ACMP->CTRL.EDGECLR = 1;
	ACMP->CTRL.EDGECLR = 0;
}

static inline uint8_t ACMP_interrupt_flag_get(void)
{
	return ACMP->CTRL.EDGECLR;
}

static inline uint8_t ACMP_output_status_get(void)
{
	return ACMP->CTRL.COMPSTAT;
}

static inline void ACMP_interrupt_enable(void)
{
	ACMP->CTRL.INTENA = 1;
}

static inline void ACMP_interrupt_disable(void)
{
	ACMP->CTRL.INTENA = 0;
}

static inline uint8_t ACMP_edge_detected_get(void)
{
	return ACMP->CTRL.COMPEDGE;
}

static inline void ACMP_ladder_enable(void)
{
	ACMP->LAD.LADEN = 1;
}

static inline void ACMP_ladder_disable(void)
{
	ACMP->LAD.LADEN = 0;
}

static inline void ACMP_ladder_step_set(uint8_t ladder_step)
{
	ACMP->LAD.LADSEL = ladder_step;
}

static inline uint8_t ACMP_ladder_step_get(void)
{
	return ACMP->LAD.LADSEL;
}

static inline void ACMP_ladder_vref_select(ACMP_ladder_vref_sel_en vref)
{
	ACMP->LAD.LADREF = vref;
}

static inline ACMP_ladder_vref_sel_en ACMP_voltage_ladder_vref_get(void)
{
	return ACMP->LAD.LADREF;
}

#endif /* HPL_ANALOG_COMPARATOR_ */
