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
 * Este periférico permite generar comunicaciones de tipo <em>serie</em> para la transferencia de datos
 * entre dos dispositivos. En particular, el mismo puede o no utilizar una línea de clock para
 *
 * @{
 */

#ifndef HAL_USART_H_
#define HAL_USART_H_

#include <stdint.h>
#include <HAL_SYSCON.h>
#include <HAL_GPIO.h>

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
 * @brief Tipo de dato para los callback en recepción de dato completa
 * @note Estos callbacks son ejecutados desde un contexto de interrupción, por lo que el usuario deberá tener
 * todas las consideraciones necesarias al respecto.
 */
typedef void (*hal_usart_rx_callback)(void);

/**
 * Tipo de dato para los callback en transmisión de dato completa
 * @note Estos callbacks son ejecutados desde un contexto de interrupción, por lo que el usuario deberá tener
 * todas las consideraciones necesarias al respecto.
 */
typedef void (*hal_usart_tx_callback)(void);

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
	hal_usart_rx_callback rx_ready_callback; /**< Callback a ejecutar en una recepción exitosa (Cuando se terminó de recibir un dato) */
	hal_usart_tx_callback tx_ready_callback; /**< Callback a ejecutar en una transmisión exitosa (Cuando se terminó de enviar un dato) */
}hal_usart_config_t;

/**
 * @brief Inicializar USART con los parámetros deseados
 * @param[in] inst Instancia de USART inicializar
 * @param[in] config Configuración deseada de la instancia
 * @pre Haber inicializado la fuente de clock a utilizar correctamente. Ver @ref SYSCON
 */
void hal_usart_init(uint8_t inst, const hal_usart_config_t * config);

/**
 * @brief Transmitir un dato mediante una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir. Puede ser de 7, 8 o 9 bits
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */
hal_usart_tx_result hal_usart_tx_data(uint8_t inst, uint32_t data);

/**
 * @brief Recibir un dato de una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Puntero a donde guardar el dato recibido
 * @return Estado de la recepción
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */
hal_usart_rx_result hal_usart_rx_data(uint8_t inst, uint32_t *data);

/**
 * @brief Registrar el callback a ser llamado en la recepcion de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejectutar cada vez que se recibe un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void hal_usart_rx_register_callback(uint8_t inst, hal_usart_rx_callback new_callback);

/**
 * @brief Registrar el callback a ser llamado una vez finalizada la transmision de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejecutar cada vez que se termina de enviar un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void hal_usart_tx_register_callback(uint8_t inst, hal_usart_tx_callback new_callback);

#endif /* HAL_USART_H_ */

/**
 * @}
 */
