/**
 * @file HAL_ADC.h
 * @brief Declaraciones a nivel de aplicacion del periferico ADC (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @example Ejemplo_ADC.c
 * @brief Ejemplo sobre la utilización del \e ADC.
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
 */

/**
 * @defgroup ADC Conversor analógico a digital (ADC)
 *
 * # Descripción
 *
 * Este periférico como su nombre lo indica, convierte una o más entradas analógicas, a un
 * valor equivalente digital. En el caso del LPC845, tiene un único módulo \e ADC con una
 * resolución de 12 bits, el cual tiene 12 canales, lo cual implica que se pueden realizar
 * conversiones de 12 fuentes analógicas distintas, pero no así realizar conversiones
 * <em> al mismo tiempo </em>. En caso de querer tomar señales de múltiples fuentes analógicas, se
 * deberán hacer sucesivas conversiones en los distintos canales deseados.
 *
 * Una resolución de 12 bits implica que la conversión aumentará cada unidad siguiendo la siguiente ecuación:
 * \f$ ADC_{res} = \frac{V_{ref_{p}}}{2^N} \f$
 *
 * Esto implica que podemos preveer el valor resultante de la conversión analógica/digital mediante la
 * siguiente ecuación: \f$ ADC_{conv} = \frac{V_{ADC_{in}}}{ADC_{res}} \f$
 *
 * Cabe destacar, que las conversiones serán redondeadas \b siempre hacia abajo, es decir, se descartan los
 * valores decimales.
 *
 * ## Concepto de <em>Secuencia de conversión</em>
 *
 * Para el \e ADC de este microcontrolador, un inicio de conversión en realidad puede implicar el inicio de
 * una <em>secuencia de conversión</em>. Dicha secuencia puede implicar uno o más canales a convertir, y puede
 * generar eventos tanto cuando se termina la secuencia completa, o cuando se termina cada canal de la
 * secuencia. Asimismo los inicios de conversión pueden disparar una secuencia completa, o el próximo de los
 * canales de dicha secuencia. Se tienen dos secuencias configurables (<em>Secuencia A y Secuencia B</em>),
 * las cuales se pueden configurar de forma tal que una secuencia interrumpa a la otra.
 *
 * ## Inicio de conversiones
 *
 * El \e ADC de este microcontrolador permite el inicio de secuencia de conversión/canal de dos formas:
 * 		1. Iniciadas por software: Las secuencias de conversión son iniciadas mediante código.
 * 		2. Iniciadas por hardware: Las secuencias de conversión son iniciadas dependiendo de otras señales,
 * 		sean las mismas internas o externas al microcontrolador.
 * 		.
 *
 * ## Calibración de hardware
 *
 * Este periférico contiene un bloque de autocalibración, el cual debe ser utilizado luego de cada reinicio
 * del microcontrolador o cada vez que se sale de modo de bajo consumo, para obtener la resolución y presición
 * especificada por el fabricante.
 *
 * La librería implementa la calibración por hardware en la función @ref hal_adc_init
 *
 * ## Velocidad de conversión
 *
 * Cada conversión realizada toma un tiempo que dependerá del clock configurado en el periférico. Podemos
 * obtener este tiempo de conversión mediante la ecuación: \f$ t_{conv_{ADC}} = \frac{1}{25 * f_{ADC}} \f$
 *
 * El multiplicador \f$ 25 \f$ en el denominador, es debido a la naturaleza del periférico de <e>aproximaciones
 * sucesivas</e>. Esto implica que desde que se genera un inicio de conversión hasta que la misma finaliza,
 * deben transcurrir \f$ 25 \f$ ciclos de clock del \e ADC.
 *
 * Ejemplo: Configurando el \e ADC con una \f$f_{ADC} = 25MHz\f$ obtenemos el tiempo tomado por cada
 * conversión:
 *
 * \f{eqnarray*}{
 *     t_{conv_{ADC}} = \frac{1}{25 * 1MHz} \\
 *     t_{conv_{ADC}} = 1 \mu s
 * \f}
 *
 * Esto implica que entre un inicio de conversión y la finalización de la misma, pasará \f$1 \mu s\f$. Nótese que
 * este tiempo corresponde a una conversión para un único canal. En caso de estar convirtiendo varios canales, se
 * deberá multiplicar \f$ t_{conv_{ADC}} \f$ por la cantidad de canales activos en la secuencia de conversión, para
 * obtener el tiempo total desde un inicio de secuencia de conversión y la finalización de todos los canales.
 *
 * @see Ejemplo_ADC.c
 *
 * @{
 */

#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include <stdint.h>

/** Selección de fuente de clock para el \e ADC */
typedef enum
{
	HAL_ADC_CLOCK_SOURCE_FRO = 0, /**< Free running oscillator como fuente de clock */
	HAL_ADC_CLOCK_SYS_PLL /**< Phase locked loop oscillator como fuente de clock */
}hal_adc_clock_source_en;

/** Selección de modo bajo consumo */
typedef enum
{
	HAL_ADC_LOW_POWER_MODE_DISABLED = 0, /**< Modo bajo consumo inhabilitado */
	HAL_ADC_LOW_POWER_MODE_ENABLED /**< Modo bajo consumo habilitado */
}hal_adc_low_power_mode_en;

