/**
 * @file HPL_GPIO.h
 * @brief Declaraciones a nivel de abstraccion de periferico del GPIO (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_GPIO_H_
#define HPL_GPIO_H_

#include <stdint.h>
#include <HRI_GPIO.h>

extern volatile GPIO_per_t * const GPIO; //!< Periferico GPIO

typedef enum
{
	GPIO_DIR_INPUT = 0,
	GPIO_DIR_OUTPUT
}GPIO_dir_en;

typedef enum
{
	GPIO_PORT_0 = 0,
	GPIO_PORT_1
}GPIO_port_en;

typedef enum
{
	GPIO_PORTPIN_0_0 = 0,
	GPIO_PORTPIN_0_1,
	GPIO_PORTPIN_0_2,
	GPIO_PORTPIN_0_3,
	GPIO_PORTPIN_0_4,
	GPIO_PORTPIN_0_5,
	GPIO_PORTPIN_0_6,
	GPIO_PORTPIN_0_7,
	GPIO_PORTPIN_0_8,
	GPIO_PORTPIN_0_9,
	GPIO_PORTPIN_0_10,
	GPIO_PORTPIN_0_11,
	GPIO_PORTPIN_0_12,
	GPIO_PORTPIN_0_13,
	GPIO_PORTPIN_0_14,
	GPIO_PORTPIN_0_15,
	GPIO_PORTPIN_0_16,
	GPIO_PORTPIN_0_17,
	GPIO_PORTPIN_0_18,
	GPIO_PORTPIN_0_19,
	GPIO_PORTPIN_0_20,
	GPIO_PORTPIN_0_21,
	GPIO_PORTPIN_0_22,
	GPIO_PORTPIN_0_23,
	GPIO_PORTPIN_0_24,
	GPIO_PORTPIN_0_25,
	GPIO_PORTPIN_0_26,
	GPIO_PORTPIN_0_27,
	GPIO_PORTPIN_0_28,
	GPIO_PORTPIN_0_29,
	GPIO_PORTPIN_0_30,
	GPIO_PORTPIN_0_31,
	GPIO_PORTPIN_1_0,
	GPIO_PORTPIN_1_1,
	GPIO_PORTPIN_1_2,
	GPIO_PORTPIN_1_3,
	GPIO_PORTPIN_1_4,
	GPIO_PORTPIN_1_5,
	GPIO_PORTPIN_1_6,
	GPIO_PORTPIN_1_7,
	GPIO_PORTPIN_1_8,
	GPIO_PORTPIN_1_9,
	GPIO_PORTPIN_1_10,
	GPIO_PORTPIN_1_11,
	GPIO_PORTPIN_1_12,
	GPIO_PORTPIN_1_13,
	GPIO_PORTPIN_1_14,
	GPIO_PORTPIN_1_15,
	GPIO_PORTPIN_1_16,
	GPIO_PORTPIN_1_17,
	GPIO_PORTPIN_1_18,
	GPIO_PORTPIN_1_19,
	GPIO_PORTPIN_1_20,
	GPIO_PORTPIN_1_21
}GPIO_portpin_en;

/**
 * @brief Leer estado del pin absoluto (sin importar mascaras ni funcion alternativa)
 * @param[in] portpin Numero de port/pin a consultar
 * @return Estado del pin absoluto
 */
static inline uint8_t GPIO_read_port_byte(GPIO_portpin_en portpin)
{
	return GPIO->B[portpin].PBYTE;
}

/**
 * @brief Escribir estado del pin absoluto (sin importar mascaras ni funcion alternativa)
 * @param[in] portpin Numero de port/pin a escribir
 * @param[in] value Valor a escribir
 */
static inline void GPIO_write_port_byte(GPIO_portpin_en portpin, uint8_t value)
{
	GPIO->B[portpin].PBYTE = value;
}

