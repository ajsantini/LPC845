/**
 * @file HRI_UART.c
 * @brief Declaración del periférico UART (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_USART.h>

volatile USART_per_t * const USART[] = { //!< Perifericos USART
		(USART_per_t *) UART0_BASE,
		(USART_per_t *) UART1_BASE,
		(USART_per_t *) UART2_BASE,
		(USART_per_t *) UART3_BASE,
		(USART_per_t *) UART4_BASE
};
