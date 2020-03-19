/**
 * @file HPL_ADC.c
 * @brief Funciones a nivel de abstraccion de periferico para el ADC (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdio.h>
#include <stdint.h>
#include <HPL_ADC.h>
#include <HPL_SYSCON.h>
#include <HRI_ADC.h>
#include <HRI_SYSCON.h>
#include <HRI_NVIC.h>
#include <HRI_SWM.h>

#define	ADC_MAX_FREQ			1.2e6 //<! Maxima frecuencia de conversion admitida por el ADC
#define	ADC_COMPARE_AMOUNT		12 //!< Cantidad de canales de comparacion admitidos por el ADC

static void dummy_irq_callback(void);

static void (*adc_seq_completed_callback[2])(void) = //!< Callback cuando terminan las secuencias de conversion
{
	dummy_irq_callback,
	dummy_irq_callback
};

static void (*adc_overrun_callback)(void) = dummy_irq_callback; //!< Callback cuando ocurre un overrun

static void (*adc_compare_callback)(void) = dummy_irq_callback; //!< Callbacks para las comparaciones de ADC

/**
 * @brief Inicializacion del ADC
 * @param[in] adc_config Configuracion del ADC deseada
 */
void ADC_config_control(const ADC_config_t *adc_config)
{
	ADC_CTRL_reg_t adc_ctrl_aux;
	uint32_t calib_aux;

	if(clock_source == ADC_CLOCK_SOURCE_FRO)
	{
		calib_aux = SYSCON_get_fro_clock() / 500000;
	}
	else
	{
		calib_aux = SYSCON_get_pll_clock() / 500000;
	}

	// Encendido del ADC
	SYSCON->PDRUNCFG.ADC_PD = 0;

	// Seleccion de fuente de clock del ADC
	SYSCON->ADCCLKSEL.SEL = clock_source;

	// Habilitacion del clock del ADC
	SYSCON->SYSAHBCLKCTRL0.ADC = 1;

	// Division del clock del ADC
	SYSCON->ADCCLKDIV.DIV = 1;

	adc_ctrl_aux = ADC->CTRL;

	// Para la calibracion, en una unica escritura hago lo siguiente:
	adc_ctrl_aux.CLKDIV = calib_aux;
	adc_ctrl_aux.CALMODE = 1;
	adc_ctrl_aux.LPWRMODE = 0;

	*((uint32_t *) &ADC->CTRL) = *((uint32_t *) &adc_ctrl_aux);

	// Poll el bit de calibracion hasta que la termine
	while(ADC->CTRL.CALMODE);

	ADC->TRM.VRANGE = adc_config->voltage_range;
	ADC->CTRL.ASYNCMODE = adc_config->async_mode;
	ADC->CTRL.LPWRMODE = adc_config->low_power_mode;
	ADC->CTRL.CLKDIV = adc_config->clock_div;
}

/**
 * @brief Configuracion de una secuencia del ADC
 * @param[in] sequence Que secuencia configurar
 * @param[in] config Configuracion deseada para la secuencia
 */
void ADC_config_sequence(ADC_sequence_sel_en sequence, const ADC_sequence_config_t *config)
{
	ADC->SEQ_CTRL[sequence].SEQ_ENA = 0;
	ADC->SEQ_CTRL[sequence].CHANNELS = config->channels;
	ADC->SEQ_CTRL[sequence].TRIGGER = config->trigger;
	ADC->SEQ_CTRL[sequence].TRIGPOL = config->trigger_polarity;
	ADC->SEQ_CTRL[sequence].SYNCBYPASS = config->sync_bypass;
	ADC->SEQ_CTRL[sequence].SINGLESTEP = config->single_step;

	if(sequence == ADC_SEQUENCE_SEL_A)
	{
		ADC->SEQ_CTRL[sequence].LOWPRIO = config->low_priority;
	}

	ADC->SEQ_CTRL[sequence].MODE = config->interrupt_mode;
}

/**
 * @brief Iniciar conversiones en modo burst en una secuencia
 * @param[in] sequence En que secuencia iniciar conversiones en modo rafaga
 */
void ADC_start_burst_in_sequence(ADC_sequence_sel_en sequence)
{
	ADC_SEQ_CTRL_reg_t adc_sec_ctrl = ADC->SEQ_CTRL[sequence];

	adc_sec_ctrl.BURST = 1;
	adc_sec_ctrl.SEQ_ENA = 1;

	*((uint32_t *) &ADC->SEQ_CTRL[sequence]) = *((uint32_t *) &adc_sec_ctrl);
}

/**
 * @brief Detener conversiones en modo burst en una secuencia
 * @param[in] sequence En que secuencia detener conversiones en modo rafaga
 */
void ADC_stop_burst_in_sequence(ADC_sequence_sel_en sequence)
{
	ADC->SEQ_CTRL[sequence].BURST = 0;
	ADC->SEQ_CTRL[sequence].SEQ_ENA = 0;
}

/**
 * @brief Habilitar secuencia
 * @param[in] sequence Que secuencia habilitar
 */
void ADC_enable_sequence(ADC_sequence_sel_en sequence)
{
	ADC->SEQ_CTRL[sequence].SEQ_ENA = 1;
}

/**
 * @brief Inhabilitar secuencia
 * @param[in] sequence Que secuencia inhabilitar
 */
void ADC_disable_sequence(ADC_sequence_sel_en sequence)
{
	ADC->SEQ_CTRL[sequence].SEQ_ENA = 0;
}

