/**
 * @file HAL_SYSCON.h
 * @brief Declaraciones a nivel de aplicacion del periferico SYSCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

/**
 * @defgroup SYSCON Configuración del Sistema (SYSCON)
 *
 * # Introducción
 *
 * Este periférico es el encargado de manejar múltiples características del sistema del microcontrolador. Entre
 * ellas se encuentran:
 * 		- Control de clocks
 * 		- Control de reset/clocks de periféricos
 * 		- Selección de pines para interrupciones externas (Ver @ref PININT)
 * 		- Configuración de modos de bajo consumo del microcontrolador
 * 		- Configuración de wake-up para salir de los modos de bajo consumo del microcontrolador
 * 		- Configuración del Brown-Out Detector
 * 		- Configuración del Micro Trace Buffer
 * 		- Control de interrupción de latencias
 * 		- Control de Non Maskable Interrupt
 * 		- Valor de calibración del @ref SYSTICK
 * 		.
 *
 * En la capa de abstracción de aplicación (HAL) se implementan principalmente las funciones relacionadas con los
 * tres primeros ítems hasta el momento.
 *
 * # Control de clocks
 *
 * ## Fuentes de clock
 *
 * Las distintas fuentes de clock que se explican a continuación, son seleccionables mediante el *SYSCON*. Algunos
 * necesitan una referencia para funcionar, mientras que otros funcionan sin necesidad de ninguna.
 *
 * ### Free Running Oscillator (FRO)
 *
 * Este oscilador es con el que comienza el microcontrolador por defecto luego de un reset. La frecuencia del
 * mismo se puede configurar (*no implementado todavia*) pero por default comienza en 24MHz con
 * un divisor /2, resultando en una frecuencia efectiva de 12MHz.
 *
 * @note Las funciones relacionadas con el *FRO* son @ref hal_syscon_fro_clock_config y
 * @ref hal_syscon_fro_clock_get
 *
 * ### Phase Locked Loop (PLL)
 *
 * Este oscilador toma una frecuencia de entrada y genera una conversión para obtener una frecuencia efectiva
 * mayor a la de entrada. La frecuencia de entrada mínima del mismo es de 10MHz.
 *
 * @note Las funciones relacionadas con el *PLL* son @ref hal_syscon_pll_clock_config y
 * @ref hal_syscon_pll_clock_get.
 *
 * ### Main/System clock
 *
 * El clock principal o de sistema (estos nombres se usan indistintamente) genera la frecuencia base de la cual
 * se derivan la mayoría de los periféricos. El mismo puede ser tomado de la señal de salida del *PLL* o de la
 * señal previa al *PLL*. Este clock es el que provee la frecuencia del núcleo del microcontrolador.
 *
 * @note La selección de la fuente de clock principal se realiza mediante la función
 * @ref hal_syscon_system_clock_set_source mientras que la obtención de la frecuencia actual del clock
 * principal configurada se obtiene con la función @ref hal_syscon_system_clock_get.
 *
 * ### Clock externo
 *
 * El clock externo puede ser de utilidad cuando se tiene una referencia de frecuencia de muy buena estabilidad
 * externa al microcontrolador. Casos típicos son *cristales*, u osciladores de alta presición y bajo drift.
 * Si se utiliza un *cristal externo* se utilizarán los pines P0_8 y P0_9 como fuentes de entrada para el
 * circuito oscilador interno, el cual se encargará de generar la frecuencia de clock correspondiente, mientras que
 * si se utiliza un oscilador externo, se utilizará únicamente el pin P0_1.
 *
 * @note Para configurar el clock externo se utilizan las funciones @ref hal_syscon_external_crystal_config y
 * @ref hal_syscon_external_clock_config.
 *
 * ### Generadores fraccionales de clock
 *
 * El microcontrolador dispone de dos *Generadores fraccionales de clock*. Los mismos son de gran utilidad
 * cuando se necesita tener presición en la frecuencia de algún periférico y la frecuencia del clock principal
 * con los divisores del periférico no nos alcanzas para lograr dicha presición. La ventaja de estos generadores
 * es que toman una frecuencia de referencia, y generan una división fraccional del mismo. El divisor de estos
 * generadores varía entre 1 y 2 con valores decimales, pudiendo así lograr frecuencias que los demás
 * divisores no pueden, dada su naturaleza de división entera.
 *
 * Periféricos que pueden ser excitados mediante estos generadores:
 * 		- @ref UART
 * 		- @ref SPI
 * 		- @ref IIC
 * 		.
 *
 * @note La función para configurar los generadores fraccionales de clock es @ref hal_syscon_frg_config.
 *
 * ### Oscilador del *Watchdog*
 *
 * El periférico @ref WATCHDOG tiene su propia fuente de clock. Este oscilador es de ultra bajo consumo, pero su
 * presición es de más/menos 40. El oscilador puede tener como base una variedad de valores y también tiene su
 * propio divisor, logrando rangos entre 9.3KHz y 2.3MHz. Se parte de una frecuencia base
 * seleccionable, y luego se aplica un divisor al mismo para obtener la frecuencia final de oscilación.
 *
 * @note Las características de este oscilador se controlan mediante la función
 * @ref hal_syscon_watchdog_oscillator_config
 *
 * ## Divisores de clock
 *
 * Ciertos periféricos permiten dividir la frecuencia de clock principal o de otra fuente, para así tener un menor
 * consumo de energía, o para cumplir con especificaciones de frecuencia que requiera el periférico particularmente.
 *
 * ### Divisor del clock principal
 *
 * La frecuencia del clock principal puede ser dividida por cualquier número entero entre 1 y 255. Para
 * situaciones donde la velocidad de procesamiento no sea una necesidad primordial, se puede reducir la
 * misma mediante este divisor. Como este divisor afecta al clock principal, y la mayoría de los periféricos
 * corren con un clock asociado al mismo, se verá reducido el consumo notablemente, a expensas de reducir
 * la velocidad de procesamiento.
 *
 * @note La función para el control de este divisor es @ref hal_syscon_system_clock_set_divider.
 *
 * ### Divisor del clock del ADC
 *
 * El clock de la lógica del periférico @ref ADC es alimentado por el clock principal luego de pasar por este
 * divisor. El divisor puede ser configurado en cualquier valor entero entre 0 y 255. Si se coloca en 0, el
 * clock del *ADC* será anulado. Nótese que la configuración necesaria del divisor es realizada en las
 * funciones de inicialización del *ADC*.
 *
 * @note El periférico @ref ADC se ocupará de configurar su divisor en caso de ser necesario. No se proveen
 * funciones en este módulo para la configuración del mismo.
 *
 * ### Divisor del clock del SCT
 *
 * Al igual que con el *ADC*, el clock de la lógica del periférico @ref SCT es alimentado por el clock principal
 * luego de pasar por este divisor. El divisor puede ser configurado en cualquier valor entero entre 0 y 255. Si se
 * coloca en 0, el clock del *SCT* será anulado.
 *
 * @note Este periférico, así como las funciones para configurar su divisor en el *SYSCON*, todavía no
 * está implementado en la librería.
 *
 * ### Divisor de la salida CLKOUT
 *
 * En caso de ser necesario, el periférico *SYSCON* puede ser configurado para general una salida en uno de los
 * pines del microcontrolador que esté relacionada a algunas fuentes de clock. Antes de salir por el pin, la señal
 * pasa por un divisor, el cual puede ser configurado en cualquier valor entero entre 0 y 255. Si el mismo es
 * configurado en 0, la salida es anulada. Fuentes de clock disponibles para la salida CLKOUT:
 * 		- FRO
 * 		- Clock principal
 * 		- PLL
 * 		- Clock externo (Cristal/entrada externa)
 * 		- Oscilador del watchdog
 * 		.
 *
 * @note La función para el manejo de la salida *CLKOUT* es @ref hal_syscon_clkout_config.
 *
 * ### Divisores para el filtro de Glitches del IOCON
 *
 * El periférico @ref IOCON tiene la posibilidad de ser configurado para eliminar glitches en entradas mediante
 * un filtrado por hardware. Dicho filtro de glitches puede ser configurado para tomar su señal de excitación de
 * uno de los siete divisores que tiene el *SYSCON* reservados para este fin. El valor de estos divisores puede
 * ser cualquier valor entero entre 0 y 255. Si se configura como 0, el divisor no generará señal de excitación,
 * anulando así la funcionalidad.
 *
 * @note La función para el manejo de los divisores de filtros de glitches es
 * @ref hal_syscon_iocon_glitch_divider_set.
 *
 * @{
 */

