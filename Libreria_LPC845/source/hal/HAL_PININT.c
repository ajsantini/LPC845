/**
 * @file HAL_PININT.c
 * @brief Funciones a nivel de aplicacion del periferico PININT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <stddef.h>
#include <HAL_PININT.h>
#include <HAL_USART.h>
#include <HPL_PININT.h>
#include <HPL_SYSCON.h>
#include <HPL_SWM.h>
#include <HPL_NVIC.h>

/** Cantidad de canales de *PININT* disponibles */
#define		PININT_CHANNEL_AMOUNT		(8)

/**
 * @brief Interrupcion de UART3
 */
extern void UART3_irq(void);

/**
 * @brief Interrupcion de UART4
 */
extern void UART4_irq(void);

static void dummy_irq_callback(void);

static void (*pinint_callbacks[PININT_CHANNEL_AMOUNT])(void) = { //!< Callbacks para las 8 interrupciones disponibles
		dummy_irq_callback,
		dummy_irq_callback,
		dummy_irq_callback,
		dummy_irq_callback,
		dummy_irq_callback,
		dummy_irq_callback,
		dummy_irq_callback
};

static void hal_pinint_enable_channel_irq(hal_pinint_channel_en channel);

static void hal_pinint_disable_channel_irq(hal_pinint_channel_en channel);

static void hal_pinint_handle_irq(hal_pinint_channel_en channel);

/**
 * @brief Inicialización del periférico
 */
void hal_pinint_init(void)
{
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_GPIO_INT);
}

/**
 * @brief De-Inicialización del periférico
 */
void hal_pinint_deinit(void)
{
	// Inhabilitación de canales en el periférico
	SYSCON_set_pinint_pin(HAL_PININT_CHANNEL_0, HAL_GPIO_PORTPIN_NOT_USED);
	SYSCON_set_pinint_pin(HAL_PININT_CHANNEL_1, HAL_GPIO_PORTPIN_NOT_USED);
	SYSCON_set_pinint_pin(HAL_PININT_CHANNEL_2, HAL_GPIO_PORTPIN_NOT_USED);
	SYSCON_set_pinint_pin(HAL_PININT_CHANNEL_3, HAL_GPIO_PORTPIN_NOT_USED);
	SYSCON_set_pinint_pin(HAL_PININT_CHANNEL_4, HAL_GPIO_PORTPIN_NOT_USED);
	SYSCON_set_pinint_pin(HAL_PININT_CHANNEL_5, HAL_GPIO_PORTPIN_NOT_USED);
	SYSCON_set_pinint_pin(HAL_PININT_CHANNEL_6, HAL_GPIO_PORTPIN_NOT_USED);
	SYSCON_set_pinint_pin(HAL_PININT_CHANNEL_7, HAL_GPIO_PORTPIN_NOT_USED);

	// Inhabilitación de canales en el NVIC
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT0);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT1);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT2);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT3);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT4);
	NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT5_DAC1);

	// Para el canal 6 y 7 no deshabilito interrupciones, dado que pueden ser usadas por UART3 y 4

	SYSCON_disable_clock(SYSCON_ENABLE_CLOCK_SEL_GPIO_INT);
}

/**
 * @brief Configuración de canal de *PININT*
 *
 * @note Esta función no configura el modo de detección. Ver: @ref hal_pinint_edge_detections_config y
 * @ref hal_pinint_level_detections_config
 *
 * @param[in] channel Canal a configurar
 * @param[in] portpin Puerto/pin en donde configurar el canal
 * @param[in] callback Callback a ejecutar en detección
 * @pre Haber inicializado el periférico
 */
void hal_pinint_channel_config(hal_pinint_channel_en channel, hal_gpio_portpin_en portpin, hal_pinint_callback_t callback)
{
	SYSCON_set_pinint_pin(channel, portpin);

	if(callback != NULL)
	{
		pinint_callbacks[channel] = callback;
		hal_pinint_enable_channel_irq(channel);
	}
	else
	{
		pinint_callbacks[channel] = dummy_irq_callback;
		hal_pinint_disable_channel_irq(channel);
	}
}

/**
 * @brief Configurar detecciones por flanco
 * @param[in] channel Canal a configurar
 * @param[in] edge Flancos a detectar
 * @pre Haber inicializado el periférico
 */
