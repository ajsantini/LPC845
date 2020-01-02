/**
 * @file HPL_UART.h
 * @brief Declaraciones a nivel de abstraccion de periferico del UART (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_UART_H_
#define HPL_UART_H_

#include <stdint.h>

#define	UART_INIT_SUCCESS			0
#define	UART_INIT_INVALID_UART		-1
#define	UART_INIT_INVALID_PORT		-2
#define	UART_INIT_INVALID_PIN		-3
#define	UART_INIT_INVALID_PORTPIN	-4
#define	UART_INIT_CLOCK_UNDERFLOW	-5

#define	UART_TX_BYTE_SUCCESS		0
#define	UART_TX_BYTE_INVALID_UART	-1
#define	UART_TX_BYTE_NOT_AVAILABLE	-2
#define	UART_TX_BYTE_INVALID_BITS	-3

#define	UART_RX_BYTE_SUCCESS		0
#define	UART_RX_BYTE_INVALID_UART	-1
#define	UART_RX_BYTE_NOT_AVAILABLE	-2
#define	UART_RX_BYTE_INVALID_BITS	-3

typedef enum
{
	UART_OVERSAMPLING_X5 = 4,
	UART_OVERSAMPLING_X6,
	UART_OVERSAMPLING_X7,
	UART_OVERSAMPLING_X8,
	UART_OVERSAMPLING_X9,
	UART_OVERSAMPLING_X10,
	UART_OVERSAMPLING_X11,
	UART_OVERSAMPLING_X12,
	UART_OVERSAMPLING_X13,
	UART_OVERSAMPLING_X14,
	UART_OVERSAMPLING_X15,
	UART_OVERSAMPLING_X16,
}UART_oversampling_en;

typedef enum
{
	UART_DATALEN_7BIT = 0,
	UART_DATALEN_8BIT,
	UART_DATALEN_9BIT
}UART_datalen_en;

typedef enum
{
	UART_PARITY_NO_PARITY = 0,
	UART_PARITY_EVEN = 2,
	UART_PARITY_ODD
}UART_parity_en;

typedef enum
{
	UART_STOPLEN_1BIT = 0,
	UART_STOPLEN_2BIT
}UART_stop_en;

typedef struct
{
	uint8_t data_length;
	uint8_t parity;
	uint8_t stop_bits;
	uint8_t oversampling;
	uint32_t baudrate;
	uint32_t tx_port;
	uint32_t tx_pin;
	uint32_t rx_port;
	uint32_t rx_pin;
	void (*rx_ready_callback)(void);
	void (*tx_ready_callback)(void);
}UART_config_t;

/**
 * @brief Inicializar UART.
 * @param[in] uart_selection Que instancia de UART inicializar.
 * @param[in] config Puntero a configuracion de la UART.
 * @return Estado de la inicializacion de la UART.
 */
int32_t UART_init(uint8_t uart_selection, UART_config_t *config);

/**
 * @brief Transmitir un dato mediante la UART.
 * @param[in] uart_selection Que instancia de UART usar.
 * @param[in] data Dato a transmitir. Puede ser de 7, 8 o 9 bits.
 * @return Estado de la transmision.
 */
int32_t UART_tx_byte(uint32_t uart_selection, uint32_t data);

/**
 * @brief Recibir un dato de la UART.
 * @param[in] uart_selection Que instancia de UART usar.
 * @param[in] data Puntero a donde guardar el dato recibido.
 * @return Estado de la recepcion.
 */
int32_t UART_rx_byte(uint32_t uart_selection, uint32_t *data);

/**
 * @brief Registrar el callback a ser llamado una vez finalizada la transmision de un dato por UART.
 * @param[in] uart_selection A que instancia de UART registrar el callback.
 * @param[in] new_callback Puntero a funcion a llamar cada vez que se termina de enviar un dato por UART.
 */
void UART_register_tx_callback(uint32_t uart_selection, void (*new_callback)(void));

/**
 * @brief Registrar el callback a ser llamado en la recepcion de un dato por UART.
 * @param[in] uart_selection A que instancia de UART registrar el callback.
 * @param[in] new_callback Puntero a funcion a llamar cada vez que se recibe un dato por UART.
 */
void UART_register_rx_callback(uint32_t uart_selection, void (*new_callback)(void));

#endif /* HPL_UART_H_ */
