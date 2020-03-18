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

static void (*adc_seqa_completed_callback)(void) = NULL; //!< Callback cuando termina la secuencia A de conversiones
static void (*adc_seqb_completed_callback)(void) = NULL; //!< Callback cuando termina la secuencia B de conversiones
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
 *
 * Inicializa el ADC. Se ocupa de encender el periferico, clockear el mismo, la calibracion
 * de hardware, y fijar el clock del mismo para la frecuencia deseada.
 *
 * @param[in] adc_freq Frecuencia del ADC deseada
 * @param[in] clock_source Seleccion de clock para el ADC
 */
void ADC_init(uint32_t adc_freq, ADC_clock_source_en clock_source)
{
	ADC_CTRL_reg_t adc_ctrl_aux;
	uint32_t *aux_reg = (uint32_t *) &adc_ctrl_aux;
	uint64_t aux;
	uint64_t calib_aux;

	adc_freq *= 25;

	if(clock_source == ADC_CLOCK_SOURCE_FRO)
	{
		aux = SYSCON_get_fro_clock() / adc_freq;
		calib_aux = SYSCON_get_fro_clock() / 500000;
	}
	else
	{
		aux = SYSCON_get_pll_clock() / adc_freq;
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

	*((uint32_t *) &ADC->CTRL) = *aux_reg;

	// Poll el bit de calibracion hasta que la termine
	while(ADC->CTRL.CALMODE);

	// Una vez calibrado, configuro el clock deseado y queda listo para trabajar
	ADC->CTRL.CLKDIV = aux;
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
 * @brief Funcion de interrupcion cuando termina la secuencia de conversion B del ADC
 */
void ADC_SEQB_IRQHandler(void)
{
	if(adc_seqb_completed_callback != NULL)
	{
		adc_seqb_completed_callback();
	}

	// Limpio interrupcion de secuencia B completa
	ADC->FLAGS.SEQB_INT = 1;
}
