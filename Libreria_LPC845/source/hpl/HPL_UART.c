/**
 * @file HPL_UART.c
 * @brief Funciones a nivel de abstraccion de periferico para el UART (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <HRI_UART.h>
#include <HPL_UART.h>

volatile UART_per_t * const UART[] = { //!< Perifericos USART
		(UART_per_t *) UART0_BASE,
		(UART_per_t *) UART1_BASE,
		(UART_per_t *) UART2_BASE,
		(UART_per_t *) UART3_BASE,
		(UART_per_t *) UART4_BASE
};
