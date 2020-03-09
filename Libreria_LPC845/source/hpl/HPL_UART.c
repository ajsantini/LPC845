/**
 * @file HPL_UART.c
 * @brief Funciones a nivel de abstraccion de periferico para el UART (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdio.h>
#include <stdint.h>
#include <HPL_UART.h>
#include <HPL_SYSCON.h>
#include <HRI_SYSCON.h>
#include <HRI_SWM.h>
#include <HRI_NVIC.h>
#include <HRI_UART.h>

volatile UART_per_t * const UART[5] = { //!< Perifericos USART
		(UART_per_t *) UART0_BASE,
		(UART_per_t *) UART1_BASE,
		(UART_per_t *) UART2_BASE,
		(UART_per_t *) UART3_BASE,
		(UART_per_t *) UART4_BASE
};

static void (*uart_rx_callback[5])(void) = { //!< Callbacks registrados a la recepcion de un dato por UART
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
};

static void (*uart_tx_callback[5])(void) = { //!< Callbacks registrados a la finalizacion de transmision de un dato por UART
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
};

static uint16_t calculate_BRGVAL(uint32_t uart_clock, uint32_t baudrate, uint8_t oversampling);

/**
 * @brief Registrar el callback a ser llamado en la recepcion de un dato por UART.
 * @param[in] uart_selection A que instancia de UART registrar el callback.
 * @param[in] new_callback Puntero a funcion a llamar cada vez que se recibe un dato por UART.
 */
void UART_register_rx_callback(uint32_t uart_selection, void (*new_callback)(void))
{
	uart_rx_callback[uart_selection] = new_callback;
}

/**
 * @brief Registrar el callback a ser llamado una vez finalizada la transmision de un dato por UART.
 * @param[in] uart_selection A que instancia de UART registrar el callback.
 * @param[in] new_callback Puntero a funcion a llamar cada vez que se termina de enviar un dato por UART.
 */
void UART_register_tx_callback(uint32_t uart_selection, void (*new_callback)(void))
{
	uart_tx_callback[uart_selection] = new_callback;
}

/**
 * @brief Inicializar UART.
 * @param[in] uart_selection Que instancia de UART inicializar.
 * @param[in] config Puntero a configuracion de la UART.
 * @return Estado de la inicializacion de la UART.
 */