/** Selección de secuencia de \e ADC */
typedef enum
{
	HAL_ADC_SEQUENCE_SEL_A = 0, /**< Secuencia A */
	HAL_ADC_SEQUENCE_SEL_B /**< Secuencia B */
}hal_adc_sequence_sel_en;

/** Fuente de trigger para el \e ADC */
typedef enum
{
	HAL_ADC_TRIGGER_SEL_NONE = 0, /**< Ninguna (trigger por software) */
	HAL_ADC_TRIGGER_SEL_PININT0_IRQ, /**< Interrupción de PININT, canal 0 */
	HAL_ADC_TRIGGER_SEL_PININT1_IRQ, /**< Interrupción de PININT, canal 1 */
	HAL_ADC_TRIGGER_SEL_SCT0_OUT3, /**< Salida 3 del SCT */
	HAL_ADC_TRIGGER_SEL_SCT0_OUT4, /**< Salida 4 del SCT */
	HAL_ADC_TRIGGER_SEL_T0_MAT3, /**< Match 3 del CTIMER */
	HAL_ADC_TRIGGER_SEL_CMP0_OUT_ADC, /**< Salida 0 del comparador analógico */
	HAL_ADC_TRIGGER_SEL_GPIO_INT_BMAT, /**< Pattern match */
	HAL_ADC_TRIGGER_SEL_ARM_TXEV /**< Señal TXEV causada por una instrucción SEV */
}hal_adc_trigger_sel_en;

/** Selección de polaridad del trigger del \e ADC */
typedef enum
{
	HAL_ADC_TRIGGER_POL_SEL_NEGATIVE_EDGE = 0, /**< Flanco negativo */
	HAL_ADC_TRIGGER_POL_SEL_POSITIVE_EDGE /**< Flanco positivo */
}hal_adc_trigger_pol_sel_en;

/** Selección de sincronismo en secuencia del \e ADC */
typedef enum
{
	HAL_ADC_SYNC_SEL_ENABLE_SYNC = 0, /**< Habilitación de sincronismo */
	HAL_ADC_SYNC_SEL_BYPASS_SYNC /**< Bypass el sincronismo */
}hal_adc_sync_sel_en;

/** Selección de modo de interrupción del \e ADC */
typedef enum
{
	HAL_ADC_INTERRUPT_MODE_EOC = 0, /**< Modo de interrupción en fin de conversión */
	HAL_ADC_INTERRUPT_MODE_EOS /**< Modo de interrupción en fin de secuencia */
}hal_adc_interrupt_mode_en;

/** Canal que genero el resultado de \e ADC */
typedef enum
{
	HAL_ADC_RESULT_CHANNEL_0 = 0, /**< Canal 0 */
	HAL_ADC_RESULT_CHANNEL_1, /**< Canal 1 */
	HAL_ADC_RESULT_CHANNEL_2, /**< Canal 2 */
	HAL_ADC_RESULT_CHANNEL_3, /**< Canal 3 */
	HAL_ADC_RESULT_CHANNEL_4, /**< Canal 4 */
	HAL_ADC_RESULT_CHANNEL_5, /**< Canal 5 */
	HAL_ADC_RESULT_CHANNEL_6, /**< Canal 6 */
	HAL_ADC_RESULT_CHANNEL_7, /**< Canal 7 */
	HAL_ADC_RESULT_CHANNEL_8, /**< Canal 8 */
	HAL_ADC_RESULT_CHANNEL_9, /**< Canal 9 */
	HAL_ADC_RESULT_CHANNEL_10, /**< Canal 10 */
	HAL_ADC_RESULT_CHANNEL_11, /**< Canal 11 */
	HAL_ADC_RESULT_CHANNEL_GLOBAL /**< Global */
}hal_adc_result_channel_en;

/** Resultado de obtención de resultado de secuencia */
typedef enum
{
	HAL_ADC_SEQUENCE_RESULT_VALID = 0, /**< Resultado válido */
	HAL_ADC_SEQUENCE_RESULT_INVALID /**< Resultado inválido */
}hal_adc_sequence_result_en;

/** Tipo de dato para callback de interrupcion de sequencia */
typedef void (*adc_sequence_interrupt_t)(void);

