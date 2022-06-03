/**
 * @file HAL_USART.h
 * @brief Declaraciones a nivel de aplicacion del periferico USART (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @defgroup UART Transmisor/Receptor Asincrónico Universal (UART)
 *
 * # Introducción
 *
 * Este periférico permite generar comunicaciones de tipo *serie* para la transferencia de datos
 * entre dos dispositivos. En particular, el mismo puede o no utilizar una línea de clock para arbitrar
 * dichas comunicaciones. Para el caso asincrónico, los dispositivos a comunicar deberán establecer una
 * velocidad conocida de comunicación. En particular en la documentación presente, se explican e implementan
 * funciones para el modo **asincrónico** únicamente.
 *
 * # Componentes de entrada/salida
 *
 * @image html conexiones_usart.png "Interconexiones USART"
 * @image latex conexiones_usart.png "Interconexiones USART" width=0.8\textwidth
 *
 * En la imágen se muestra un esquema de conexionado típico entre dos dispositivos que se comunican mediante
 * un protocolo *USART*. Cabe destacar que pueden existir dispositivos intermedios para adaptar la señal
 * a niveles de tensión que cumplan con una determinada norma. Normas típicas son:
 * 		- RS232
 * 		- RS485
 * 		- RS422
 * 		- Lazo de corriente
 * 		.
 *
 * Algunas normas pueden incluir definiciones acerca de los conectores que se necesitan utilizar, pero **todas**
 * ellas definen niveles de tensión necesarios para cumplir con la misma. Dependiendo de la norma, la
 * comunicación entre los dispositivos podrá ser *Half Duplex* o *Full Duplex*.
 *
 * # Parámetros de la comunicación
 *
 * @image html comunicacion_usart.png "Trama USART"
 * @image latex comunicacion_usart.png "Trama USART" width=0.8\textwidth
 *
 * En la imágen se muestra la estructura de una comunicación generada por este periférico:
 * 		- *Baudrate*: Define la velocidad de comunicación entre los dispositivos. Se encuentra
 * 		expresada en *bits por segundo (bps)*. Conociendo la velocidad de comunicación, podemos
 * 		obtener el tiempo que tomará cada bit de la comunicación mediante la fórmula
 * 		\f{eqnarray*}{
 * 			t_{bit} = \frac{1}{baudrate}
 * 		\f}
 * 		- *Bit de start*: El bit de start se utiliza para indicar el inicio de una comunicación
 * 		entre los dispositivos interconectados. Siempre es fijado como un 0 lógico, y tiene un
 * 		largo de un bit.
 * 		- *Bits de datos*: Esta es la información a transmitir de un dispositivo a otro. Su longitud
 * 		puede variar dependiendo de la aplicación, en general se encuentra entre 5 y 9 bits.
 * 		- *Bit de paridad*: El bit de paridad puede o no existir en la configuración de la comunicación.
 * 		El mismo se utiliza para detectar errores simples en la comunicación, la cual se puede ver
 * 		afectada por factores externos como: ruido, cables defectuosos, mala instalación, etc.
 * 			- Paridad **Par**: El bit de paridad será un 1 lógico cuando la cantidad de bits
 * 			en el dato a transmitir en 1 (excluyendo bit de start y bit/s de stop) sea par.
 * 			- Paridad **Impar**: El bit de paridad será un 1 lógico cuando la cantidad de bits
 * 			en el dato a transmitir en 1 (excluyendo bit de start y bit/s de stop) sea impar.
 * 			.
 * 		- *Bit/s de stop*: El o los bit/s de stop se utilizan para indicar el fin de la transmisión
 * 		del dato actual. Siempre se fijan al valor 1 lógico, y su duración puede ser configurable,
 * 		en general pudiendo tomar valores de 1 o 2 bits.
 * 		.
 *
 * Cabe destacar que ambos dispositivos deben conocer estos parámetros y deben estar configurados con los
 * mismos para que la comunicación pueda establecerse.
 *
 * # Configuración y consideraciones para la generación del Baudrate
 *
 * El periférico *USART* puede utilizar una de las siguientes fuentes de clock para la generación de
 * su *Baudrate*:
 * 		- FRO
 * 		- Main Clock
 * 		- FRG0
 * 		- FRG1
 * 		- \f$ \frac{\mathrm{FRO}}{2}\f$
 *
 * En particular, si se necesita un bajo error en la comunicación, es deseable utilizar alguna instancia de
 * *FRG* como fuente de clock para la generación del baudrate, dado que permitirá obtener valores de clock
 * más flexibles que las demás fuentes de clock. Es aconsejable utilizar una fuente de clock que sea lo más
 * alta y lo más cercana a un múltiplo del *Baudrate* deseado posible, para así minimizar el error.
 *
 * ## Ejemplo para obtener un Baudrate de 115200 bps
 *
 * \f{eqnarray*}{
 * 		Baudrate = \frac{CLK_{USART}}{(OSRVAL+1) \cdot (BRGVAL+1)}
 * \f}
 *
 * Donde:
 * - \f$CLK_{USART}\f$ : el clock seleccionado para la USART que utilicemos.
 * - \f$OSRVAL\f$ :
 * - \f$BRGVAL\f$ :
 *
 * \f{eqnarray*}{
 * 		BRGVAL = \frac{CLK_{USART}}{(OSRVAL+1) \cdot Baudrate} - 1
 * \f}
 *
 * @{
 */