int32_t UART_init(uint8_t uart_selection, const UART_config_t * const config)
{
	uint32_t aux;

	if(uart_selection > 4)
	{
		return UART_INIT_INVALID_UART;
	}

	if(config->rx_port > 1 || config->tx_port)
	{
		return UART_INIT_INVALID_PORT;
	}

	if(config->rx_pin > 31 || config->tx_pin > 31)
	{
		return UART_INIT_INVALID_PIN;
	}

	if((config->rx_port == 1 && config->rx_pin > 9) ||
			(config->tx_port == 1 && config->tx_pin > 9))
	{
		return UART_INIT_INVALID_PORTPIN;
	}

	// Habilitacion de la Switch-Matrix
	SYSCON->SYSAHBCLKCTRL0.SWM = 1;

	switch(uart_selection)
	{
	case 0:
		// Seleccion de clock para la UART
		SYSCON_set_peripheral_clock_source(PERIPHERAL_SELECTION_UART0, config->clock_selection);

		aux = calculate_BRGVAL(SYSCON_get_peripheral_clock(PERIPHERAL_SELECTION_UART0),
								config->baudrate,
								config->oversampling);

		if(aux > 0xFFF)
		{
			return UART_INIT_CLOCK_UNDERFLOW;
		}

		// Habilitacion en el SYSAHBCLKCTRL register
		SYSCON->SYSAHBCLKCTRL0.UART0 = 1;

		// Limpiar los resets de las USARTS en PRESETCTRL register
		SYSCON->PRESETCTRL0.UART0_RST_N = 1;

		// Habilitacion de interrupciones en NVIC
		NVIC->ISER0.ISE_UART0 = 1;

		// Configuracion de pines
		SWM->PINASSIGN0.U0_RXD_I = (config->rx_port * 32) + config->rx_pin;
		SWM->PINASSIGN0.U0_TXD_O = (config->tx_port * 32) + config->tx_pin;

		break;
	case 1:
		// Seleccion de clock para la UART
		SYSCON_set_peripheral_clock_source(PERIPHERAL_SELECTION_UART1, config->clock_selection);

		aux = calculate_BRGVAL(SYSCON_get_peripheral_clock(PERIPHERAL_SELECTION_UART1),
								config->baudrate,
								config->oversampling);

		if(aux > 0xFFF)
		{
			return UART_INIT_CLOCK_UNDERFLOW;
		}

		// Habilitacion en el SYSAHBCLKCTRL register
		SYSCON->SYSAHBCLKCTRL0.UART1 = 1;

		// Limpiar los resets de las USARTS en PRESETCTRL register
		SYSCON->PRESETCTRL0.UART1_RST_N = 1;

		// Habilitacion de interrupciones en NVIC
		NVIC->ISER0.ISE_UART1 = 1;

		// Configuracion de pines
		SWM->PINASSIGN1.U1_RXD_I = (config->rx_port * 32) + config->rx_pin;
		SWM->PINASSIGN1.U1_TXD_O = (config->tx_port * 32) + config->tx_pin;

		break;
	case 2:
		// Seleccion de clock para la UART
		SYSCON_set_peripheral_clock_source(PERIPHERAL_SELECTION_UART2, config->clock_selection);

		aux = calculate_BRGVAL(SYSCON_get_peripheral_clock(PERIPHERAL_SELECTION_UART2),
								config->baudrate,
								config->oversampling);

		if(aux > 0xFFF)
		{
			return UART_INIT_CLOCK_UNDERFLOW;
		}

		// Habilitacion en el SYSAHBCLKCTRL register
		SYSCON->SYSAHBCLKCTRL0.UART2 = 1;

		// Limpiar los resets de las USARTS en PRESETCTRL register
		SYSCON->PRESETCTRL0.UART2_RST_N = 1;

		// Habilitacion de interrupciones en NVIC
		NVIC->ISER0.ISE_UART2 = 1;

		// Configuracion de pines
		SWM->PINASSIGN2.U2_RXD_I = (config->rx_port * 32) + config->rx_pin;
		SWM->PINASSIGN2.U2_TXD_O = (config->tx_port * 32) + config->tx_pin;

		break;
	case 3:
		// Seleccion de clock para la UART
		SYSCON_set_peripheral_clock_source(PERIPHERAL_SELECTION_UART3, config->clock_selection);

		aux = calculate_BRGVAL(SYSCON_get_peripheral_clock(PERIPHERAL_SELECTION_UART3),
								config->baudrate,
								config->oversampling);

		if(aux > 0xFFF)
		{
			return UART_INIT_CLOCK_UNDERFLOW;
		}

		// Habilitacion en el SYSAHBCLKCTRL register
		SYSCON->SYSAHBCLKCTRL0.UART3 = 1;

		// Limpiar los resets de las USARTS en PRESETCTRL register
		SYSCON->PRESETCTRL0.UART3_RST_N = 1;

		// Habilitacion de interrupciones en NVIC
		NVIC->ISER0.ISE_PININT6 = 1;

		// Configuracion de pines
		SWM->PINASSIGN12.UART3_RXD = (config->rx_port * 32) + config->rx_pin;
		SWM->PINASSIGN11.UART3_TXD = (config->tx_port * 32) + config->tx_pin;

		break;
	case 4:
		// Seleccion de clock para la UART
		SYSCON_set_peripheral_clock_source(PERIPHERAL_SELECTION_UART4, config->clock_selection);

		aux = calculate_BRGVAL(SYSCON_get_peripheral_clock(PERIPHERAL_SELECTION_UART4),
								config->baudrate,
								config->oversampling);

		if(aux > 0xFFF)
		{
			return UART_INIT_CLOCK_UNDERFLOW;
		}

		// Habilitacion en el SYSAHBCLKCTRL register
		SYSCON->SYSAHBCLKCTRL0.UART4 = 1;

		// Limpiar los resets de las USARTS en PRESETCTRL register
		SYSCON->PRESETCTRL0.UART4_RST_N = 1;

		// Habilitacion de interrupciones en NVIC
		NVIC->ISER0.ISE_PININT7 = 1;

		// Configuracion de pines
		SWM->PINASSIGN12.UART4_RXD = (config->rx_port * 32) + config->rx_pin;
		SWM->PINASSIGN12.UART4_TXD = (config->tx_port * 32) + config->tx_pin;

		break;
	default:
		// Inhabilitacion de la Switch-Matrix
		SYSCON->SYSAHBCLKCTRL0.SWM = 0;

		return UART_INIT_INVALID_UART;
		break;
	}

	// Inhabilitacion de la Switch-Matrix
	SYSCON->SYSAHBCLKCTRL0.SWM = 0;

	// Calculo para poner en los registros del baudrate
	UART[uart_selection]->OSR.OSRVAL = config->oversampling;

	// Largo de caracter
	UART[uart_selection]->CFG.DATALEN = config->data_length;

	// Paridad
	UART[uart_selection]->CFG.PARITYSEL = config->parity;

	// Bits de stop
	UART[uart_selection]->CFG.STOPLEN = config->stop_bits;

	// Baud rate
	UART[uart_selection]->BRG.BRGVAL = aux;

	// Habilitacion de interrupciones de RX
	UART[uart_selection]->INTENSET.RXRDYEN = 1;

	// Las interrupciones de TX se habilitaran cuando se envie algun byte

	// Callbacks de interrupciones
	UART_register_rx_callback(uart_selection, config->rx_ready_callback);
	UART_register_tx_callback(uart_selection, config->tx_ready_callback);

	// Habilitacion de UART
	UART[uart_selection]->CFG.ENABLE = 1;

	return UART_INIT_SUCCESS;
}