/** Configuración de secuencia de \e ADC */
typedef struct
{
	uint16_t channels; /**< Canales habilitados. Cada uno de los bits representa el canal */
	hal_adc_trigger_sel_en trigger; /**< Configuración de fuente de trigger para la secuencia */
	hal_adc_trigger_pol_sel_en trigger_pol; /**< Configuración de flanco del trigger para la secuencia */
	hal_adc_sync_sel_en sync_bypass; /**< Configuración de sincronismo de la secuencia */
	hal_adc_interrupt_mode_en mode;/**< Configuración de modo de interrupcion */
	uint8_t burst; /**< Configuración de modo BURST. En caso de ser 0 esta inhabilitado, cualquier otro
						valor lo habilita */
	uint8_t single_step; /**< Configuración de funcionamiento del trigger. En caso de ser 0, un trigger
							  dispara la conversión de toda la secuencia configurada, en caso de ser
							  cualquier otro valor, un trigger dispara la conversión del siguiente canal
							  habilitado en la secuencia */
	uint8_t low_priority; /**< Fijar baja prioridad de la secuencia. Unicamente aplica para la secuencia
	 	 	 	 	 	 	   \e A. En caso de ser 0, la secuencia \e A tiene prioridad por sobre el \e B,
	 	 	 	 	 	 	   cualquier otro valor, implica que la secuencia B tiene prioridad por sobre
	 	 	 	 	 	 	   la \e A */
	adc_callback_t callback; /**< Callback a ejecutar en interrupción de secuencia. La misma se generará
	 	 	 	 	 	 	 	 al final de la conversión de cada canal, o de toda la secuencia,
	 	 	 	 	 	 	 	 dependiendo de la configuración global del \e ADC */
}hal_adc_sequence_config_t;

/** Dato que representa el resultado de una conversión (sea de secuencia completa o de canal) */
typedef struct
{
	hal_adc_result_channel_en channel; /**< Canal que generó el resultado */
	uint16_t result; /**< Valor de la conversión */
}hal_adc_sequence_result_t;

/**
 * @brief Inicializar el \e ADC en modo \b asincrónico
 *
 * Realiza la calibración de hardware y fija la frecuencia de muestreo deseada.
 * Nota: Solamente se debe realizar el llamado a una de las dos funciones de inicialización del \e ADC
 *
 * @see hal_adc_clock_source_en
 * @see hal_adc_low_power_mode_en
 * @param[in] sample_freq Frecuencia de sampleo deseada
 * @param[in] div Divisor para la lógica del \e ADC
 * @param[in] clock_source Fuente de clock para el \e ADC
 * @param[in] low_power Selección de modo de bajo consumo
 */
void hal_adc_init_async_mode(uint32_t sample_freq, uint8_t div, hal_adc_clock_source_en clock_source, hal_adc_low_power_mode_en low_power);

/**
 * @brief Inicializar el \e ADC en modo \b sincrónico
 *
 * Realiza la calibración de hardware y fija la frecuencia de muestreo deseada.
 *
 * @see hal_adc_clock_source_en
 * @see hal_adc_low_power_mode_en
 * @param[in] sample_freq Frecuencia de sampleo deseada
 * @param[in] low_power Selección de modo de bajo consumo
 */
void hal_adc_init_sync_mode(uint32_t sample_freq, hal_adc_low_power_mode_en low_power);

/**
 * @brief De-inicialización del \e ADC
 */
void hal_adc_deinit(void);

/**
 * @brief Configurar una secuencia de conversión
 *
 * Esta función no habilita la secuencia, al menos que el parametro \b burst este activo
 *
 * @see hal_adc_sequence_sel_en
 * @see hal_adc_sequence_config_t
 * @param[in] sequence Seleccion de secuencia a configurar
 * @param[in] config Configuracion deseada para la secuencia
 */
void hal_adc_config_sequence(hal_adc_sequence_sel_en sequence, const hal_adc_sequence_config_t *config);

/**
 * @brief Habilitar una secuencia
 * @see hal_adc_sequence_sel_en
 * @param[in] sequence Secuencia a habilitar
 */
void hal_adc_enable_sequence(hal_adc_sequence_sel_en sequence);

/**
 * @brief Deshabilitar una secuencia
 * @see hal_adc_sequence_sel_en
 * @param[in] sequence Secuencia a deshabilitar
 */
void hal_adc_disable_sequence(hal_adc_sequence_sel_en sequence);

/**
 * @brief Disparar conversiones en una secuencia
 *
 * La configuración de la secuencia, en particular el parametro \b single_step, influye
 * en si esta funcion dispara una secuencia entera o un paso de la misma.
 *
 * @see hal_adc_sequence_sel_en
 * @param[in] sequence Secuencia a disparar
 */
void hal_adc_start_sequence(hal_adc_sequence_sel_en sequence);

/**
 * @brief Obtener resultado de la secuencia
 *
 * El comportamiento de esta funcion depende de la configuración de la secuencia, en particular
 * de la configuracion <b>MODE</b>. En caso de estar configurada para interrumpir al final de cada
 * conversión, la función únicamente guardara el resultado de la conversión en el primer lugar
 * del parámetro <e>result</e>, caso contrario, guardara la cantidad de canales habilitados en la
 * conversión en los distintos lugares del parámetro <e>result</e>.
 *
 * @see hal_adc_sequence_result_en
 * @see hal_adc_sequence_sel_en
 * @see hal_adc_sequence_result_t
 * @param[in] sequence Secuencia de la cual obtener el resultado
 * @param[out] result Lugares donde guardar los resultados de la secuencia
 * @return Resultado de la función
 */
hal_adc_sequence_result_en hal_adc_get_sequence_result(hal_adc_sequence_sel_en sequence, hal_adc_sequence_result_t *result);

#endif /* HAL_ADC_H_ */

/**
 * @}
 */
