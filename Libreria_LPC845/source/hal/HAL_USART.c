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

static void dummy_callback(hal_usart_sel_en d, void *e);

static void (*usart_rx_callback[])(hal_usart_sel_en, void *) = //!< Callbacks registrados a la recepcion de un dato por USART
{
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback
};

static void *usart_rx_data[] = { //!< Datos registrados a la recepcion de un dato
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
};

static void (*usart_tx_callback[])(hal_usart_sel_en, void *) = //!< Callbacks registrados a la finalizacion de transmision de un dato por USART
{
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback,
		dummy_callback
};

static void *usart_tx_data[] = { //!< Datos registrados a la transmision de un dato
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
};

static inline uint16_t hal_usart_calculate_brgval(uint32_t usart_clock, uint32_t baudrate, uint8_t oversampling);
static void hal_usart_handle_irq(uint8_t inst);

static const SYSCON_peripheral_sel_en USART_SYSCON_PER[] = {
	SYSCON_PERIPHERAL_SEL_UART0,
	SYSCON_PERIPHERAL_SEL_UART1,
	SYSCON_PERIPHERAL_SEL_UART2,
	SYSCON_PERIPHERAL_SEL_UART3,
	SYSCON_PERIPHERAL_SEL_UART4,
};

static const SYSCON_enable_clock_sel_en USART_SYSCON_CLOCK_ENABLE[] = {
	SYSCON_ENABLE_CLOCK_SEL_UART0,
	SYSCON_ENABLE_CLOCK_SEL_UART1,
	SYSCON_ENABLE_CLOCK_SEL_UART2,
	SYSCON_ENABLE_CLOCK_SEL_UART3,
	SYSCON_ENABLE_CLOCK_SEL_UART4,
};

static const SYSCON_reset_sel_en USART_SYSCON_RESET_SEL[] = {
	SYSCON_RESET_SEL_UART0,
	SYSCON_RESET_SEL_UART1,
	SYSCON_RESET_SEL_UART2,
	SYSCON_RESET_SEL_UART3,
	SYSCON_RESET_SEL_UART4,
};

static const NVIC_irq_sel_en USART_NVICS[] = {
	NVIC_IRQ_SEL_UART0,
	NVIC_IRQ_SEL_UART1,
	NVIC_IRQ_SEL_UART2,
	NVIC_IRQ_SEL_PININT6_UART3,
	NVIC_IRQ_SEL_PININT7_UART4,
};

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

	SYSCON_set_peripheral_clock_source(USART_SYSCON_PER[inst], config->clock_selection);

	aux = hal_usart_calculate_brgval(hal_syscon_peripheral_clock_get(USART_SYSCON_PER[inst]),
									config->baudrate,
									config->oversampling);

	SYSCON_enable_clock(USART_SYSCON_CLOCK_ENABLE[inst]);
	SYSCON_clear_reset(USART_SYSCON_RESET_SEL[inst]);
	NVIC_enable_interrupt(USART_NVICS[inst]);

	USART_set_OSRVAL(inst, config->oversampling);
	USART_config_data_length(inst, config->data_length);
	USART_config_parity(inst, config->parity);
	USART_config_stop_bits(inst, config->stop_bits);
	USART_set_BRGVAL(inst, aux);

	// Las interrupciones de TX se habilitaran cuando se envie algun byte

	if(config->rx_ready_callback != NULL)
	{
		USART_enable_irq_RXRDY(inst);
		hal_usart_rx_register_callback(inst, config->rx_ready_callback, config->rx_ready_data);
	}

	if(config->tx_ready_callback != NULL)
	{
		hal_usart_tx_register_callback(inst, config->tx_ready_callback, config->tx_ready_data);
	}

	USART_enable(inst);
}

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

void hal_usart_inhibit_interrupts(hal_usart_sel_en inst) {
	NVIC_disable_interrupt(USART_NVICS[inst]);
}

void hal_usart_deinhibit_interrupts(hal_usart_sel_en inst) {
	NVIC_enable_interrupt(USART_NVICS[inst]);
}

void hal_usart_rx_register_callback(hal_usart_sel_en inst, hal_usart_rx_callback new_callback, void *cb_data)
{
	if(new_callback == NULL)
	{
		USART_disable_irq_RXRDY(inst);
		usart_rx_callback[inst] = dummy_callback;
		usart_rx_data[inst] = NULL;
	}
	else
	{
		USART_enable_irq_RXRDY(inst);
		usart_rx_callback[inst] = new_callback;
		usart_rx_data[inst] = cb_data;
	}
}

void hal_usart_tx_register_callback(hal_usart_sel_en inst, hal_usart_tx_callback new_callback, void *cb_data)
{
	if(new_callback == NULL)
	{
		usart_tx_callback[inst] = dummy_callback;
		usart_tx_data[inst] = NULL;
	}
	else
	{
		// Las interrupciones de TX se habilitaran (en caso de ser necesario) en el envio de un dato
		usart_tx_callback[inst] = new_callback;
		usart_tx_data[inst] = cb_data;
	}
}

/**
 * @brief Llamado a funcion dummy para irq iniciales
 */
static void dummy_callback(hal_usart_sel_en d, void *e)
{
	(void) d;
	(void) e;
	return;
}

static inline uint16_t hal_usart_calculate_brgval(uint32_t uart_clock, uint32_t baudrate, uint8_t oversampling)
{
	return ((uart_clock) / ((oversampling + 1) * baudrate)) - 1;
}

static void hal_usart_handle_irq(uint8_t inst)
{
	if(USART_get_irq_status_RXRDY(inst) && USART_get_flag_RXRDY(inst))
	{
		uint32_t dummy_data;

		usart_rx_callback[inst](inst, usart_rx_data[inst]);

		// Limpio flag de interrupcion leyendo el registro correspondiente
		dummy_data = USART_get_data(inst);
		(void) dummy_data;
	}

	if(USART_get_irq_status_TXRDY(inst) && USART_get_flag_TXRDY(inst))
	{
		// Deshabilito interrupciones
		USART_disable_irq_TXRDY(inst);

		// Es probable que en este callback se inicie otra transmision, en cuyo caso se volveran a habilitar
		usart_tx_callback[inst](inst, usart_tx_data[inst]);
	}
}

void UART0_IRQHandler(void)
{
	hal_usart_handle_irq(0);
}

void UART1_IRQHandler(void)
{
	hal_usart_handle_irq(1);
}

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

void UART3_irq(void)
{
	hal_usart_handle_irq(3);
}

void UART4_irq(void)
{
	hal_usart_handle_irq(4);
}
