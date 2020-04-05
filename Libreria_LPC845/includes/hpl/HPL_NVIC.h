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

extern volatile NVIC_per_t * const NVIC; //!< Periferico NVIC

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

typedef enum
{
	NVIC_IRQ_PRIORITY_HIGHEST = 0,
	NVIC_IRQ_PRIORITY_HIGH,
	NVIC_IRQ_PRIORITY_LOW,
	NVIC_IRQ_PRIORITY_LOWEST
}NVIC_irq_priority_en;

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

/*
 * @brief Fijar prioridad en el NVIC a un periferico
 */
static inline void NVIC_set_irq_priority(NVIC_irq_sel_en irq, NVIC_irq_priority_en priority)
{
	switch(irq)
	{
	case NVIC_IRQ_SEL_SPI0: { NVIC->IPR0.IP_SPI0 = priority; break; }
	case NVIC_IRQ_SEL_SPI1: { NVIC->IPR0.IP_SPI1 = priority; break; }
	case NVIC_IRQ_SEL_DAC0: { NVIC->IPR0.IP_DAC0 = priority; break; }
	case NVIC_IRQ_SEL_UART0: { NVIC->IPR0.IP_UART0 = priority; break; }
	case NVIC_IRQ_SEL_UART1: { NVIC->IPR1.IP_UART1 = priority; break; }
	case NVIC_IRQ_SEL_UART2: { NVIC->IPR1.IP_UART2 = priority; break; }
	case NVIC_IRQ_SEL_IIC1: { NVIC->IPR1.IP_I2C1 = priority; break; }
	case NVIC_IRQ_SEL_IIC0: { NVIC->IPR2.IP_I2C0 = priority; break; }
	case NVIC_IRQ_SEL_SCT: { NVIC->IPR2.IP_SCT = priority; break; }
	case NVIC_IRQ_SEL_MRT: { NVIC->IPR2.IP_MRT = priority; break; }
	case NVIC_IRQ_SEL_CMP_CAPT: { NVIC->IPR2.IP_CMP = priority; break; }
	case NVIC_IRQ_SEL_WDT: { NVIC->IPR3.IP_WDT = priority; break; }
	case NVIC_IRQ_SEL_BOD: { NVIC->IPR3.IP_BOD = priority; break; }
	case NVIC_IRQ_SEL_FLASH: { NVIC->IPR3.IP_FLASH = priority; break; }
	case NVIC_IRQ_SEL_WKT: { NVIC->IPR3.IP_WKT = priority; break; }
	case NVIC_IRQ_SEL_ADC_SEQA: { NVIC->IPR4.IP_ADC_SEQA = priority; break; }
	case NVIC_IRQ_SEL_ADC_SEQB: { NVIC->IPR4.IP_ADC_SEQB = priority; break; }
	case NVIC_IRQ_SEL_ADC_THCMP: { NVIC->IPR4.IP_ADC_THCMP = priority; break; }
	case NVIC_IRQ_SEL_ADC_OVR: { NVIC->IPR4.ID_ADC_OVR = priority; break; }
	case NVIC_IRQ_SEL_DMA: { NVIC->IPR5.IP_DMA = priority; break; }
	case NVIC_IRQ_SEL_IIC2: { NVIC->IPR5.IP_I2C2 = priority; break; }
	case NVIC_IRQ_SEL_IIC3: { NVIC->IPR5.IP_I2C3 = priority; break; }
	case NVIC_IRQ_SEL_CTIMER: { NVIC->IPR5.IP_CT32B0 = priority; break; }
	case NVIC_IRQ_SEL_PININT0: { NVIC->IPR6.IP_PININT0 = priority; break; }
	case NVIC_IRQ_SEL_PININT1: { NVIC->IPR6.IP_PININT1 = priority; break; }
	case NVIC_IRQ_SEL_PININT2: { NVIC->IPR6.IP_PININT2 = priority; break; }
	case NVIC_IRQ_SEL_PININT3: { NVIC->IPR6.IP_PININT3 = priority; break; }
	case NVIC_IRQ_SEL_PININT4: { NVIC->IPR7.IP_PININT4 = priority; break; }
	case NVIC_IRQ_SEL_PININT5_DAC1: { NVIC->IPR7.IP_PININT5 = priority; break; }
	case NVIC_IRQ_SEL_PININT6_UART3: { NVIC->IPR7.IP_PININT6 = priority; break; }
	case NVIC_IRQ_SEL_PININT7_UART4: { NVIC->IPR7.IP_PININT7 = priority; break; }

	}
}

#endif /* HPL_NVIC_H_ */
