/**
 * @file HAL_IOCON.h
 * @brief Declaraciones a nivel de aplicacion del periferico IOCON (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @defgroup IOCON Control de Entrada/Salida (IOCON)
 *
 * # Introducción
 *
 * El periférico \e IOCON es el responsable de configurar las características eléctricas de cada pin del
 * microcontrolador. Dichas características son:
 * 		- Resistor de Pull-up/Pull-Down
 * 		- Modo <em>open rain</em>
 * 		- Histéresis
 * 		- Filtro de <em>glitches</em> configurable
 * 		- Modo analógico
 * 		- Modo IIC <em>Fast Mode - Plus</em> en una de las interfaces
 * 		.
 *
 * # Inversión de lógica del pin
 *
 * Este periférico tiene la posibilidad de invertir la lógica del pin, cuando el mismo es una entrada, mediante hardware.
 * Asumiendo que no está activada la inversión del pin y que está configurado como entrada, si se toma una lectura
 * del mismo, se leerá como \b cero cuando externamente esté a una tensión de aproximadamente VSS y se
 * leerá como \b uno cuando externamente esté a una tensión de aproximadamente VDD. Si se
 * activa la inversión del pin sucederá lo inverso, es decir, si está configurado como entrada y externamente hay
 * una tensión de aproximadamente VSS la lectura se tomará como \b uno y si hay una tensión de
 * aproximadamente VDD la lectura se tomará como \b cero.
 *
 * # Resistencias para fijar un nivel
 *
 * Cada uno de los pines del microcontrolador puede ser configurado para tener una resistencia de
 * <em>pull-up</em> o de <em>pull-down</em>. Esto implica que aunque la entrada no tenga ningún valor fuerte
 * externamente, estas resistencias se encargarán de fijar un valor o bien naturalmente alto (<em>pull-up</em>) o
 * bajo (<em>pull-down</em>). Existe un tercer tipo llamado <em>repetidor</em> el cual configura un
 * <em>pull-up</em> o un <em>pull-down</em> dependiendo del último valor que hubo en la entrada, esto quiere
 * decir que si el pin tiene un valor externamente fijado en estado alto, y queda sin un valor fijo, el
 * microcontrolador automáticamente dejará configurado un <em>pull-up</em>, de esta forma reteniendo el último
 * valor fijo, y viceversa cuando el pin es fijado externamente en estado bajo.
 *
 * # Salidas de tipo <em>Open Drain</em>
 *
 * Una salida tipo <em>open drain</em> en comparación con una salida tradicional, permite colocar un nivel
 * de tensión distinto al de la alimentación del microcontrolador cuando se acciona el pin.
 *
 * \b NOTA: La hoja de datos aclara que la tensión a la cual se puede manejar el pin mediante un resistor de
 * <em>pull-up</em> externo, no puede superar la tensión VDD.
 *
 * # Histéresis
 *
 * Cada pin puede configurar una <em>histéresis</em> cuando se comporta como entrada. Que el pin tenga una
 * <em>histéresis</em> implica que el valor cambiará su lectura una vez superado un cierto umbral Vhys.
 * Esto evita lecturas erróneas cuando la entrada tiene una variación a un ritmo lenta que no se puede evitar.
 *
 * # Filtro de <em>Glitches</em>
 *
 * @image html entrada_con_glitches.png "Entrada con \e Glitches"
 * @image latex entrada_con_glitches.png "Entrada con \e Glitches" width=0.8\textwidth
 *
 * Si una entrada tiene \emph{glitches}, como se muestra en la imágen, se pueden tomar lecturas erróneas a causa
 * de la falta de estabilidad de la señal que excita a la entrada. Una forma de evitar este problema es
 * configurando correctamente el clock asociado al periférico \e IOCON mediante el @ref SYSCON, y el filtro de
 * \e glitches asociado al pin, el cual tomará una lectura como válida luego de una cantidad configurable de
 * ciclos del clock mencionado.
 *
 * # Modo analógico
 *
 * Para utilizar un pin como salida analógica, como se describe en el periférico @ref DAC, se debe configurar
 * dicho pin como salida analógica también mediante el periférico \e IOCON. Además se debe inhabilitar cualquier
 * tipo de resistor interno cuando se configura el pin como salida analógica.
 *
 * # Modo IIC
 *
 * Cuando se utiliza el periférico @ref IIC, en particular la instancia 0, se debe configurar mediante el \e IOCON
 * dependiendo de la velocidad con la que se va a utilizar.
 *
 * # Implementación en la librería
 *
 * Es importante destacar que cada periférico inicializará el \e IOCON en el punto que lo necesite, y luego lo
 * desactivará para conservar energía. La única excepción es el periférico @ref GPIO el cual no genera ninguna
 * inicialización respecto del \e IOCON, por lo que el usuario deberá configurar (en caso de ser necesario) cada
 * uno de los pines como se desee.
 *
 * @{
 */

