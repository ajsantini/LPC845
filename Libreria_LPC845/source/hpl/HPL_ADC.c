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

volatile ADC_per_t * const ADC = (ADC_per_t *) ADC_BASE; //!< Periferico ADC

static void (*adc_seq_completed_callback[2])(void) = //!< Callback cuando terminan las secuencias de conversion
{
	NULL, NULL
};

static void (*adc_overrun_callback)(void) = NULL; //!< Callback cuando ocurre un overrun
static void (*adc_compare_callback[ADC_COMPARE_AMOUNT])(void) = //!< Callbacks para las comparaciones de ADC
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

/**
 * @brief Inicializacion del ADC
 * @param[in] clock_source Seleccion de clock para el ADC
 * @param[in] adc_config Configuracion del ADC deseada
 */
void ADC_init(ADC_clock_source_en clock_source, const ADC_config_t *adc_config)
{
	ADC_CTRL_reg_t adc_ctrl_aux;
	uint32_t aux;
	uint32_t calib_aux;

	adc_freq *= 25;

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

	*((uint32_t *) ADC->SEQ_CTRL[sequence]) = *((uint32_t *) &adc_sec_ctrl);
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
 * @brief Configurar un callback para cuando se genera una interrupcion de secuencia
 * @param[in] sequence Sobre que secuencia configurar el callback
 * @param[in] callback Puntero a la funcion a ejecutar cuando ocurra la interrupcion
 */
void ADC_set_sequence_callback(ADC_sequence_sel_en sequence, void (*callback)(void))
{
	adc_seq_completed_callback[sequence] = callback;
}

/**
 * @brief Configuracion de las conversiones del ADC
 *
 * Configura el ADC con las siguientes salvedades:
 * -) Solo se permiten conversiones disparadas por software
 * -) Un disparo de conversion, convertira todos los canales habilitados
 * -) Las interrupciones seran una vez que termine la conversion de toda la secuencia
 *
 * @param[in] conversions_config Configuracion deseada para las conversiones
 */
void ADC_config_conversions(const ADC_conversions_config_t * const conversions_config)
{
	uint32_t counter;

	// Habilitacion del clock de la Switch Matrix
	SYSCON->SYSAHBCLKCTRL0.SWM = 1;

	// Configuracion de pines necesarios de ADC en la Switch-Matrix
	for(counter = 0; counter < 12; counter++)
	{
		if(conversions_config->channels & (1 << counter))
		{
			switch(counter)
			{
			case 0:
				SWM->PINENABLE0.ADC_0 = 0;
				break;
			case 1:
				SWM->PINENABLE0.ADC_1 = 0;
				break;
			case 2:
				SWM->PINENABLE0.ADC_2 = 0;
				break;
			case 3:
				SWM->PINENABLE0.ADC_3 = 0;
				break;
			case 4:
				SWM->PINENABLE0.ADC_4 = 0;
				break;
			case 5:
				SWM->PINENABLE0.ADC_5 = 0;
				break;
			case 6:
				SWM->PINENABLE0.ADC_6 = 0;
				break;
			case 7:
				SWM->PINENABLE0.ADC_7 = 0;
				break;
			case 8:
				SWM->PINENABLE0.ADC_8 = 0;
				break;
			case 9:
				SWM->PINENABLE0.ADC_9 = 0;
				break;
			case 10:
				SWM->PINENABLE0.ADC_10 = 0;
				break;
			case 11:
				SWM->PINENABLE0.ADC_11 = 0;
				break;
			default: break;
			} // End switch(counter)
		}
	}

	// Inhabilitacion del clock de la Switch Matrix
	SYSCON->SYSAHBCLKCTRL0.SWM = 0;

	// Solo conversiones por software, deshabilito las de hardware
	ADC->SEQB_CTRL.TRIGGER = 0;

	// Canales a convertir
	ADC->SEQB_CTRL.CHANNELS = conversions_config->channels;

	// Polaridad del START
	ADC->SEQB_CTRL.TRIGPOL = 1;

	// Un start de conversion, convierte todos los canales habilitados directamente
	ADC->SEQB_CTRL.SINGLESTEP = 0;

	// Interrupciones al final de cada secuencia
	ADC->SEQB_CTRL.MODE = 1;
	ADC->INTEN.SEQB_INTEN = 1;

	// Configuro el callback correspondiente y habilito/inhabilito interrupciones en NVIC
	ADC_register_callback(conversions_config->conversion_ended_callback);

	// Habilitacion de la secuencia
	if(conversions_config->burst)
	{
		// Si se configura por burst, el bit BURST y ENABLE deben ser escritos a la vez
		ADC_SEQB_CTRL_reg_t aux;
		uint32_t *aux1 = (uint32_t *) &aux;

		aux.BURST = 1;
		aux.SEQB_ENA = 1;

		// Despues de esta linea ya comienzan las conversiones
		*((uint32_t *) &ADC->SEQB_CTRL) = *aux1;
	}
	else
	{
		// Si no es por burst, simplemente habilito la secuencia
		ADC->SEQB_CTRL.SEQB_ENA = 1;

		// Para iniciar una conversion, hay que llamar a la funcion:
		// ADC_start_conversions()
	}
}

/**
 * @brief Iniciar conversiones de ADC
 *
 * El ADC debe haber sido previamente configurado correctamente.
 */
void ADC_start_conversions(void)
{
	if(!ADC->SEQB_CTRL.BURST)
	{
		// Solo disparo con el start si no esta en modo BURST
		ADC->SEQB_CTRL.START = 1;
	}
}

/**
 * @brief Obtener resultado de conversiones de ADC de un canal en particular
 * @param[in] channel Canal del cual obtener el resultado
 * @param[out] conversion Resultado de la conversion de ADC del canal asociado
 */
void ADC_get_conversion(uint8_t channel, uint32_t *conversion)
{
	if(ADC->DAT[channel].DATAVALID)
	{
		*conversion = ADC->DAT[channel].RESULT;
	}
}

/**
 * @brief Registrar un callback a llamar en la funcion de interrupcion cuando termina la secuencia de conversiones
 * @param[in] new_callback Puntero a funcion a llamar una vez terminada la secuencia de conversiones
 */
void ADC_register_callback(void (*new_callback)(void))
{
	adc_seqb_completed_callback = new_callback;

	if(new_callback != NULL)
	{
		// Habilito interrupciones en el NVIC
		NVIC->ISER0.ISE_ADC_SEQB = 1;
	}
	else
	{
		// Inhabilito interrupciones en el NVIC
		NVIC->ICER0.ICE_ADC_SEQB = 1;
	}
}

/**
 * @brief Funcion de interrupcion cuando termina la secuencia de conversion A del ADC
 */
void ADC_SEQA_IRQHandler(void)
{
	if(adc_seqb_completed_callback != NULL)
	{
		adc_seq_completed_callback[ADC_SEQUENCE_SEL_A]();
	}

	// Limpio interrupcion de secuencia A completa
	ADC->FLAGS.SEQA_INT = 1;
}

/**
 * @brief Funcion de interrupcion cuando termina la secuencia de conversion B del ADC
 */
void ADC_SEQB_IRQHandler(void)
{
	if(adc_seqb_completed_callback != NULL)
	{
		adc_seq_completed_callback[ADC_SEQUENCE_SEL_B]();
	}

	// Limpio interrupcion de secuencia B completa
	ADC->FLAGS.SEQB_INT = 1;
}

/**
 * @brief Funcion de interrupcion cuando se detecta la condicion de threshold establecida
 */
void ADC_THCMP_IRQHandler(void)
{

}

/**
 * @brief Funcion de interrupcion cuando se detecta una condicion de OVERRUN
 */
void ADC_OVR_IRQHandler(void)
{

}