/**
 * @brief Leer estado del pin absoluto (sin importar mascaras ni funcion alternativa)
 * @param[in] portpin Numero de port/pin a consultar
 * @return Estado del pin absoluto
 */
static inline uint8_t GPIO_read_port_word(GPIO_portpin_en portpin)
{
	return GPIO->W[portpin].PWORD;
}

/**
 * @brief Escribir estado del pin absoluto (sin importar mascaras ni funcion alternativa)
 * @param[in] portpin Numero de port/pin a escribir
 * @param[in] value Valor a escribir
 */
static inline void GPIO_write_port_word(GPIO_portpin_en portpin, uint8_t value)
{
	GPIO->W[portpin].PWORD = value;
}

/**
 * @brief Leer registro de direccion
 * @param[in] port Numero de puerto a consultar
 * @return Valor del registro
 */
static inline uint32_t GPIO_read_dir(GPIO_port_en port)
{
	return GPIO->DIR[port].DIRP;
}

/**
 * @brief Escribir registro de direccion
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
static inline void GPIO_write_dir(GPIO_port_en port, uint32_t value)
{
	GPIO->DIR[port].DIRP = value;
}

/**
 * @brief Leer registro de mascara
 * @param[in] port Numero de puerto a consultar
 * @return Valor del registro
 */
static inline uint32_t GPIO_read_mask(GPIO_port_en port)
{
	return GPIO->MASK[port].MASKP;
}

/**
 * @brief Escribir registro de mascara
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
static inline void GPIO_write_mask(GPIO_port_en port, uint32_t value)
{
	GPIO->MASK[port].MASKP = value;
}

/**
 * @brief Leer registro de puerto/pin
 * @param[in] port Numero de puerto a consultar
 * @return Valor del registro
 */
static inline uint32_t GPIO_read_portpin(GPIO_port_en port)
{
	return GPIO->PIN[port].PORT;
}

/**
 * @brief Escribir registro de puerto/pin
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
static inline void GPIO_write_portpin(GPIO_port_en port, uint32_t value)
{
	GPIO->PIN[port].PORT = value;
}

/**
 * @brief Leer registro de puerto/pin enmascarado
 * @param[in] port Numero de puerto a consultar
 * @return Valor del registro
 */
static inline uint32_t GPIO_read_masked_portpin(GPIO_port_en port)
{
	return GPIO->MPIN[port].MPORTP;
}

/**
 * @brief Escribir registro de puerto/pin enmascarado
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
static inline void GPIO_write_masked_portpin(GPIO_port_en port, uint32_t value)
{
	GPIO->MPIN[port].MPORTP = value;
}

/**
 * @brief Escribir registro de set
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
static inline void GPIO_write_set(GPIO_port_en port, uint32_t value)
{
	GPIO->SET[port].SETP = value;
}

/**
 * @brief Escribir registro de clear
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
static inline void GPIO_write_clear(GPIO_port_en port, uint32_t value)
{
	GPIO->CLR[port].CLRP = value;
}

/**
 * @brief Escribir registro de toggle
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
static inline void GPIO_write_toggle(GPIO_port_en port, uint32_t value)
{
	GPIO->NOT[port].NOTP = value;
}

/**
 * @brief Escribir registro de direction set
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
static inline void GPIO_write_dir_set(GPIO_port_en port, uint32_t value)
{
	GPIO->DIRSET[port].DIRSETP = value;
}

/**
 * @brief Escribir registro de direction clear
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
static inline void GPIO_write_dir_clear(GPIO_port_en port, uint32_t value)
{
	GPIO->DIRCLR[port].DIRCLRP = value;
}

/**
 * @brief Escribir registro de direction toggle
 * @param[in] port Numero de puerto a configurar
 * @param[in] value Valor deseado
 */
static inline void GPIO_write_dir_toggle(GPIO_port_en port, uint32_t value)
{
	GPIO->DIRNOT[port].DIRNOTP = value;
}

#endif /* HPL_GPIO_H_ */