#ifndef HAL_IOCON_H_
#define HAL_IOCON_H_

#include <HAL_GPIO.h>

/** Selección de resistor interno en el pin */
typedef enum
{
	HAL_IOCON_PULL_NONE = 0, /**< Ningun resistor interno */
	HAL_IOCON_PULL_DOWN, /**< Resistor interno conectado a Vss */
	HAL_IOCON_PULL_UP, /**< Resistor interno conectado a Vdd */
	HAL_IOCON_PULL_REPEATER /**< Modo repetidor */
}hal_iocon_pull_mode_en;

/** Modo de muestreo del filtro de glitches */
typedef enum
{
	HAL_IOCON_SAMPLE_MODE_BYPASS = 0, /**< Sin filtro de glitches */
	HAL_IOCON_SAMPLE_MODE_1_CLOCK, /**< Por lo menos un ciclo de clock para filtrar el glitch */
	HAL_IOCON_SAMPLE_MODE_2_CLOCK, /**< Por lo menos dos ciclos de clock para filtrar el glitch */
	HAL_IOCON_SAMPLE_MODE_3_CLOCK /**< Por lo menos tres ciclos de clock para filtrar el glitch */
}hal_iocon_sample_mode_en;

/** Selección de banco de división de clock para el filtro de glitches */
typedef enum
{
	HAL_IOCON_CLK_DIV_0 = 0, /**< Banco 0 */
	HAL_IOCON_CLK_DIV_1, /**< Banco 1 */
	HAL_IOCON_CLK_DIV_2, /**< Banco 2 */
	HAL_IOCON_CLK_DIV_3, /**< Banco 3 */
	HAL_IOCON_CLK_DIV_4, /**< Banco 4 */
	HAL_IOCON_CLK_DIV_5, /**< Banco 5 */
	HAL_IOCON_CLK_DIV_6 /**< Banco 6 */
}hal_iocon_clk_sel_en;

/** Modo de funcionamiento IIC */
typedef enum
{
	HAL_IOCON_IIC_MODE_STANDARD = 0, /**< Modo IIC standard */
	HAL_IOCON_IIC_MODE_GPIO, /**< Modo GPIO */
	HAL_IOCON_IIC_MODE_FAST_MODE /**< Modo IIC Fast-Mode */
}hal_iocon_iic_mode_en;

/** Estructura de configuración de un pin mediante el \e IOCON */
typedef struct
{
	hal_iocon_pull_mode_en pull_mode; /**< Resistor interno */
	uint8_t hysteresis; /**< Histéresis. Cualquier valor distinto de cero la activa */
	uint8_t invert_input; /**< Inversión de lógica de lectura. Cualquier valor distinto de cero la activa */
	uint8_t open_drain; /**< Modo <em>Open Drain</em>. Cualquier valor distinto de cero lo activa */
	hal_iocon_sample_mode_en sample_mode; /**< Cantidad de muestras del filtro de glitches */
	hal_iocon_clk_sel_en clk_sel; /**< Selección de clock para el filtro de glitches */
	hal_iocon_iic_mode_en iic_mode; /**< Selección de modo IIC */
}hal_iocon_config_t;

/**
 * @brief Configuracion de un pin
 *
 * Nótese que las configuraciones de modo analógico no están en la estructura de configuración
 * @ref hal_iocon_config_t. Esto es debido a que dichas configuraciones se realizarán en el periférico
 * correspondiente, sea el @ref ADC o @ref DAC.
 *
 * @param[in] portpin Puerto/pin a configurar
 * @param[in] pin_config Puntero a estructura de configuracion del pin
 * @pre Configuración de divisores de clock de bancos de filtro de glitches en caso de ser necesario.
 */
void hal_iocon_config_io(hal_gpio_portpin_en portpin, const hal_iocon_config_t *config);

#endif /* HAL_IOCON_H_ */

/**
 * @}
 */