/**
 * @example Ejemplo_SYSCON.c
 *
 * El programa configura el FRO para funcionar sin divisor y con la frecuencia configurada por default, obteniendo
 * una frecuencia de funcionamiento del mismo de 24MHz. Además, selecciona el FRO como clock principal de sistema.
 *
 * Luego configura la salida CLKOUT para salir en el Puerto 0 ; Pin 18 mediante un divisor por 24, obteniendo una
 * frecuencia de salida en el pin Puerto 0 ; Pin 18 de 1MHz.
 *
 * La única forma de medir dicha salida es mediante un osciloscopio.
 */

#ifndef HAL_SYSCON_H_
#define HAL_SYSCON_H_

#include <stdint.h>
#include <HAL_GPIO.h>

/** Selección de fuente de clock para el clock principal */
typedef enum
{
	HAL_SYSCON_SYSTEM_CLOCK_SEL_FRO = 0, /**< Free Running Oscillator */
	HAL_SYSCON_SYSTEM_CLOCK_SEL_EXT, /**< Clcok externo */
	HAL_SYSCON_SYSTEM_CLOCK_SEL_WATCHDOG, /**< Watchdog Oscillator */
	HAL_SYSCON_SYSTEM_CLOCK_SEL_FRO_DIV, /**< Free Running Oscillator dividido 2 */
	HAL_SYSCON_SYSTEM_CLOCK_SEL_PLL /**< Phase Locked Loop */
}hal_syscon_system_clock_sel_en;