/**
 * @brief Generar un software start en una secuencia
 * @param[in] sequence Sobre que secuencia generar el software start
 */
void ADC_software_start_sequence(ADC_sequence_sel_en sequence)
{
	ADC->SEQ_CTRL[sequence].START = 1;
}

/**
 * @brief Fijar valor de umbral de comparacion (parte baja)
 * @param[in] threshold_selection Que threshold fijar
 * @param[in] threshold_value Valor de threshold a fijar
 */
void ADC_set_compare_low_threshold(ADC_threshold_sel_en threshold_selection, uint16_t threshold_value)
{
	ADC->THR_LOW[threshold_selection].THRLOW = threshold_value;
}

/**
 * @brief Fijar valor de umbral de comparacion (parte alta)
 * @param[in] threshold_selection Que threshold fijar
 * @param[in] threshold_value Valor de threshold a fijar
 */
void ADC_set_compare_high_threshold(ADC_threshold_sel_en threshold_selection, uint16_t threshold_value)
{
	ADC->THR_HIGH[threshold_selection].THRHIGH = threshold_value;
}

/**
 * @brief Fijar contra que umbral de comparacion se compara el canal
 * @param[in] channel Numero de canal
 * @param[in] threshold_selection Contra que umbral de comparacion comparar
 */
void ADC_set_channel_threshold(uint8_t channel, ADC_threshold_sel_en threshold_selection)
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
void ADC_enable_sequence_interrupt(ADC_sequence_sel_en sequence)
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
void ADC_disable_sequence_interrupt(ADC_sequence_sel_en sequence)
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
 * @brief Configurar un callback para cuando se genera una interrupcion de secuencia
 * @param[in] sequence Sobre que secuencia configurar el callback
 * @param[in] callback Puntero a la funcion a ejecutar cuando ocurra la interrupcion
 */
void ADC_set_sequence_callback(ADC_sequence_sel_en sequence, void (*callback)(void))
{
	adc_seq_completed_callback[sequence] = callback;
}

/**
 * @brief Habilitacion de interrupcion de overrun
 */
void ADC_enable_overrun_interrupt(void)
{
	ADC->INTEN.OVR_INTEN = 1;
}

/**
 * @brief Inhabilitacion de interrupcion de overrun
 */
void ADC_disable_overrun_interrupt(void)
{
	ADC->INTEN.OVR_INTEN = 0;
}

/**
 * @brief Configurar un callback para cuando se genera una interrupcion de overrun
 * @param[in] callback Puntero a la funcion a ejecutar cuando ocurra la interrupcion
 */
void ADC_set_overrun_callback(void (*callback)(void))
{
	adc_overrun_callback = callback;
}

/**
 * @brief Habilitacion de interrupcion de threshold
 * @param[in] channel Sobre que canal habilitar la interrupcion
 * @param[in] mode Modo de interrupcion
 */
void ADC_enable_threshold_interrupt(uint8_t channel, ADC_threshold_interrupt_sel_en mode)
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
void ADC_disable_threshold_interrupt(uint8_t channel)
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
 * @brief Configurar un callback para cuando se genera una interrupcion de threshold
 * @param[in] callback Puntero a la funcion a ejecutar cuando ocurra la interrupcion
 */
void ADC_set_threshold_callback(void (*callback)(void))
{
	adc_compare_callback = callback;
}

/**
 * @brief Leer alguno de los registros globales de resultado de conversion
 * @param[in] seequence De que secuencia leer el resultado global de conversion
 * @param[out] data Contenido del registro
 */
void ADC_get_global_data(ADC_sequence_sel_en sequence, ADC_global_data_t *data)
{
	*data = *((ADC_global_data_t *) &ADC->SEQ_GDAT[sequence]);
}

/**
 * @brief Leer alguno de los registros de canal de resultado de conversion
 * @param[in] channel De que canal leer el resultado de canal de conversion
 * @param[out] data Contenido del registro
 */
void ADC_get_channel_data(uint8_t channel, ADC_channel_data_t *data)
{
	*data = *((ADC_channel_data_t *) &ADC->DAT[channel]);
}

/**
 * @brief Funcion dummy para usar como default para las interrupciones
 */
static void dummy_irq_callback(void)
{
	return;
}

/**
 * @brief Funcion de interrupcion cuando termina la secuencia de conversion A del ADC
 */
void ADC_SEQA_IRQHandler(void)
{
	adc_seq_completed_callback[ADC_SEQUENCE_SEL_A]();

	if(ADC->SEQ_CTRL[ADC_SEQUENCE_SEL_A].MODE == 1)
	{
		ADC->FLAGS.SEQA_INT = 1;
	}
}

/**
 * @brief Funcion de interrupcion cuando termina la secuencia de conversion B del ADC
 */
void ADC_SEQB_IRQHandler(void)
{
	adc_seq_completed_callback[ADC_SEQUENCE_SEL_B]();

	if(ADC->SEQ_CTRL[ADC_SEQUENCE_SEL_B].MODE == 1)
	{
		ADC->FLAGS.SEQB_INT = 1;
	}
}

/**
 * @brief Funcion de interrupcion cuando se detecta alguna de las condiciones de threshold establecidas
 */
void ADC_THCMP_IRQHandler(void)
{
	adc_compare_callback();
}

/**
 * @brief Funcion de interrupcion cuando se detecta alguna de las condiciones de overrun
 */
void ADC_OVR_IRQHandler(void)
{
	adc_overrun_callback();
}
