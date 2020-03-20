/**
 * @file HPL_ADC.h
 * @brief Declaraciones a nivel de abstraccion de periferico del ADC (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_ADC_H_
#define HPL_ADC_H_

#include <stdint.h>
#include <HRI_ADC.h>

#define MAX_ADC_CONVERSION_VALUE				0xFFF

typedef enum
{
	ADC_OPERATION_MODE_SYNCHRONOUS = 0,
	ADC_OPERATION_MODE_ASYNCHRONOUS
}ADC_operation_mode_en;

typedef enum
{
	ADC_LOW_POWER_MODE_DISABLED = 0,
	ADC_LOW_POWER_MODE_ENABLED
}ADC_low_power_mode_en;

typedef enum
{
	ADC_SEQUENCE_SEL_A = 0,
	ADC_SEQUENCE_SEL_B
}ADC_sequence_sel_en;

typedef enum
{
	ADC_TRIGGER_SEL_NONE = 0,
	ADC_TRIGGER_SEL_PININT0_IRQ,
	ADC_TRIGGER_SEL_PININT1_IRQ,
	ADC_TRIGGER_SEL_SCT0_OUT3,
	ADC_TRIGGER_SEL_SCT0_OUT4,
	ADC_TRIGGER_SEL_T0_MAT3,
	ADC_TRIGGER_SEL_CMP0_OUT_ADC,
	ADC_TRIGGER_SEL_GPIO_INT_BMAT,
	ADC_TRIGGER_SEL_ARM_TXEV
}ADC_trigger_sel_en;

typedef enum
{
	ADC_TRIGGER_POL_SEL_NEGATIVE_EDGE = 0,
	ADC_TRIGGER_POL_SEL_POSITIVE_EDGE
}ADC_trigger_pol_sel_en;

typedef enum
{
	ADC_SYNC_SEL_ENABLE_SYNC = 0,
	ADC_SYNC_SEL_BYPASS_SYNC
}ADC_sync_sel_en;

typedef enum
{
	ADC_CLOCK_SOURCE_FRO = 0,
	ADC_CLOCK_SOURCE_PLL
}ADC_clock_source_en;

typedef enum
{
	ADC_THRESHOLD_SEL_0 = 0,
	ADC_THRESHOLD_SEL_1
}ADC_threshold_sel_en;

typedef enum
{
	ADC_INTERRUPT_MODE_EOC = 0,
	ADC_INTERRUPT_MODE_EOS
}ADC_interrupt_mode_en;

typedef enum
{
	ADC_THRESHOLD_INTERRUPT_SEL_OUTSIDE = 1,
	ADC_THRESHOLD_INTERRUPT_SEL_CROSSING
}ADC_threshold_interrupt_sel_en;

typedef struct
{
	uint32_t : 4;
	uint32_t RESULT : 12;
	uint32_t THCMPRANGE : 2;
	uint32_t THCMPCROSS : 2;
	uint32_t : 6;
	uint32_t CHANNEL : 4;
	uint32_t OVERRUN : 1;
	uint32_t DATAVALID : 1;
}ADC_global_data_t;

typedef struct
{
	uint32_t : 4;
	uint32_t RESULT : 12;
	uint32_t THCMPRANGE : 2;
	uint32_t THCMPCROSS : 2;
	uint32_t : 6;
	uint32_t CHANNEL : 4;
	uint32_t OVERRUN : 1;
	uint32_t DATAVALID : 1;
}ADC_channel_data_t;

typedef enum
{
	ADC_VRANGE_HIGH_VOLTAGE = 0,
	ADC_VRANGE_LOW_VOLTAGE
}ADC_vrange_sel_en;

/**
 * @brief Configuracion del registro de control del ADC
 * @param[in] div Divisor deseado
 * @param[in] operation Modo de operacion sincronico/asincronico
 * @param[in] power Modo de consumo
 */
static inline void ADC_control_config(uint8_t div, ADC_operation_mode_en operation, ADC_low_power_mode_en power)
{
	ADC->CTRL.CLKDIV = div;
	ADC->CTRL.ASYNCMODE = operation;
	ADC->CTRL.LPWRMODE = power;
}

/**
 * @brief Configuracion de canales habilitados en una secuencia
 * @param[in] sequence Secuencia a configurar
 * @param[in] channels Canales a habilitar en forma de mascara
 */
static inline void ADC_sequence_config_channels(ADC_sequence_sel_en sequence, uint16_t channels)
{
	ADC->SEQ_CTRL[sequence].CHANNELS = channels;
}

