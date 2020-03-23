/**
 * @file HAL_UART.c
 * @brief Funciones a nivel de aplicacion del periferico UART (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_UART.h>
#include <HAL_SYSCON.h>
#include <HPL_UART.h>
#include <HPL_NVIC.h>
#include <HPL_SWM.h>
#include <HPL_SYSCON.h>

static void dummy_callback(void);

static void (*uart_rx_callback[])(void) = //!< Callbacks registrados a la recepcion de un dato por UART
{
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback
};

static void (*uart_tx_callback[])(void) = //!< Callbacks registrados a la finalizacion de transmision de un dato por UART
{
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback
};

static inline uint16_t uart_calculate_brgval(uint32_t uart_clock, uint32_t baudrate, uint8_t oversampling);
static void uart_handle_irq(uint8_t inst);

/**
 * @brief Inicializar UART con los parametros deseados
 * @param[in] inst Que instancia de UART inicializar
 * @param[in] config Puntero a configuracion de la UART
 */
void hal_uart_init(uint8_t inst, const hal_uart_config_t * config)
{
	uint32_t aux;

	SWM_init();
	SWM_assign_uart_RXD(inst, config->rx_port, config->rx_pin);
	SWM_assign_uart_TXD(inst, config->tx_port, config->tx_pin);
	SWM_deinit();

	switch(inst)
	{
	case 0:
		// Seleccion de clock para la UART
		hal_syscon_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART0, config->clock_selection);

		aux = uart_calculate_brgval(hal_syscon_get_peripheral_clock(HAL_SYSCON_PERIPHERAL_SEL_UART0),
								config->baudrate,
								config->oversampling);

		// Habilitacion en el SYSAHBCLKCTRL register
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART0);

		// Limpiar los resets de las USARTS en PRESETCTRL register
		SYSCON_clear_reset(SYSCON_RESET_SEL_UART0);

		// Habilitacion de interrupciones en NVIC
		NVIC_enable_interrupt(NVIC_IRQ_SEL_UART0);

		break;
	case 1:
		// Seleccion de clock para la UART
		hal_syscon_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART1, config->clock_selection);

		aux = uart_calculate_brgval(hal_syscon_get_peripheral_clock(HAL_SYSCON_PERIPHERAL_SEL_UART1),
								config->baudrate,
								config->oversampling);

		// Habilitacion en el SYSAHBCLKCTRL register
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART1);

		// Limpiar los resets de las USARTS en PRESETCTRL register
		SYSCON_clear_reset(SYSCON_RESET_SEL_UART1);

		// Habilitacion de interrupciones en NVIC
		NVIC_enable_interrupt(NVIC_IRQ_SEL_UART1);

		break;
	case 2:
		// Seleccion de clock para la UART
		hal_syscon_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART2, config->clock_selection);

		aux = uart_calculate_brgval(hal_syscon_get_peripheral_clock(HAL_SYSCON_PERIPHERAL_SEL_UART2),
								config->baudrate,
								config->oversampling);

		// Habilitacion en el SYSAHBCLKCTRL register
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART2);

		// Limpiar los resets de las USARTS en PRESETCTRL register
		SYSCON_clear_reset(SYSCON_RESET_SEL_UART2);

		// Habilitacion de interrupciones en NVIC
		NVIC_enable_interrupt(NVIC_IRQ_SEL_UART2);

		break;
	case 3:
		// Seleccion de clock para la UART
		hal_syscon_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART3, config->clock_selection);

		aux = uart_calculate_brgval(hal_syscon_get_peripheral_clock(HAL_SYSCON_PERIPHERAL_SEL_UART3),
								config->baudrate,
								config->oversampling);

		// Habilitacion en el SYSAHBCLKCTRL register
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART3);

		// Limpiar los resets de las USARTS en PRESETCTRL register
		SYSCON_clear_reset(SYSCON_RESET_SEL_UART3);

		// Habilitacion de interrupciones en NVIC
		NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT6_UART3);

		break;
	case 4:
		// Seleccion de clock para la UART
		hal_syscon_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART4, config->clock_selection);

		aux = uart_calculate_brgval(hal_syscon_get_peripheral_clock(HAL_SYSCON_PERIPHERAL_SEL_UART4),
								config->baudrate,
								config->oversampling);

		// Habilitacion en el SYSAHBCLKCTRL register
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART4);

		// Limpiar los resets de las USARTS en PRESETCTRL register
		SYSCON_clear_reset(SYSCON_RESET_SEL_UART4);

		// Habilitacion de interrupciones en NVIC
		NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT7_UART4);

		break;
	default: { return; break; }
	}

	// Oversampling
	UART_set_OSRVAL(inst, config->oversampling);

	// Largo de caracter
	UART_config_data_length(inst, config->data_length);

	// Paridad
	UART_config_parity(inst, config->parity);

	// Bits de stop
	UART_config_stop_bits(inst, config->stop_bits);

	// Baud rate
	UART_set_BRGVAL(inst, aux);

	// Habilitacion de interrupciones de RX
	UART_enable_irq_RXRDY(inst);

	// Las interrupciones de TX se habilitaran cuando se envie algun byte

	// Callbacks de interrupciones
	hal_uart_register_rx_callback(inst, config->rx_ready_callback);
	hal_uart_register_tx_callback(inst, config->tx_ready_callback);

	// Habilitacion de UART
	UART_enable(inst);
}