/** Selección de fuente de clock para la salida CLKOUT */
typedef enum
{
	HAL_SYSCON_CLKOUT_SOURCE_SEL_FRO = 0, /**< Free Running Oscillator */
	HAL_SYSCON_CLKOUT_SOURCE_SEL_MAIN_CLOCK, /**< Clock principal */
	HAL_SYSCON_CLKOUT_SOURCE_SEL_SYS_PLL, /**< Phase Locked Loop */
	HAL_SYSCON_CLKOUT_SOURCE_SEL_EXT_CLOCK, /**< Clock externo */
	HAL_SYSCON_CLKOUT_SOURCE_SEL_WATCHDOG_OSC /**< Watchdog Oscillator */
}hal_syscon_clkout_source_sel_en;

/** Selección de fuente de clock para los generadores fraccionales de clock */
typedef enum
{
	HAL_SYSCON_FRG_CLOCK_SEL_FRO = 0, /**< Free Running Oscillator */
	HAL_SYSCON_FRG_CLOCK_SEL_MAIN_CLOCK, /**< Clock principal */
	HAL_SYSCON_FRG_CLOCK_SEL_SYS_PLL, /**< Phase Locked Loop */
	HAL_SYSCON_FRG_CLOCK_SEL_NONE /**< Ninguno */
}hal_syscon_frg_clock_sel_en;