/**
 * @brief Obtener los canales configuados en la secuencia
 * @param[in] sequence Secuencia a consultar
 * @return Mascara de canales habilitados en la secuencia
 */
static inline uint16_t ADC_sequence_get_channels(ADC_sequence_sel_en sequence)
{
	return ADC->SEQ_CTRL[sequence].CHANNELS;
}

/**
 * @brief Configuracion de trigger en una secuencia
 * @param[in] sequence Secuencia a configurar
 * @param[in] trigger Fuente de trigger deseada
 */
static inline void ADC_sequence_config_trigger(ADC_sequence_sel_en sequence, ADC_trigger_sel_en trigger)
{
	ADC->SEQ_CTRL[sequence].TRIGGER = trigger;
}

/**
 * @brief Configuracion de la polaridad del trigger en una secuencia
 * @param[in] sequence Secuencia a configurar
 * @param[in] pol Polaridad del trigger deseado
 */
static inline void ADC_sequence_config_trigger_pol(ADC_sequence_sel_en sequence, ADC_trigger_pol_sel_en pol)
{
	ADC->SEQ_CTRL[sequence].TRIGPOL = pol;
}

/**
 * @brief Configuracion de la sincronizacion en una secuencia
 * @param[in] sequence Secuencia a configurar
 * @param[in] sync Metodo de sincronizacion deseado
 */
static inline void ADC_sequence_config_sync(ADC_sequence_sel_en sequence, ADC_sync_sel_en sync)
{
	ADC->SEQ_CTRL[sequence].SYNCBYPASS = sync;
}

/**
 * @brief Iniciar conversiones por software en una secuencia
 * @param[in] sequence Secuencia a configurar
 */
static inline void ADC_sequence_set_start(ADC_sequence_sel_en sequence)
{
	ADC->SEQ_CTRL[sequence].START = 1;
}

/**
 * @brief Iniciar conversiones en rafaga en una secuencia
 * @param[in] sequence Secuencia a configurar
 */
static inline void ADC_sequence_set_burst(ADC_sequence_sel_en sequence)
{
	ADC_SEQ_CTRL_reg_t adc_sec_ctrl = ADC->SEQ_CTRL[sequence];

	adc_sec_ctrl.BURST = 1;
	adc_sec_ctrl.SEQ_ENA = 1;

	*((uint32_t *) &ADC->SEQ_CTRL[sequence]) = *((uint32_t *) &adc_sec_ctrl);
}

/**
 * @brief Detener conversiones en rafaga en una secuencia
 * @param[in] sequence Secuencia a configurar
 */
static inline void ADC_sequence_clear_burst(ADC_sequence_sel_en sequence)
{
	ADC->SEQ_CTRL[sequence].BURST = 0;
	ADC->SEQ_CTRL[sequence].SEQ_ENA = 0;
}

/**
 * @brief Fijar modo singlestep
 * @param[in] sequence Secuencia a configurar
 */
static inline void ADC_sequence_set_singlestep(ADC_sequence_sel_en sequence)
{
	ADC->SEQ_CTRL[sequence].SINGLESTEP = 1;
}

/**
 * @brief Limpiar modo singlestep
 * @param[in] sequence Secuencia a configurar
 */
static inline void ADC_sequence_clear_singlestep(ADC_sequence_sel_en sequence)
{
	ADC->SEQ_CTRL[sequence].SINGLESTEP = 0;
}

/**
 * @brief Fijar baja prioridad para la secuencia A
 */
static inline void ADC_sequence_A_lowpriority_set(void)
{
	ADC->SEQ_CTRL[ADC_SEQUENCE_SEL_A].LOWPRIO = 0;
}

/**
 * @brief Fijar baja prioridad para la secuencia A
 */
static inline void ADC_sequence_A_lowpriority_clear(void)
{
	ADC->SEQ_CTRL[ADC_SEQUENCE_SEL_A].LOWPRIO = 1;
}

/**
 * @brief Configurar modo de interrupcion para una secuencia
 * @param[in] sequence Que secuencia configurar
 * @param[in] mode Modo de interrupcion
 */
static inline void ADC_sequence_config_interrupt_mode(ADC_sequence_sel_en sequence, ADC_interrupt_mode_en mode)
{
	ADC->SEQ_CTRL[sequence].MODE = mode;
}

/**
 * @brief Obtener modo de interrupcion de una secuencia
 * @param[in] sequence Secuencia a consultar
 * @return Modo de interrupcion de la secuencia
 */