void hal_pinint_edge_detections_config(hal_pinint_channel_en channel, hal_pinint_edge_detections_en edge)
{
	PININT_set_interrupt_mode(channel, PININT_INTERRUPT_MODE_EDGE);

	switch(edge)
	{
	case HAL_PININT_EDGE_DETECTIONS_NONE: {	PININT_disable_falling_edge(channel); PININT_disable_rising_edge(channel); break; }
	case HAL_PININT_EDGE_DETECTIONS_RISING: {	PININT_disable_falling_edge(channel); PININT_enable_rising_edge(channel); break; }
	case HAL_PININT_EDGE_DETECTIONS_FALLING: {	PININT_enable_falling_edge(channel); PININT_disable_rising_edge(channel); break; }
	case HAL_PININT_EDGE_DETECTIONS_BOTH: {	PININT_enable_falling_edge(channel); PININT_enable_rising_edge(channel); break; }
	}
}

/**
 * @brief Configurar detecciones por nivel
 * @param[in] channel Canal a configurar
 * @param[in] level Nivel a detectar
 * @pre Haber inicializado el periférico
 */
void hal_pinint_level_detections_config(hal_pinint_channel_en channel, hal_pinint_level_detections_en level)
{
	PININT_set_interrupt_mode(channel, PININT_INTERRUPT_MODE_LEVEL);

	switch(level)
	{
	case HAL_PININT_LEVEL_DETECTIONS_NONE: { PININT_disable_level_detections(channel); break; }
	case HAL_PININT_LEVEL_DETECTIONS_HIGH: { PININT_enable_level_detections(channel); PININT_enable_high_level(channel); break; }
	case HAL_PININT_LEVEL_DETECTIONS_LOW: { PININT_enable_level_detections(channel); PININT_enable_low_level(channel); break; }
	}
}

/**
 * @brief Funcion dummy para inicializar los punteros de interrupciones
 */
static void dummy_irq_callback(void)
{
	return;
}

static void hal_pinint_enable_channel_irq(hal_pinint_channel_en channel)
{
	switch(channel)
	{
	case 0: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT0); break; }
	case 1: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT1); break; }
	case 2: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT2); break; }
	case 3: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT3); break; }
	case 4: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT4); break; }
	case 5: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT5_DAC1); break; }
	case 6: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT6_UART3); break; }
	case 7: { NVIC_enable_interrupt(NVIC_IRQ_SEL_PININT7_UART4); break; }
	}
}


static void hal_pinint_disable_channel_irq(hal_pinint_channel_en channel)
{
	switch(channel)
	{
	case 0: { NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT0); break; }
	case 1: { NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT1); break; }
	case 2: { NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT2); break; }
	case 3: { NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT3); break; }
	case 4: { NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT4); break; }
	case 5: { NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT5_DAC1); break; }
	case 6: { NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT6_UART3); break; }
	case 7: { NVIC_disable_interrupt(NVIC_IRQ_SEL_PININT7_UART4); break; }
	}
}

/**
 * @brief Manejo de interrupciones para el modulo
 * @param[in] channel Canal que generó la itnerrupción
 */
static void hal_pinint_handle_irq(hal_pinint_channel_en channel)
{
	pinint_callbacks[channel]();

	if(PININT_get_interrupt_mode(channel) == PININT_INTERRUPT_MODE_EDGE)
	{
		PININT_clear_edge_level_irq(channel);
	}
}

/**
 * @brief Interrupción para PININT0
 */
void PININT0_IRQHandler(void)
{
	hal_pinint_handle_irq(0);
}

/**
 * @brief Interrupción para PININT1
 */
void PININT1_IRQHandler(void)
{
	hal_pinint_handle_irq(1);
}

/**
 * @brief Interrupción para PININT2
 */
void PININT2_IRQHandler(void)
{
	hal_pinint_handle_irq(2);
}

/**
 * @brief Interrupción para PININT3
 */
void PININT3_IRQHandler(void)
{
	hal_pinint_handle_irq(3);
}

/**
 * @brief Interrupción para PININT4
 */
void PININT4_IRQHandler(void)
{
	hal_pinint_handle_irq(4);
}

/**
 * @brief Interrupción para PININT5
 */
void PININT5_IRQHandler(void)
{
	hal_pinint_handle_irq(5);
}

/**
 * @brief Interrupción para PININT6 y USART3
 */
void PININT6_IRQHandler(void)
{
	if(PININT->IST.PSTAT & (1 << 6))
	{
		hal_pinint_handle_irq(6);
	}

	UART3_irq();
}

/**
 * @brief Interrupción para PININT7 y USART4
 */
void PININT7_IRQHandler(void)
{
	if(PININT->IST.PSTAT & (1 << 7))
	{
		hal_pinint_handle_irq(7);
	}

	UART4_irq();
}