/** Selección de frecuencia base para el watchdog oscillator */
typedef enum
{
	HAL_SYSCON_WATCHDOG_CLKANA_0KHZ = 0, /**< 0MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_600KHZ, /**< 0.6MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_1050KHZ, /**< 1.05MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_1400KHZ, /**< 1.4MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_1750KHZ, /**< 1.75MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_2100KHZ, /**< 2.1MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_2400KHZ, /**< 2.4MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_3000KHZ, /**< 3MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_3250KHZ, /**< 3.25MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_3500KHZ, /**< 3.5MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_3750KHZ, /**< 3.75MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_4000KHZ, /**< 4MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_4200KHZ, /**< 4.2MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_4400KHZ, /**< 4.4MHz */
	HAL_SYSCON_WATCHDOG_CLKANA_4600KHZ /**< 4.6MHz */
}hal_syscon_watchdog_clkana_sel_en;

/** Selección de periférico para controlar fuente de clock */
typedef enum
{
	HAL_SYSCON_PERIPHERAL_SEL_UART0 = 0, /**< UART0 */
	HAL_SYSCON_PERIPHERAL_SEL_UART1, /**< UART1 */
	HAL_SYSCON_PERIPHERAL_SEL_UART2, /**< UART2 */
	HAL_SYSCON_PERIPHERAL_SEL_UART3, /**< UART3 */
	HAL_SYSCON_PERIPHERAL_SEL_UART4, /**< UART4 */
	HAL_SYSCON_PERIPHERAL_SEL_IIC0, /**< IIC0 */
	HAL_SYSCON_PERIPHERAL_SEL_IIC1, /**< IIC1 */
	HAL_SYSCON_PERIPHERAL_SEL_IIC2, /**< IIC2 */
	HAL_SYSCON_PERIPHERAL_SEL_IIC3, /**< IIC3 */
	HAL_SYSCON_PERIPHERAL_SEL_SPI0, /**< SPI0 */
	HAL_SYSCON_PERIPHERAL_SEL_SPI1 /**< SPI1 */
}hal_syscon_peripheral_sel_en;

/** Selección de fuente de clock para los periféricos */
typedef enum
{
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRO = 0, /**< Free Running Oscillator */
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_MAIN, /**< Clock principal */
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRG0, /**< Generador fraccional 0 */
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRG1, /**< Generador fraccional 1 */
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRO_DIV, /**< Free Running Oscillator dividido por 2 */
	HAL_SYSCON_PERIPHERAL_CLOCK_SEL_NONE = 7 /**< Ninguna */
}hal_syscon_peripheral_clock_sel_en;

/** Selección de banco de división para filtros de glitches */
typedef enum
{
	HAL_SYSCON_IOCON_GLITCH_SEL_0 = 0, /**< Banco 0 */
	HAL_SYSCON_IOCON_GLITCH_SEL_1, /**< Banco 1 */
	HAL_SYSCON_IOCON_GLITCH_SEL_2, /**< Banco 2 */
	HAL_SYSCON_IOCON_GLITCH_SEL_3, /**< Banco 3 */
	HAL_SYSCON_IOCON_GLITCH_SEL_4, /**< Banco 4 */
	HAL_SYSCON_IOCON_GLITCH_SEL_5, /**< Banco 5 */
	HAL_SYSCON_IOCON_GLITCH_SEL_6, /**< Banco 6 */
	HAL_SYSCON_IOCON_GLITCH_SEL_7 /**< Banco 7 */
}hal_syscon_iocon_glitch_sel_en;

/** Fuente de clock para el PLL */
typedef enum
{
	HAL_SYSCON_PLL_SOURCE_SEL_FRO = 0, /**< Free Running Oscillator */
	HAL_SYSCON_PLL_SOURCE_SEL_EXT_CLK, /**< Clock externo */
	HAL_SYSCON_PLL_SOURCE_SEL_WATCHDOG, /**< Watchdog Oscillator */
	HAL_SYSCON_PLL_SOURCE_SEL_FRO_DIV /**< Free Running Oscillator dividido por 2 */
}hal_syscon_pll_source_sel_en;

/**
 * @brief Obtener la frecuencia actual del main clock
 * @return Frecuencia del main clock en Hz
 */
uint32_t hal_syscon_system_clock_get(void);

