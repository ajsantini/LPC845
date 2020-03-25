/**
 * @file HAL_UART.h
 * @brief Declaraciones a nivel de aplicacion del periferico UART (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <stdint.h>
#include <HAL_SYSCON.h>

typedef enum
{
	HAL_UART_DATALEN_7BIT = 0,
	HAL_UART_DATALEN_8BIT,
	HAL_UART_DATALEN_9BIT
}hal_uart_datalen_en;

typedef enum
{
	HAL_UART_PARITY_NO_PARITY = 0,
	HAL_UART_PARITY_EVEN = 2,
	HAL_UART_PARITY_ODD
}hal_uart_parity_en;

typedef enum
{
	HAL_UART_STOPLEN_1BIT = 0,
	HAL_UART_STOPLEN_2BIT
}hal_uart_stop_en;

typedef enum
{
	HAL_UART_OVERSAMPLING_X5 = 4,
	HAL_UART_OVERSAMPLING_X6,
	HAL_UART_OVERSAMPLING_X7,
	HAL_UART_OVERSAMPLING_X8,
	HAL_UART_OVERSAMPLING_X9,
	HAL_UART_OVERSAMPLING_X10,
	HAL_UART_OVERSAMPLING_X11,
	HAL_UART_OVERSAMPLING_X12,
	HAL_UART_OVERSAMPLING_X13,
	HAL_UART_OVERSAMPLING_X14,
	HAL_UART_OVERSAMPLING_X15,
	HAL_UART_OVERSAMPLING_X16,
}hal_uart_oversampling_en;

typedef enum
{
	HAL_UART_TX_RESULT_OK = 0,
	HAL_UART_TX_RESULT_NOT_READY
}hal_uart_tx_result;

typedef enum
{
	HAL_UART_RX_RESULT_OK = 0,
	HAL_UART_RX_RESULT_NOT_READY
}hal_uart_rx_result;

typedef struct
{
	hal_uart_datalen_en data_length;
	hal_uart_parity_en parity;
	hal_uart_stop_en stop_bits;
	hal_uart_oversampling_en oversampling;
	hal_syscon_peripheral_clock_sel_en clock_selection;
	uint32_t baudrate;
	uint8_t tx_port;
	uint8_t tx_pin;
	uint8_t rx_port;
	uint8_t rx_pin;
	void (*rx_ready_callback)(void);
	void (*tx_ready_callback)(void);
}hal_uart_config_t;

/**
 * @brief Inicializar UART con los parametros deseados
 * @param[in] inst Que instancia de UART inicializar
 * @param[in] config Puntero a configuracion de la UART
 */
void hal_uart_init(uint8_t inst, const hal_uart_config_t * config);

/**
 * @brief Transmitir un dato mediante la UART
 * @param[in] inst Que instancia de UART usar
 * @param[in] data Dato a transmitir. Puede ser de 7, 8 o 9 bits
 */
hal_uart_tx_result hal_uart_tx_byte(uint8_t inst, uint32_t data);

/**
 * @brief Recibir un dato de la UART
 * @param[in] inst Que instancia de UART usar
 * @param[in] data Puntero a donde guardar el dato recibido
 * @return Estado de la recepcion
 */
hal_uart_rx_result hal_uart_rx_byte(uint8_t inst, uint32_t *data);

/**
 * @brief Registrar el callback a ser llamado una vez finalizada la transmision de un dato por UART
 * @param[in] inst A que instancia de UART registrar el callback
 * @param[in] new_callback Puntero a funcion a llamar cada vez que se termina de enviar un dato por UART
 */
void hal_uart_register_tx_callback(uint8_t inst, void (*new_callback)(void));

/**
 * @brief Registrar el callback a ser llamado en la recepcion de un dato por UART
 * @param[in] inst A que instancia de UART registrar el callback
 * @param[in] new_callback Puntero a funcion a llamar cada vez que se recibe un dato por UART
 */
void hal_uart_register_rx_callback(uint8_t inst, void (*new_callback)(void));

/**
 * @brief Interrupcion de UART3
 */
void UART3_irq(void);

/**
 * @brief Interrupcion de UART4
 */
void UART4_irq(void);

#endif /* HAL_UART_H_ */
