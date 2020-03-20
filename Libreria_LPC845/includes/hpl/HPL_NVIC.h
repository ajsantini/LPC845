/**
 * @file HPL_NVIC.h
 * @brief Declaraciones a nivel de abstraccion de periferico del NVIC (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HPL_NVIC_H_
#define HPL_NVIC_H_

#include <stdint.h>
#include <HRI_NVIC.h>

typedef enum
{
	NVIC_IRQ_SEL_SPI0 = 0,
	NVIC_IRQ_SEL_SPI1,
	NVIC_IRQ_SEL_DAC0,
	NVIC_IRQ_SEL_UART0,
	NVIC_IRQ_SEL_UART1,
	NVIC_IRQ_SEL_UART2,
	NVIC_IRQ_SEL_IIC1 = 7,
	NVIC_IRQ_SEL_IIC0,
	NVIC_IRQ_SEL_SCT,
	NVIC_IRQ_SEL_MRT,
	NVIC_IRQ_SEL_CMP_CAPT,
	NVIC_IRQ_SEL_WDT,
	NVIC_IRQ_SEL_BOD,
	NVIC_IRQ_SEL_FLASH,
	NVIC_IRQ_SEL_WKT,
	NVIC_IRQ_SEL_ADC_SEQA,
	NVIC_IRQ_SEL_ADC_SEQB,
	NVIC_IRQ_SEL_ADC_THCMP,
	NVIC_IRQ_SEL_ADC_OVR,
	NVIC_IRQ_SEL_DMA,
	NVIC_IRQ_SEL_IIC2,
	NVIC_IRQ_SEL_IIC3,
	NVIC_IRQ_SEL_CTIMER,
	NVIC_IRQ_SEL_PININT0,
	NVIC_IRQ_SEL_PININT1,
	NVIC_IRQ_SEL_PININT2,
	NVIC_IRQ_SEL_PININT3,
	NVIC_IRQ_SEL_PININT4,
	NVIC_IRQ_SEL_PININT5_DAC1,
	NVIC_IRQ_SEL_PININT6_UART3,
	NVIC_IRQ_SEL_PININT7_UART4
}NVIC_irq_sel_en;

/**
 * @brief Habilitacion de interrupciones
 * @param[in] irq Seleccion de fuente de interrupcion
 */
static inline void NVIC_enable_interrupt(NVIC_irq_sel_en irq)
{
	*((uint32_t *) &NVIC->ISER0) |= (1 << irq);
}

/**
 * @brief Inhabilitacion de interrupciones
 * @param[in] irq Seleccion de fuente de interrupcion
 */
static inline void NVIC_disable_interrupt(NVIC_irq_sel_en irq)
{
	*((uint32_t *) &NVIC->ICER0) |= (1 << irq);
}

/**
 * @brief Fijar interupcion pendiente por software
 * @param[in] irq Seleccion de fuente de interrupcion
 */
static inline void NVIC_set_pending_interrupt(NVIC_irq_sel_en irq)
{
	*((uint32_t *) &NVIC->ISPR0) |= (1 << irq);
}

/**
 * @brief Limpiar interupcion pendiente por software
 * @param[in] irq Seleccion de fuente de interrupcion
 */
static inline void NVIC_clear_pending_interrupt(NVIC_irq_sel_en irq)
{
	*((uint32_t *) &NVIC->ICPR0) |= (1 << irq);
}

/**
 * @brief Obtener estado de interrupcion
 * @param[in] irq Seleccion de fuente de interrupcion
 * @return Si la interrupcion estaba activa devuelve 1, caso contrario devuelve 0
 */
static inline uint8_t NVIC_get_active_interrupt(NVIC_irq_sel_en irq)
{
	return (*((uint32_t *) &NVIC->IABR0) & (1 << irq)) >> irq;
}

#warning Faltan las funciones para fijar niveles de prioridad de irqs

#endif /* HPL_NVIC_H_ */