/**
 * @brief Transmitir un dato mediante la UART
 * @param[in] inst Que instancia de UART usar
 * @param[in] data Dato a transmitir. Puede ser de 7, 8 o 9 bits
 */
hal_uart_tx_result hal_uart_tx_byte(uint8_t inst, uint32_t data)
{
	// Chequeo si se puede enviar data
	if(UART_get_flag_TXRDY(inst))
	{
		// Escribo data
		UART_write_data(inst, data);

		// Habilito interrupciones de TXRDY
		UART_enable_irq_TXRDY(inst);
	}
	else
	{
		return HAL_UART_TX_RESULT_NOT_READY;
	}

	return HAL_UART_TX_RESULT_OK;
}

/**
 * @brief Recibir un dato de la UART
 * @param[in] inst Que instancia de UART usar
 * @param[in] data Puntero a donde guardar el dato recibido
 * @return Estado de la recepcion
 */
hal_uart_rx_result hal_uart_rx_byte(uint8_t inst, uint32_t *data)
{
	if(UART_get_flag_RXRDY(inst))
	{
		*data = UART_get_data(inst);
	}
	else
	{
		return HAL_UART_RX_RESULT_NOT_READY;
	}

	return HAL_UART_RX_RESULT_OK;
}

/**
 * @brief Registrar el callback a ser llamado en la recepcion de un dato por UART
 * @param[in] inst A que instancia de UART registrar el callback
 * @param[in] new_callback Puntero a funcion a llamar cada vez que se recibe un dato por UART
 */
void hal_uart_register_rx_callback(uint8_t inst, void (*new_callback)(void))
{
	uart_rx_callback[inst] = new_callback;
}

/**
 * @brief Registrar el callback a ser llamado una vez finalizada la transmision de un dato por UART
 * @param[in] inst A que instancia de UART registrar el callback
 * @param[in] new_callback Puntero a funcion a llamar cada vez que se termina de enviar un dato por UART
 */
void hal_uart_register_tx_callback(uint8_t inst, void (*new_callback)(void))
{
	uart_tx_callback[inst] = new_callback;
}

/**
 * @brief Llamado a funcion dummy para irq iniciales
 */
static void dummy_callback(void)
{
	return;
}

/**
 * @brief Calculo del valor para el registro de Baudrate.
 * @param[in] uart_clock Clock asociado con la UART.
 * @param[in] baudrate Baudrate deseado a calcular.
 * @param[in] oversampling Oversampling deseado para la UART.
 * @return Valor a poner en el registro BRG.
 */
static inline uint16_t uart_calculate_brgval(uint32_t uart_clock, uint32_t baudrate, uint8_t oversampling)
{
	return ((uart_clock) / ((oversampling + 1) * baudrate)) - 1;
}

static void uart_handle_irq(uint8_t inst)
{
	if(UART_get_irq_status_RXRDY(inst) && UART_get_flag_RXRDY(inst))
	{
		uint32_t dummy_data;

		uart_rx_callback[inst]();

		// Limpio flag de interrupcion leyendo el registro correspondiente
		dummy_data = UART_get_data(inst);
		(void) dummy_data;
	}

	if(UART_get_irq_status_TXRDY(inst) && UART_get_flag_TXRDY(inst))
	{
		// Deshabilito interrupciones
		UART_disable_irq_TXRDY(inst);

		// Es probable que en este callback se inicie otra transmision, en cuyo caso se volveran a habilitar
		uart_tx_callback[inst]();
	}
}

/**
 * @brief Interrupcion de UART0
 */
void UART0_IRQHandler(void)
{
	uart_handle_irq(0);
}

/**
 * @brief Interrupcion de UART1
 */
void UART1_IRQHandler(void)
{
	uart_handle_irq(1);
}

/**
 * @brief Interrupcion de UART2
 */
void UART2_IRQHandler(void)
{
	uart_handle_irq(2);
}

/*
 * NOTA IMPORTANTE:
 *
 * Las interrupciones de las USART 3 y 4 se encuentran en el archivo HAL_PININT.c
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
	uart_handle_irq(3);
}

/**
 * @brief Interrupcion de UART4
 */
void UART4_irq(void)
{
	uart_handle_irq(4);
}
