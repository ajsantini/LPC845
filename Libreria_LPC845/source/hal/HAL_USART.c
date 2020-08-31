/**
 * @file HAL_USART.c
 * @brief Funciones a nivel de aplicacion del periferico USART (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <stddef.h>
#include <HAL_SYSCON.h>
#include <HAL_USART.h>
#include <HPL_NVIC.h>
#include <HPL_SWM.h>
#include <HPL_SYSCON.h>
#include <HPL_USART.h>

static void dummy_callback(hal_usart_sel_en d);

static void (*usart_rx_callback[])(hal_usart_sel_en) = //!< Callbacks registrados a la recepcion de un dato por USART
{
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback
};

static void (*usart_tx_callback[])(hal_usart_sel_en) = //!< Callbacks registrados a la finalizacion de transmision de un dato por USART
{
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback
};

static inline uint16_t hal_usart_calculate_brgval(uint32_t usart_clock, uint32_t baudrate, uint8_t oversampling);
static void hal_usart_handle_irq(uint8_t inst);

/**
 * @brief Inicializar USART con los parámetros deseados
 * @param[in] inst Instancia de USART inicializar
 * @param[in] config Configuración deseada de la instancia
 * @pre Haber inicializado la fuente de clock a utilizar correctamente. Ver @ref SYSCON
 */
void hal_usart_init(hal_usart_sel_en inst, const hal_usart_config_t * config)
{
	uint32_t aux;

	SWM_init();

	if(config->rx_portpin != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_uart_RXD(inst, HAL_GPIO_PORTPIN_TO_PORT(config->rx_portpin), HAL_GPIO_PORTPIN_TO_PIN(config->rx_portpin));
	}

	if(config->tx_portpin != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_uart_TXD(inst, HAL_GPIO_PORTPIN_TO_PORT(config->tx_portpin), HAL_GPIO_PORTPIN_TO_PIN(config->tx_portpin));
	}

	SWM_deinit();

	switch(inst)
	{
	case 0:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART0, config->clock_selection);

		aux = hal_usart_calculate_brgval(hal_syscon_peripheral_clock_get(HAL_SYSCON_PERIPHERAL_SEL_UART0),
										config->baudrate,
										config->oversampling);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART0);

		SYSCON_clear_reset(SYSCON_RESET_SEL_UART0);

		NVIC_enable_interrupt(NVIC_IRQ_SEL_UART0);

		break;
	case 1:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART1, config->clock_selection);

		aux = hal_usart_calculate_brgval(hal_syscon_peripheral_clock_get(HAL_SYSCON_PERIPHERAL_SEL_UART1),
										config->baudrate,
										config->oversampling);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART1);

		SYSCON_clear_reset(SYSCON_RESET_SEL_UART1);

		NVIC_enable_interrupt(NVIC_IRQ_SEL_UART1);

		break;
	case 2:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART2, config->clock_selection);

		aux = hal_usart_calculate_brgval(hal_syscon_peripheral_clock_get(HAL_SYSCON_PERIPHERAL_SEL_UART2),
										config->baudrate,
										config->oversampling);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART2);

		SYSCON_clear_reset(SYSCON_RESET_SEL_UART2);

		NVIC_enable_interrupt(NVIC_IRQ_SEL_UART2);

		break;
	case 3:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART3, config->clock_selection);

		aux = hal_usart_calculate_brgval(hal_syscon_peripheral_clock_get(HAL_SYSCON_PERIPHERAL_SEL_UART3),
										config->baudrate,
										config->oversampling);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART3);

		SYSCON_clear_reset(SYSCON_RESET_SEL_UART3);

		NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT6_UART3);

		break;
	case 4:
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_UART4, config->clock_selection);

		aux = hal_usart_calculate_brgval(hal_syscon_peripheral_clock_get(HAL_SYSCON_PERIPHERAL_SEL_UART4),
										config->baudrate,
										config->oversampling);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_UART4);

		SYSCON_clear_reset(SYSCON_RESET_SEL_UART4);

		NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT7_UART4);

		break;
	default: { return; break; }
	}

	USART_set_OSRVAL(inst, config->oversampling);

	USART_config_data_length(inst, config->data_length);

	USART_config_parity(inst, config->parity);

	USART_config_stop_bits(inst, config->stop_bits);

	USART_set_BRGVAL(inst, aux);

	// Las interrupciones de TX se habilitaran cuando se envie algun byte

	if(config->rx_ready_callback != NULL)
	{
		USART_enable_irq_RXRDY(inst);
		hal_usart_rx_register_callback(inst, config->rx_ready_callback);
	}

	if(config->tx_ready_callback != NULL)
	{
		hal_usart_tx_register_callback(inst, config->tx_ready_callback);
	}

	USART_enable(inst);
}