/**
 * @brief Calculo del valor para el registro de Baudrate.
 * @param[in] uart_clock Clock asociado con la UART.
 * @param[in] baudrate Baudrate deseado a calcular.
 * @param[in] oversampling Oversampling deseado para la UART.
 * @return Valor a poner en el registro BRG.
 */
static uint16_t calculate_BRGVAL(uint32_t uart_clock, uint32_t baudrate, uint8_t oversampling)
{
	return ((uart_clock) / ((oversampling + 1) * baudrate)) - 1;
}

/**
 * @brief Transmitir un dato mediante la UART.
 * @param[in] uart_selection Que instancia de UART usar.
 * @param[in] data Dato a transmitir. Puede ser de 7, 8 o 9 bits.
 * @return Estado de la transmision.
 */
int32_t UART_tx_byte(uint32_t uart_selection, uint32_t data)
{
	if(uart_selection > 4)
	{
		return UART_TX_BYTE_INVALID_UART;
	}

	// Chequeo si se puede enviar data
	if(UART[uart_selection]->STAT.TXRDY)
	{
		// Escribo data
		switch(UART[uart_selection]->CFG.DATALEN)
		{
		case UART_DATALEN_7BIT:
			UART[uart_selection]->TXDAT.TXDAT = data & 0x007F;
			break;
		case UART_DATALEN_8BIT:
			UART[uart_selection]->TXDAT.TXDAT = data & 0x00FF;
			break;
		case UART_DATALEN_9BIT:
			UART[uart_selection]->TXDAT.TXDAT = data & 0x01FF;
			break;
		default:
			return UART_TX_BYTE_INVALID_BITS;
		}

		// Habilito interrupciones de TXRDY
		UART[uart_selection]->INTENSET.TXRDYEN = 1;
	}
	else
	{
		return UART_TX_BYTE_NOT_AVAILABLE;
	}

	return UART_TX_BYTE_SUCCESS;
}

/**
 * @brief Recibir un dato de la UART.
 * @param[in] uart_selection Que instancia de UART usar.
 * @param[in] data Puntero a donde guardar el dato recibido.
 * @return Estado de la recepcion.
 */
int32_t UART_rx_byte(uint32_t uart_selection, uint32_t *data)
{
	if(uart_selection > 4)
	{
		return UART_RX_BYTE_INVALID_UART;
	}

	if(UART[uart_selection]->STAT.RXRDY)
	{
		switch(UART[uart_selection]->CFG.DATALEN)
		{
		case UART_DATALEN_7BIT:
			*data = UART[uart_selection]->RXDAT.RXDAT & 0x007F;
			break;
		case UART_DATALEN_8BIT:
			*data = UART[uart_selection]->RXDAT.RXDAT & 0x00FF;
			break;
		case UART_DATALEN_9BIT:
			*data = UART[uart_selection]->RXDAT.RXDAT & 0x01FF;
			break;
		default:
			return UART_RX_BYTE_INVALID_BITS;
		}
	}
	else
	{
		return UART_RX_BYTE_NOT_AVAILABLE;
	}

	return UART_RX_BYTE_SUCCESS;
}

