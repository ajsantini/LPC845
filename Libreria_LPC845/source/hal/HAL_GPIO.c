/**
 * @file HAL_GPIO.c
 * @brief Funciones a nivel de aplicacion del periferico GPIO (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_GPIO.h>
#include <HPL_SYSCON.h>
#include <HPL_GPIO.h>

/**
 * @brief Inicializar un puerto
 * @param[in] port Puerto a inicializar
 */
void hal_gpio_init(hal_gpio_port_en port)
{
	if(port == HAL_GPIO_PORT_0)
	{
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_GPIO0);
	}
	else
	{
		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_GPIO1);
	}
}

/**
 * @brief Fijar dirección de una GPIO
 * @param[in] portpin Número de puerto/pin a configurar
 * @param[in] dir Dirección deseada
 * @param[in] initial_state Estado inicial (aplica únicamente para salidas)
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_set_dir(hal_gpio_portpin_en portpin, hal_gpio_dir_en dir, uint8_t initial_state)
{
	if(dir == HAL_GPIO_DIR_INPUT)
	{
		GPIO_write_dir_clear(HAL_GPIO_PORTPIN_TO_PORT(portpin), 1 << HAL_GPIO_PORTPIN_TO_PIN(portpin));
	}
	else
	{
		GPIO_write_dir_set(HAL_GPIO_PORTPIN_TO_PORT(portpin), 1 << HAL_GPIO_PORTPIN_TO_PIN(portpin));

		if(initial_state)
		{
			GPIO_write_set(HAL_GPIO_PORTPIN_TO_PORT(portpin), 1 << HAL_GPIO_PORTPIN_TO_PIN(portpin));
		}
	}
}

/**
 * @brief Fijar estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_set_pin(hal_gpio_portpin_en portpin)
{
	GPIO_write_port_byte(portpin, 1);
}

/**
 * @brief Fijar estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_set Máscara de bits a fijar
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_set_port(hal_gpio_port_en port, uint32_t bits_to_set)
{
	GPIO_write_portpin(port, GPIO_read_portpin(port) | bits_to_set);
}

/**
 * @brief Fijar estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_set Máscara de bits a fijar
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_masked_set_port(hal_gpio_port_en port, uint32_t bits_to_set)
{
	GPIO_write_masked_portpin(port, GPIO_read_portpin(port) | bits_to_set);
}

/**
 * @brief Limpiar estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_clear_pin(hal_gpio_portpin_en portpin)
{
	GPIO_write_port_byte(portpin, 0);
}

/**
 * @brief Limpiar estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_clear Máscara de bits a limpiar
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_clear_port(hal_gpio_port_en port, uint32_t bits_to_clear)
{
	GPIO_write_portpin(port, GPIO_read_portpin(port) & (~bits_to_clear));
}

/**
 * @brief Limpiar estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_clear Máscara de bits a limpiar
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_masked_clear_port(hal_gpio_port_en port, uint32_t bits_to_clear)
{
	GPIO_write_masked_portpin(port, GPIO_read_portpin(port) & (~bits_to_clear));
}

/**
 * @brief Invertir estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_toggle_pin(hal_gpio_portpin_en portpin)
{
	GPIO_write_toggle(HAL_GPIO_PORTPIN_TO_PORT(portpin), 1 << HAL_GPIO_PORTPIN_TO_PIN(portpin));
}

/**
 * @brief Invertir estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_toggle Máscara de bits a invertir
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_toggle_port(hal_gpio_port_en port, uint32_t bits_to_toggle)
{
	GPIO_write_portpin(port, GPIO_read_portpin(port) ^ bits_to_toggle);
}

/**
 * @brief Invertir estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_toggle Máscara de bits a invertir
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_masked_toggle_port(hal_gpio_port_en port, uint32_t bits_to_toggle)
{
	GPIO_write_masked_portpin(port, GPIO_read_portpin(port) ^ bits_to_toggle);
}

/**
 * @brief Leer el estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 * @return Estado actual del puerto/pin omitiendo máscara
 *
 * @see hal_gpio_portpin_en
 * @pre Haber inicializado el puerto correspondiente
 */
uint8_t hal_gpio_read_pin(hal_gpio_portpin_en portpin)
{
	return GPIO_read_port_byte(portpin);
}

/**
 * @brief Leer estado de un puerto (sin importar máscara)
 * @param[in] port Puerto a consultar
 * @return Estado del puerto sin enmascarar
 *
 * @see hal_gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
uint32_t hal_gpio_read_port(hal_gpio_port_en port)
{
	return GPIO_read_portpin(port);
}

/**
 * @brief Leer estado de un puerto (teniendo en cuenta máscara)
 * @param[in] port Puerto a consultar
 * @return Estado del puerto contemplando la máscara asociada
 *
 * @see hal_gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
uint32_t hal_gpio_masked_read_port(hal_gpio_port_en port)
{
	return GPIO_read_masked_portpin(port);
}

/**
 * @brief Fijar enmascaramiento de pines en un puerto
 * @param[in] port Puerto a fijar enmascaramiento
 * @param[in] mask Máscara de bits a fijar
 *
 * @see hal_gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_set_mask_bits(hal_gpio_port_en port, uint32_t mask)
{
	GPIO_write_mask(port, GPIO_read_mask(port) | mask);
}

/**
 * @brief Limpiar enmascaramiento de pines en un puerto
 * @param[in] port Puerto a limpiar enmascaramiento
 * @param[in] mask Máscara de bits a limpiar
 *
 * @see hal_gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_clear_mask_bits(hal_gpio_port_en port, uint32_t mask)
{
	GPIO_write_mask(port, GPIO_read_mask(port) & (~mask));
}

/**
 * @brief Invertir enmascaramiento de pines en un puerto
 * @param[in] port Puerto a invertir enmascaramiento
 * @param[in] mask Máscara de bits a invertir
 *
 * @see hal_gpio_port_en
 * @pre Haber inicializado el puerto correspondiente
 */
void hal_gpio_toggle_mask_bits(hal_gpio_port_en port, uint32_t mask)
{
	GPIO_write_mask(port, GPIO_read_mask(port) ^ mask);
}