/**
 * @brief Configuración de fuente de clock para el clock principal
 * @param[in] clock_source Selección deseada
 */
void hal_syscon_system_clock_set_source(hal_syscon_system_clock_sel_en clock_source);

/**
 * @brief Fijar el divisor del clock principal
 * @param[in] div Divisor deseado. Cero inhabilita el clock principal
 */
void hal_syscon_system_clock_set_divider(uint8_t div);

/**
 * @brief Obtener la frecuencia actual del FRO
 * @return Frecuencia del FRO en Hz
 */
uint32_t hal_syscon_fro_clock_get(void);

/**
 * @brief Configurar el ext clock a partir de un cristal externo
 * @param[in] crystal_freq Frecuencia del cristal externo utilizado
 */
void hal_syscon_external_crystal_config(uint32_t crystal_freq);

/**
 * @brief Configurar el ext clock a partir de una fuente de clock externa
 * @param[in] external_clock_freq Frecuencia de la fuente de clock externa en Hz
 */
void hal_syscon_external_clock_config(uint32_t external_clock_freq);

/**
 * @brief Configurar el clock FRO
 *
 * @note Esta función habilita el FRO
 *
 * @param[in] direct Si es distinto de cero se omite el divisor del FRO
 */
void hal_syscon_fro_clock_config(uint8_t direct);

/**
 * @brief Inhabilitar el FRO
 */
void hal_syscon_fro_clock_disable(void);

/**
 * @brief Configurar el pin de clock out (salida de clock hacia afuera)
 * @param[in] portpin Número de puerto/pin por donde sacar el clock out
 * @param[in] clock_source Fuente deseada para la salida clock out
 * @param[in] divider Divisor deseado para la salida clock out
 */
void hal_syscon_clkout_config(hal_gpio_portpin_en portpin, hal_syscon_clkout_source_sel_en clock_source, uint8_t divider);

/**
 * @brief Configurar el divisor fraccional
 *
 * El divisor siempre se debe fijar en 256 para estos MCU.
 *
 * @param[in] inst Instancia de FRG a configurar
 * @param[in] clock_source Fuente de clock de entrada para el FRG
 * @param[in] mul Multiplicador deseado
 */
void hal_syscon_frg_config(uint8_t inst, hal_syscon_frg_clock_sel_en clock_source, uint32_t mul);

/**
 * @brief Configuración del watchdog oscillator
 * @param[in] clkana_sel Selección de frecuencia base del oscilador
 * @param[in] div Divisor. El valor efectivo de división es: 2 (1 + div)
 *
 * @note Recordar que la presición de este oscilador es de más/menos 40%
 */
void hal_syscon_watchdog_oscillator_config(hal_syscon_watchdog_clkana_sel_en clkana_sel, uint8_t div);

/**
 * @brief Obtener la frecuencia de clock en Hz configurada para cierto periférico
 * @param[in] peripheral Periférico deseado
 * @return Frecuencia en Hz del clock del periférico
 */
uint32_t hal_syscon_peripheral_clock_get(hal_syscon_peripheral_sel_en peripheral);

/**
 * @brief Configurar divisor para el clock de glitches del IOCON
 * @param[in] sel Selección de divisor
 * @param[in] div Valor de división deseado
 */
void hal_syscon_iocon_glitch_divider_set(hal_syscon_iocon_glitch_sel_en sel, uint32_t div);

/**
 * @brief Configurar el PLL
 * @param[in] clock_source Fuente de clock de referencia para el PLL
 * @param[in] freq Frecuencia deseada de salida del PLL
 */
void hal_syscon_pll_clock_config(hal_syscon_pll_source_sel_en clock_source, uint32_t freq);

/**
 * @brief Obtener frecuencia actual configurada del PLL
 * @return Frecuencia actual del PLL en Hz
 */
uint32_t hal_syscon_pll_clock_get(void);

#endif /* HAL_SYSCON_H_ */

/**
 * @}
 */