/**
 * @brief Transmitir un dato mediante una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir. Puede ser de 7, 8 o 9 bits
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */
hal_usart_tx_result hal_usart_tx_data(hal_usart_sel_en inst, uint32_t data)
{
	// Chequeo si se puede enviar data
	if(USART_get_flag_TXRDY(inst))
	{
		// Escribo data
		USART_write_data(inst, data);

		if(usart_tx_callback[inst] != dummy_callback)
		{
			// Habilito interrupciones de TXRDY unicamente si se paso un callback
			USART_enable_irq_TXRDY(inst);
		}
	}
	else
	{
		return HAL_USART_TX_RESULT_NOT_READY;
	}

	return HAL_USART_TX_RESULT_OK;
}

/**
 * @brief Recibir un dato de una instancia USART
 * @param[in] inst Instancia a utilizar
 * @param[in] data Puntero a donde guardar el dato recibido
 * @return Estado de la recepción
 * @pre Haber inicializado la instancia mediante @ref hal_usart_init
 */
hal_usart_rx_result hal_usart_rx_data(hal_usart_sel_en inst, uint32_t *data)
{
	if(USART_get_flag_RXRDY(inst))
	{
		*data = USART_get_data(inst);
	}
	else
	{
		return HAL_USART_RX_RESULT_NOT_READY;
	}

	return HAL_USART_RX_RESULT_OK;
}

/**
 * @brief Registrar el callback a ser llamado en la recepcion de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejectutar cada vez que se recibe un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void hal_usart_rx_register_callback(hal_usart_sel_en inst, hal_usart_rx_callback new_callback)
{
	if(new_callback == NULL)
	{
		USART_disable_irq_RXRDY(inst);
		usart_rx_callback[inst] = dummy_callback;
	}
	else
	{
		USART_enable_irq_RXRDY(inst);
		usart_rx_callback[inst] = new_callback;
	}
}

/**
 * @brief Registrar el callback a ser llamado una vez finalizada la transmision de un dato por USART
 * @param[in] inst A que instancia de USART registrar el callback
 * @param[in] new_callback Callback a ejecutar cada vez que se termina de enviar un dato por USART
 * @note Recordar que estos callbacks se ejecutan en el contexto de una interrupción, por lo que se deberán
 * tener todas las consideraciones encesarias en el mismo.
 */
void hal_usart_tx_register_callback(hal_usart_sel_en inst, hal_usart_tx_callback new_callback)
{
	if(new_callback == NULL)
	{
		usart_tx_callback[inst] = dummy_callback;
	}
	else
	{
		// Las interrupciones de TX se habilitaran (en caso de ser necesario) en el envio de un dato
		usart_tx_callback[inst] = new_callback;
	}
}

/**
 * @brief Llamado a funcion dummy para irq iniciales
 */
static void dummy_callback(hal_usart_sel_en d)
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
static inline uint16_t hal_usart_calculate_brgval(uint32_t uart_clock, uint32_t baudrate, uint8_t oversampling)
{
	return ((uart_clock) / ((oversampling + 1) * baudrate)) - 1;
}

static void hal_usart_handle_irq(uint8_t inst)
{
	if(USART_get_irq_status_RXRDY(inst) && USART_get_flag_RXRDY(inst))
	{
		uint32_t dummy_data;

		usart_rx_callback[inst](inst);

		// Limpio flag de interrupcion leyendo el registro correspondiente
		dummy_data = USART_get_data(inst);
		(void) dummy_data;
	}

	if(USART_get_irq_status_TXRDY(inst) && USART_get_flag_TXRDY(inst))
	{
		// Deshabilito interrupciones
		USART_disable_irq_TXRDY(inst);

		// Es probable que en este callback se inicie otra transmision, en cuyo caso se volveran a habilitar
		usart_tx_callback[inst](inst);
	}
}

/**
 * @brief Interrupcion de UART0
 */
void UART0_IRQHandler(void)
{
	hal_usart_handle_irq(0);
}

/**
 * @brief Interrupcion de UART1
 */
void UART1_IRQHandler(void)
{
	hal_usart_handle_irq(1);
}

/**
 * @brief Interrupcion de UART2
 */
void UART2_IRQHandler(void)
{
	hal_usart_handle_irq(2);
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
	hal_usart_handle_irq(3);
}

/**
 * @brief Interrupcion de UART4
 */
void UART4_irq(void)
{
	hal_usart_handle_irq(4);
}