/**
 * @example Ejemplo_USART.c
 *
 * @brief
 * Ejemplo de configuración del perifério **USART** para establecer una comunicación entrante y saliente
 * al microcontrolador.
 *
 * @note
 * Este ejemplo establece la **comunicación serie con la computadora a través
 * del cable USB conectado al stick de desarrollo**. \n
 * Ver @ref acerca_del_stick para más información sobre este punto en particular.
 *
 * ## Configuraciones
 *
 * El programa utiliza como Main Clock el *FRO (Free Running Oscilator)* configurado sin divisor, es decir,
 * con una frecuencia de 24MHz.
 *
 * Se utiliza el periférico *USART0*, con los pines RX y TX que corresponden al puerto serie a través del
 * puerto USB:
 * - USART0 RX: Puerto 0, Pin 24
 * - USART0 TX: Puerto 0, Pin 25
 *
 * La configuración del protocolo es la siguiente:
 * - Baudrate:	115200 bps
 * - Trama 8 bits
 * - Sin paridad
 * - 1 bit de stop
 * - X16 Oversampling
 *
 * Además, la USART0 está configurada para generar interrupciones cuando se haya recibido un dato
 * y cada vez que se haya completado la transmisión de una trama de datos.
 *
 * # Descripción de funcionamiento
 *
 * El programa estará a la espera de la recepción del caracter 'S' y cada vez que reciba
 * dicho caracter responderá enviando el mensaje "Trama de respuesta automatica\n".
 *
 * Una vez cargado el ejemplo en el microcontrolador, el usuario puede verificar su funcionamiento
 * a través de cualquier software de terminal serie.
 * Por ejemplo, en sistemas Linux, puede utilizarse el programa 'Cutecom'.
 *
 * A través de dichos programas el usuario deberá enviar el caracter 'S' para luego visualizar la respuesta
 * del programa.
 *
 * @note El usuario debe asegurarse de haber detenido la sesión de debug del MCUXpresso antes
 * de probar el ejemplo. \n
 * Esto se debe a qué el puerto USB es compartido con el debugger
 * propio del LPC845, y no debe utilizarse en simultáneo con la comunicación configurada en este ejemplo.
 *
 * @author Augusto Santini
 * @author Esteban Chiama
 * @date 4/2020
 */

#ifndef HAL_USART_H_
#define HAL_USART_H_

#include <stdint.h>
#include "HAL_SYSCON.h"
#include "HAL_GPIO.h"

