/**
 * @file Ejemplo_ADC.c
 * @brief Ejemplo de utilización del \e ADC con la librería (capa de aplicación)
 *
 * El programa utiliza el clock por default con el que comienza el microcontrolador, es decir, el <em>Free Running
 * Oscillator</em> funcionando a \f$ 12MHz \f$.
 *
 * El periférico será configurado con las siguientes características:
 * 	- Funcionamiento \b sincrónico
 * 	- Frecuencia de muestreo de \f$ 1Mhz \f$
 * 	- Modo bajo consumo inhabilitado
 *  .
 *
 * La secuencia A es configurada para generar conversiones en los canales 0 y 8:
 * 	- El canal 0 está conectado al preset propio del stick de desarrollo (Puerto 0 pin 7)
 * 	- El canal 8 está ubicado en el pin número 3 (Puerto 0 pin 18) y se le puede conectar un preset externo entre
 * 	 VDD y GND.
 * 	.
 *
 * Además, la secuencia tendrá la siguiente configuración:
 * 	- Trigger: Únicamente se disparan conversiones por software
 * 	- Bypass sincronismo: Sí
 * 	- Modo de interrupción: Cuando termina la secuencia completa
 * 	- Burst: Inhabilitado
 * 	- Un trigger dispara: Una conversión de secuencia completa
 * 	- Secuencia A como baja prioridad: No
 *
 * Una vez inicializado el periférico, se configura el periférico \e Systick para interrumpir cada \f$ 1mseg \f$
 * y mediante su manejador se lleva la cuenta de los milisegundos transcurridos. Una vez transcurridos
 * \f$ 1000mseg \f$, se dispara una conversión de \e ADC, y sus resultados se guardan en dos variables globales.
 *
 * Ubicando un \e breakpoint adecuadamente, se pueden leer los resultados de las conversiones ya ubicadas en
 * las variables globales.
 *
 * @author Augusto Santini
 * @date 4/2020
 */

#include <cr_section_macros.h>
#include <stddef.h>
#include <HAL_ADC.h>
#include <HAL_SYSTICK.h>

/** Máscara de configuración de canales habilitados para la secuencia a configurar */
#define		ADC_CHANNELS				((1 << 0) | (1 << 8))

/** Tiempo de interrupción del \e Systick en \b microsegundos */
#define		TICK_TIME_USEG				(1000)

/** Frecuencia de muestreo a utilizar por el ADC */
#define		ADC_SAMPLE_FREQ				(1000000)

/** Secuencia a utilizar en el ADC */
#define		ADC_SEQUENCE				(HAL_ADC_SEQUENCE_SEL_A)

/** Tiempo de disparo de conversiones de \e ADC en \b milisegundos */
#define		ADC_CONVERSION_TIME_MSEG	(1000)

static void adc_callback(void);

static void systick_callback(void);

/** Flag para indicar finalización de secuencia de conversión de \e ADC */
static uint8_t flag_secuencia_adc_completada = 0;

/**
 * Variables para guardar los resultados de la secuencia de conversión
 */
static hal_adc_sequence_result_t resultados_conversion_adc[2];

/** Configuración de la secuencia. Como no va a cambiar es declarada \e const */
static const hal_adc_sequence_config_t adc_config =
{
	.channels = ADC_CHANNELS,
	.trigger = HAL_ADC_TRIGGER_SEL_NONE,
	.trigger_pol = HAL_ADC_TRIGGER_POL_SEL_NEGATIVE_EDGE,
	.sync_bypass = HAL_ADC_SYNC_SEL_BYPASS_SYNC,
	.mode = HAL_ADC_INTERRUPT_MODE_EOS,
	.burst = 0,
	.single_step = 0,
	.low_priority = 0,
	.callback = adc_callback
};

/**
 * @brief Punto de entrada del programa
 * @return Nunca deberia terminar esta función
 */
int main(void)
{
	// Inicialización del periférico en modo SINCRÓNICO
	hal_adc_init_sync_mode(ADC_SAMPLE_FREQ, HAL_ADC_LOW_POWER_MODE_DISABLED);

	// Configuración de la secuencia a utilizar
	hal_adc_config_sequence(ADC_SEQUENCE, &adc_config);

	// Habilitación de la secuencia a utilizar
	hal_adc_start_sequence(ADC_SEQUENCE);

	// Inicialización del \e Systick con el tiempo de tick adecuado
	hal_systick_init(TICK_TIME_USEG, systick_callback);

    while(1)
    {
    	if(flag_secuencia_adc_completada == 1)
    	{
    		uint32_t variable_auxiliar;

    		flag_secuencia_adc_completada = 0;

    		(void) variable_auxiliar; // Esta línea es ideal para colocar el breakpoint!
    	}
    }

    return 0;
}

/**
 * @brief Callback a ejecutar en cada tick del \e Systick
 */
static void systick_callback(void)
{
	static uint32_t contador_disparo_adc = 0;

	// Conteo con valor límite
	contador_disparo_adc = (contador_disparo_adc + 1) % ADC_CONVERSION_TIME_MSEG;

	if(contador_disparo_adc == 0)
	{
		hal_adc_start_sequence(ADC_SEQUENCE);
	}
}

/**
 * @brief Callback a ejecutar en cada finalización de conversión de \b secuencia de \e ADC
 */
static void adc_callback(void)
{
	// Obtención de resultados de conversión
	hal_adc_get_sequence_result(ADC_SEQUENCE, resultados_conversion_adc);

	flag_secuencia_adc_completada = 1;
}
