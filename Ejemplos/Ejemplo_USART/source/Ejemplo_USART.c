#include <HAL_USART.h>

/** Instancia de USART a utilizar */
#define		UART_NUMBER			0

/** Baudrate de USART a utilizar */
#define		UART_BAUDRATE		115200

/** Puerto/pin en donde recibir datos por USART */
#define		UART_RX_PORTPIN		HAL_GPIO_PORTPIN_0_24

/** Puerto/pin en donde transmitir datos por USART */
#define		UART_TX_PORTPIN		HAL_GPIO_PORTPIN_0_25

static void rx_callback(void);

static void tx_callback(void);

/** Estructura de configuación de la USART */
static const hal_usart_config_t uart_config =
{
	.data_length = HAL_USART_DATALEN_8BIT,
	.parity = HAL_USART_PARITY_NO_PARITY,
	.stop_bits = HAL_USART_STOPLEN_1BIT,
	.oversampling = HAL_USART_OVERSAMPLING_X16,
	.clock_selection = HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRG0,
	.baudrate = UART_BAUDRATE,
	.tx_portpin = UART_TX_PORTPIN,
	.rx_portpin = UART_RX_PORTPIN,
	.tx_ready_callback = tx_callback,
	.rx_ready_callback = rx_callback
};

/**
 * @brief Punto de entrada del programa
 * @return Nunca deberia terminar esta función
 */
int main(void)
{
	// Configuro el Main Clock como el FRO @24MHz
	hal_syscon_fro_clock_config(1);
	hal_syscon_system_clock_set_source(HAL_SYSCON_SYSTEM_CLOCK_SEL_FRO);

	// Configuro el clock fraccional para poder tener buena presicion para un baudrate de 115200bps
	hal_syscon_frg_config(0, HAL_SYSCON_FRG_CLOCK_SEL_MAIN_CLOCK, 47);

	// Inicialización de USART
	hal_usart_init(UART_NUMBER, &uart_config);

	while(1)
	{

	} // End while(1)

	return 0;
}

/** Trama de respuesta automatica para responder por serie */
static char trama[] = "Trama de respuesta automatica\n";

/** Contador de caracter de trama */
static uint32_t trama_counter = 0;

/**
 * @brief Callback a ejecutar en recepcion de dato por USART
 */
static void rx_callback(void)
{
	uint32_t data;

	hal_usart_rx_data(UART_NUMBER, &data);

	if((char) data == 'S')
	{
		hal_usart_tx_data(UART_NUMBER, trama[trama_counter++]);
	}
}

/**
 * @brief Callback a ejecutar en finalizacion de transmision de dato por USART
 */
static void tx_callback(void)
{
	if(trama[trama_counter] != '\0')
	{
		hal_usart_tx_data(UART_NUMBER, trama[trama_counter++]);
	}
	else
	{
		trama_counter = 0;
	}
}