static inline ADC_interrupt_mode_en ADC_sequence_get_mode(ADC_sequence_sel_en sequence)
{
	return ADC->SEQ_CTRL[sequence].MODE;
}

/**
 * @brief Habilitar secuencia
 * @param[in] sequence Que secuencia habilitar
 */
static inline void ADC_sequence_enable(ADC_sequence_sel_en sequence)
{
	ADC->SEQ_CTRL[sequence].SEQ_ENA = 1;
}

/**
 * @brief Inhabilitar secuencia
 * @param[in] sequence Que secuencia inhabilitar
 */
static inline void ADC_sequence_disable(ADC_sequence_sel_en sequence)
{
	ADC->SEQ_CTRL[sequence].SEQ_ENA = 0;
}

/**
 * @brief Fijar valor de umbral de comparacion (parte baja)
 * @param[in] threshold_selection Que threshold fijar
 * @param[in] threshold_value Valor de threshold a fijar
 */
static inline void ADC_set_compare_low_threshold(ADC_threshold_sel_en threshold_selection, uint16_t threshold_value)
{
	ADC->THR_LOW[threshold_selection].THRLOW = threshold_value;
}

/**
 * @brief Fijar valor de umbral de comparacion (parte alta)
 * @param[in] threshold_selection Que threshold fijar
 * @param[in] threshold_value Valor de threshold a fijar
 */
static inline void ADC_set_compare_high_threshold(ADC_threshold_sel_en threshold_selection, uint16_t threshold_value)
{
	ADC->THR_HIGH[threshold_selection].THRHIGH = threshold_value;
}

/**
 * @brief Fijar contra que umbral de comparacion se compara el canal
 * @param[in] channel Numero de canal
 * @param[in] threshold_selection Contra que umbral de comparacion comparar
 */
static inline void ADC_set_channel_threshold(uint8_t channel, ADC_threshold_sel_en threshold_selection)
{
	switch(channel)
	{
	case 0: { ADC->CHAN_THRSEL.CH0_THRSEL = threshold_selection; break; }
	case 1: { ADC->CHAN_THRSEL.CH1_THRSEL = threshold_selection; break; }
	case 2: { ADC->CHAN_THRSEL.CH2_THRSEL = threshold_selection; break; }
	case 3: { ADC->CHAN_THRSEL.CH3_THRSEL = threshold_selection; break; }
	case 4: { ADC->CHAN_THRSEL.CH4_THRSEL = threshold_selection; break; }
	case 5: { ADC->CHAN_THRSEL.CH5_THRSEL = threshold_selection; break; }
	case 6: { ADC->CHAN_THRSEL.CH6_THRSEL = threshold_selection; break; }
	case 7: { ADC->CHAN_THRSEL.CH7_THRSEL = threshold_selection; break; }
	case 8: { ADC->CHAN_THRSEL.CH8_THRSEL = threshold_selection; break; }
	case 9: { ADC->CHAN_THRSEL.CH9_THRSEL = threshold_selection; break; }
	case 10: { ADC->CHAN_THRSEL.CH10_THRSEL = threshold_selection; break; }
	case 11: { ADC->CHAN_THRSEL.CH11_THRSEL = threshold_selection; break; }
	}
}

/**
 * @brief Habilitacion de interrupcion de secuencia
 * @param[in] sequence Sobre que secuencia habilitar la interrupcion
 */
static inline void ADC_enable_sequence_interrupt(ADC_sequence_sel_en sequence)
{
	if(sequence == ADC_SEQUENCE_SEL_A)
	{
		ADC->INTEN.SEQA_INTEN = 1;
	}
	else
	{
		ADC->INTEN.SEQB_INTEN = 1;
	}
}

/**
 * @brief Inhabilitacion de interrupcion de secuencia
 * @param[in] sequence Sobre que secuencia inhabilitar la interrupcion
 */
static inline void ADC_disable_sequence_interrupt(ADC_sequence_sel_en sequence)
{
	if(sequence == ADC_SEQUENCE_SEL_A)
	{
		ADC->INTEN.SEQA_INTEN = 0;
	}
	else
	{
		ADC->INTEN.SEQB_INTEN = 0;
	}
}

/**
 * @brief Habilitacion de interrupcion de overrun
 */
static inline void ADC_enable_overrun_interrupt(void)
{
	ADC->INTEN.OVR_INTEN = 1;
}

/**
 * @brief Inhabilitacion de interrupcion de overrun
 */
static inline void ADC_disable_overrun_interrupt(void)
{
	ADC->INTEN.OVR_INTEN = 0;
}