/**
 * @brief Interrupcion de UART0
 */
void UART0_IRQHandler(void)
{
	if(UART[0]->STAT.RXRDY)
	{
		if(uart_rx_callback[0] != NULL)
		{
			uart_rx_callback[0]();
		}

		// Limpio flag de interrupcion leyendo el registro correspondiente
		(void) UART[0]->RXDAT.RXDAT;
	}

	if(UART[0]->STAT.TXRDY)
	{
		// Deshabilito interrupciones
		UART[0]->INTENCLR.TXRDYCLR = 1;

		if(uart_tx_callback[0] != NULL)
		{
			uart_tx_callback[0]();
		}
	}
}

/**
 * @brief Interrupcion de UART1
 */
void UART1_IRQHandler(void)
{
	if(UART[1]->STAT.RXRDY)
	{
		if(uart_rx_callback[1] != NULL)
		{
			uart_rx_callback[1]();
		}

		// Limpio flag de interrupcion leyendo el registro correspondiente
		(void) UART[1]->RXDAT.RXDAT;
	}

	if(UART[1]->STAT.TXRDY)
	{
		// Deshabilito interrupciones
		UART[1]->INTENCLR.TXRDYCLR = 1;

		if(uart_tx_callback[1] != NULL)
		{
			uart_tx_callback[1]();
		}
	}
}

/**
 * @brief Interrupcion de UART2
 */
void UART2_IRQHandler(void)
{
	if(UART[2]->STAT.RXRDY)
	{
		if(uart_rx_callback[2] != NULL)
		{
			uart_rx_callback[2]();
		}

		// Limpio flag de interrupcion leyendo el registro correspondiente
		(void) UART[2]->RXDAT.RXDAT;
	}

	if(UART[2]->STAT.TXRDY)
	{
		// Deshabilito interrupciones
		UART[2]->INTENCLR.TXRDYCLR = 1;

		if(uart_tx_callback[2] != NULL)
		{
			uart_tx_callback[2]();
		}
	}
}

/*
 * NOTA IMPORTANTE:
 *
 * Las interrupciones de las USART 3 y 4 se encuentran en el archivo DR_PININT.c
 * ya que comparten las interrupciones con interrupciones de GPIO.
 *
 * Lo que hago es en ese archivo, llamar a funciones de este, para que quede mas ordenado.
 *
 * En estas dos ultimas, voy a tener que revisar si la interrupcion fue a causa de la UART
 * o fue llamada porque tambien estaba configurada alguna PININT.
 */

/**
 * @brief Interrupcion de UART3
 */
void UART3_irq(void)
{
	if(UART[3]->STAT.RXRDY)
	{
		if(uart_rx_callback[3] != NULL)
		{
			uart_rx_callback[3]();
		}

		// Limpio flag de interrupcion leyendo el registro correspondiente
		(void) UART[3]->RXDAT.RXDAT;
	}

	if(UART[3]->STAT.TXRDY)
	{
		// Deshabilito interrupciones
		UART[3]->INTENCLR.TXRDYCLR = 1;

		if(uart_tx_callback[3] != NULL)
		{
			uart_tx_callback[3]();
		}
	}
}

/**
 * @brief Interrupcion de UART4
 */
void UART4_irq(void)
{
	if(UART[4]->STAT.RXRDY)
	{
		if(uart_rx_callback[4] != NULL)
		{
			uart_rx_callback[4]();
		}

		// Limpio flag de interrupcion leyendo el registro correspondiente
		(void) UART[4]->RXDAT.RXDAT;
	}

	if(UART[4]->STAT.TXRDY)
	{
		// Deshabilito interrupciones
		UART[4]->INTENCLR.TXRDYCLR = 1;

		if(uart_tx_callback[4] != NULL)
		{
			uart_tx_callback[4]();
		}
	}
}