#if defined (__cplusplus)
extern "C" {
#endif

/** Seleccion de instancia de USART */
typedef enum
{
	HAL_USART_SEL_0, /**< Instancia 0 */
	HAL_USART_SEL_1, /**< Instancia 1 */
	HAL_USART_SEL_2, /**< Instancia 2 */
	HAL_USART_SEL_3, /**< Instancia 3 */
	HAL_USART_SEL_4, /**< Instancia 4 */
	HAL_USART_SEL_AMOUNT /**< Cantidad de instancias */
}hal_usart_sel_en;

/** Cantidad de bits en cada dato */
typedef enum
{
	HAL_USART_DATALEN_7BIT = 0, /**< Largo de dato de 7 bits */
	HAL_USART_DATALEN_8BIT, /**< Largo de dato de 8 bits */
	HAL_USART_DATALEN_9BIT /**< Largo de dato de 9 bits */
}hal_usart_datalen_en;

/** Paridad en la comunicación */
typedef enum
{
	HAL_USART_PARITY_NO_PARITY = 0, /**< Comunicación sin paridad */
	HAL_USART_PARITY_EVEN = 2, /**< Comunicación con paridad par */
	HAL_USART_PARITY_ODD /**< Comunicación con paridad impar */
}hal_usart_parity_en;

/** Cantidad de bits de stop en la comunicación */
typedef enum
{
	HAL_USART_STOPLEN_1BIT = 0, /**< Un bit de stop */
	HAL_USART_STOPLEN_2BIT /**< Dos bits de stop */
}hal_usart_stop_en;

/** Oversampling en el muestreo de los bits */
typedef enum
{
	HAL_USART_OVERSAMPLING_X5 = 4, /**< Oversampling x5 */
	HAL_USART_OVERSAMPLING_X6, /**< Oversampling x6 */
	HAL_USART_OVERSAMPLING_X7, /**< Oversampling x7 */
	HAL_USART_OVERSAMPLING_X8, /**< Oversampling x8 */
	HAL_USART_OVERSAMPLING_X9, /**< Oversampling x9 */
	HAL_USART_OVERSAMPLING_X10, /**< Oversampling x10 */
	HAL_USART_OVERSAMPLING_X11, /**< Oversampling x11 */
	HAL_USART_OVERSAMPLING_X12, /**< Oversampling x12 */
	HAL_USART_OVERSAMPLING_X13, /**< Oversampling x13 */
	HAL_USART_OVERSAMPLING_X14, /**< Oversampling x14 */
	HAL_USART_OVERSAMPLING_X15, /**< Oversampling x15 */
	HAL_USART_OVERSAMPLING_X16, /**< Oversampling x16 */
}hal_usart_oversampling_en;

/** Resultado de un intento de inicio de transmisión */
typedef enum
{
	HAL_USART_TX_RESULT_OK = 0, /**< Inicio de transmisión ok */
	HAL_USART_TX_RESULT_NOT_READY /**< Inicio de transmisión no exitoso, se estaba transmitiendo */
}hal_usart_tx_result;

/** Resultado de un intento de lectura de recepción */
typedef enum
{
	HAL_USART_RX_RESULT_OK = 0, /**< Resultado de recepción ok */
	HAL_USART_RX_RESULT_NOT_READY /**< Resultado de recepción no exitoso, no se había recibido ningún dato */
}hal_usart_rx_result;

/**
 * @brief Tipo de dato para los callback en recepción de dato completa.
 * @note Estos callbacks son ejecutados desde un contexto de interrupción, por lo que el usuario deberá tener
 * todas las consideraciones necesarias al respecto.
 */
typedef void (*hal_usart_rx_callback)(hal_usart_sel_en, void *);

/**
 * @brief Tipo de dato para los callback en transmisión de dato completa
 * @note Estos callbacks son ejecutados desde un contexto de interrupción, por lo que el usuario deberá tener
 * todas las consideraciones necesarias al respecto.
 */
typedef void (*hal_usart_tx_callback)(hal_usart_sel_en, void *);

/** Estructura de configuración de una instancia de UART */
typedef struct
{
	hal_usart_datalen_en data_length; /**< Largo en bits de cada dato en la comunicación */
	hal_usart_parity_en parity; /**< Paridad de la comunicación */
	hal_usart_stop_en stop_bits; /**< Cantidad de bits de stop de la comunicación */
	hal_usart_oversampling_en oversampling; /**< Oversampling de la instancia */
	hal_syscon_peripheral_clock_sel_en clock_selection; /**< Fuente de clock para la instancia */
	uint32_t baudrate; /**< Baudrate deseado para la comunicación */
	hal_gpio_portpin_en tx_portpin; /**< Puerto/pin donde configurar las transmisiones */
	hal_gpio_portpin_en rx_portpin; /**< Puerto/pin donde configurar las recepciones */
	hal_usart_tx_callback tx_ready_callback; /**< Callback a ejecutar en una transmisión exitosa (Cuando se terminó de enviar un dato) */
	hal_usart_rx_callback rx_ready_callback; /**< Callback a ejecutar en una recepción exitosa (Cuando se terminó de recibir un dato) */
	void *tx_ready_data; /**< Datos a pasar al callback de transmisión exitosa */
	void *rx_ready_data; /**< Datos a pasar al callback de recepción exitosa */
}hal_usart_config_t;

/**
 * @brief Inicializar USART con los parámetros deseados
 * @param[in] inst Instancia de USART inicializar
 * @param[in] config Configuración deseada de la instancia
 * @pre Haber inicializado la fuente de clock a utilizar correctamente. Ver @ref SYSCON
 */
void hal_usart_init(hal_usart_sel_en inst, const hal_usart_config_t * config);

/**
 * @brief Transmitir un dato mediante una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir. Puede ser de 7, 8 o 9 bits
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */
hal_usart_tx_result hal_usart_tx_data(hal_usart_sel_en inst, uint32_t data);

/**
 * @brief Recibir un dato de una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Puntero a donde guardar el dato recibido
 * @return Estado de la recepción
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */
hal_usart_rx_result hal_usart_rx_data(hal_usart_sel_en inst, uint32_t *data);

/**
 * @brief Inhibir las interrupciones de una instancia USART
 * @param[in] inst Instancia a inhibir
 */
void hal_usart_inhibit_interrupts(hal_usart_sel_en inst);

/**
 * @brief Deinhibir las interrupciones de una instancia USART
 * @param[in] inst Instancia a deinhibir
 */
void hal_usart_deinhibit_interrupts(hal_usart_sel_en inst);

/**
 * @brief Registrar el callback a ser llamado en la recepcion de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejectutar cada vez que se recibe un dato por USART
 * @param[in] cb_data Datos a pasar al calback cada vez que se recibe un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void hal_usart_rx_register_callback(hal_usart_sel_en inst, hal_usart_rx_callback new_callback, void *cb_data);

/**
 * @brief Registrar el callback a ser llamado una vez finalizada la transmision de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejecutar cada vez que se termina de enviar un dato por USART
 * @param[in] cb_data Datos a pasar al calback cada vez que se recibe un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void hal_usart_tx_register_callback(hal_usart_sel_en inst, hal_usart_tx_callback new_callback, void *cb_data);

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HAL_USART_H_ */

/**
 * @}
 */