/**
 * @brief Habilitacion de interrupcion de threshold
 * @param[in] channel Sobre que canal habilitar la interrupcion
 * @param[in] mode Modo de interrupcion
 */
static inline void ADC_enable_threshold_interrupt(uint8_t channel, ADC_threshold_interrupt_sel_en mode)
{
	switch(channel)
	{
	case 0: { ADC->INTEN.ADCMPINTEN0 = mode; break; }
	case 1: { ADC->INTEN.ADCMPINTEN1 = mode; break; }
	case 2: { ADC->INTEN.ADCMPINTEN2 = mode; break; }
	case 3: { ADC->INTEN.ADCMPINTEN3 = mode; break; }
	case 4: { ADC->INTEN.ADCMPINTEN4 = mode; break; }
	case 5: { ADC->INTEN.ADCMPINTEN5 = mode; break; }
	case 6: { ADC->INTEN.ADCMPINTEN6 = mode; break; }
	case 7: { ADC->INTEN.ADCMPINTEN7 = mode; break; }
	case 8: { ADC->INTEN.ADCMPINTEN8 = mode; break; }
	case 9: { ADC->INTEN.ADCMPINTEN9 = mode; break; }
	case 10: { ADC->INTEN.ADCMPINTEN10 = mode; break; }
	case 11: { ADC->INTEN.ADCMPINTEN11 = mode; break; }
	}
}

/**
 * @brief Inhabilitacion de interrupcion de threshold
 * @param[in] channel Sobre que canal inhabilitar la interrupcion
 */
static inline void ADC_disable_threshold_interrupt(uint8_t channel)
{
	switch(channel)
	{
	case 0: { ADC->INTEN.ADCMPINTEN0 = 0; break; }
	case 1: { ADC->INTEN.ADCMPINTEN1 = 0; break; }
	case 2: { ADC->INTEN.ADCMPINTEN2 = 0; break; }
	case 3: { ADC->INTEN.ADCMPINTEN3 = 0; break; }
	case 4: { ADC->INTEN.ADCMPINTEN4 = 0; break; }
	case 5: { ADC->INTEN.ADCMPINTEN5 = 0; break; }
	case 6: { ADC->INTEN.ADCMPINTEN6 = 0; break; }
	case 7: { ADC->INTEN.ADCMPINTEN7 = 0; break; }
	case 8: { ADC->INTEN.ADCMPINTEN8 = 0; break; }
	case 9: { ADC->INTEN.ADCMPINTEN9 = 0; break; }
	case 10: { ADC->INTEN.ADCMPINTEN10 = 0; break; }
	case 11: { ADC->INTEN.ADCMPINTEN11 = 0; break; }
	}
}

/**
 * @brief Leer alguno de los registros globales de resultado de conversion
 * @param[in] sequence De que secuencia leer el resultado global de conversion
 * @return Contenido del registro
 */
static inline ADC_global_data_t ADC_get_global_data(ADC_sequence_sel_en sequence)
{
	return *((ADC_global_data_t *) &ADC->SEQ_GDAT[sequence]);
}

/**
 * @brief Leer alguno de los registros de canal de resultado de conversion
 * @param[in] channel De que canal leer el resultado de canal de conversion
 * @return Contenido del registro
 */
static inline ADC_channel_data_t ADC_get_channel_data(uint8_t channel)
{
	return *((ADC_channel_data_t *) &ADC->DAT[channel]);
}

/**
 * @brief Configuracion de rango de tension
 * @param[in] vrange Rango de tension de trabajo
 */
static inline void ADC_set_vrange(ADC_vrange_sel_en vrange)
{
	ADC->TRM.VRANGE = vrange;
}

/**
 * @brief Realizacion de calibracion de hardware
 * @param[in] div Divisor utilizado para lograr los 500KHz de clock
 */
static inline void ADC_hardware_calib(uint8_t div)
{
	ADC_CTRL_reg_t adc_ctrl_original = ADC->CTRL;
	ADC_CTRL_reg_t adc_ctrl_aux = ADC->CTRL;

	adc_ctrl_aux.CLKDIV = div;
	adc_ctrl_aux.CALMODE = 1;
	adc_ctrl_aux.LPWRMODE = 0;

	*((uint32_t *) &ADC->CTRL) = *((uint32_t *) &adc_ctrl_aux);

	while(ADC->CTRL.CALMODE);

	*((uint32_t *) &ADC->CTRL) = *((uint32_t *) &adc_ctrl_original);
}

#endif /* HPL_ADC_H_ */
