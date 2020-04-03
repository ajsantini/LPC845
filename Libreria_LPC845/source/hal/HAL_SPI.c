/**
 * @file HAL_SPI.c
 * @brief Funciones a nivel de aplicacion del periferico SPI (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <stddef.h>
#include <HAL_SPI.h>
#include <HPL_SPI.h>
#include <HPL_SWM.h>
#include <HPL_SYSCON.h>
#include <HPL_NVIC.h>

static void dummy_irq(void);

static void spi_irq_handler(uint8_t inst);

static void (*spi_rx_callback[])(void) = //!< Callbacks registrados a la recepcion de un dato por SPI
{
	dummy_irq,
	dummy_irq
};

static void (*spi_tx_callback[])(void) = //!< Callbacks registrados a la liberacion del buffer de transmision de SPI
{
	dummy_irq,
	dummy_irq
};

/**
 * @brief Inicializar SPI en modo master
 * @param[in] inst Instancia de SPI a inicializar
 * @param[in] config Configuracion deseada
 */
void hal_spi_master_mode_init(hal_spi_sel_en inst, const hal_spi_master_mode_config_t *config)
{
	uint8_t counter;

	SWM_init();

	if(config->miso_portpin != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_MISO(inst, HAL_GPIO_PORTPIN_TO_PORT(config->miso_portpin), HAL_GPIO_PORTPIN_TO_PIN(config->miso_portpin));
	}

	if(config->mosi_portpin != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_MOSI(inst, HAL_GPIO_PORTPIN_TO_PORT(config->mosi_portpin), HAL_GPIO_PORTPIN_TO_PIN(config->mosi_portpin));
	}

	if(config->sck_portpin != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_SCK(inst, HAL_GPIO_PORTPIN_TO_PORT(config->sck_portpin), HAL_GPIO_PORTPIN_TO_PIN(config->sck_portpin));
	}

	if(config->ssel_portpin[0] != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_SSEL0(inst, HAL_GPIO_PORTPIN_TO_PORT(config->ssel_portpin[0]), HAL_GPIO_PORTPIN_TO_PIN(config->ssel_portpin[0]));
	}

	if(config->ssel_portpin[1] != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_SSEL1(inst, HAL_GPIO_PORTPIN_TO_PORT(config->ssel_portpin[1]), HAL_GPIO_PORTPIN_TO_PIN(config->ssel_portpin[1]));
	}

	if(config->ssel_portpin[2] != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_SSEL2(inst, HAL_GPIO_PORTPIN_TO_PORT(config->ssel_portpin[2]), HAL_GPIO_PORTPIN_TO_PIN(config->ssel_portpin[2]));
	}

	if(config->ssel_portpin[3] != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_SSEL3(inst, HAL_GPIO_PORTPIN_TO_PORT(config->ssel_portpin[3]), HAL_GPIO_PORTPIN_TO_PIN(config->ssel_portpin[3]));
	}

	SWM_deinit();

	for(counter = 0; counter < 4; counter++)
	{
		if(config->ssel_polarity[counter] == HAL_SPI_SSEL_POLARITY_LOW)
		{
			SPI_set_ssel_active_low(inst, counter);
		}
		else
		{
			SPI_set_ssel_active_high(inst, counter);
		}
	}

	switch(inst)
	{
	case HAL_SPI_0:
	{
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_SPI0, config->clock_source);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_SPI0);

		SYSCON_clear_reset(SYSCON_RESET_SEL_SPI0);

		NVIC_enable_interrupt(NVIC_IRQ_SEL_SPI0);

		break;
	}

	case HAL_SPI_1:
	{
		SYSCON_set_peripheral_clock_source(SYSCON_PERIPHERAL_SEL_SPI1, config->clock_source);

		SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_SPI1);

		SYSCON_clear_reset(SYSCON_RESET_SEL_SPI1);

		NVIC_enable_interrupt(NVIC_IRQ_SEL_SPI1);

		break;
	}
	}

	SPI_enable(inst);

	SPI_set_master_mode(inst);

	SPI_select_slave(inst, 0xFF);

	SPI_set_pre_delay(inst, config->pre_delay);
	SPI_set_post_delay(inst, config->post_delay);
	SPI_set_frame_delay(inst, config->frame_delay);
	SPI_set_transfer_delay(inst, config->transfer_delay);

	if(config->rx_ready_callback != NULL)
	{
		SPI_enable_irq(inst, SPI_IRQ_RXRDY);
		spi_rx_callback[inst] = config->rx_ready_callback;
	}

	if(config->tx_free_callback != NULL)
	{
		// Las interrupciones se habilitaran en el comienzo de una transmision
		spi_tx_callback[inst] = config->tx_free_callback;
	}
}

/**
 * @brief Leer el dato recibido
 * @param[in] inst Instancia a consultar
 * @return Dato recibido
 */
uint16_t hal_spi_master_mode_rx_data(hal_spi_sel_en inst)
{
	return SPI_read_rx_data(inst);
}

/**
 * @brief Configurar la transmision
 * @param[in] inst Instancia a configurar
 * @param[in] config Configuracion para la transmision deseada
 */
void hal_spi_master_mode_config_tx(hal_spi_sel_en inst, const hal_spi_master_mode_tx_config_t *config)
{
	switch(config->clock_mode)
	{
	case HAL_SPI_CLOCK_MODE_0: { SPI_set_cpha_change(inst); SPI_set_cpol_low(inst); break; }
	case HAL_SPI_CLOCK_MODE_1: { SPI_set_cpha_capture(inst); SPI_set_cpol_low(inst); break; }
	case HAL_SPI_CLOCK_MODE_2: { SPI_set_cpha_change(inst); SPI_set_cpol_high(inst); break; }
	case HAL_SPI_CLOCK_MODE_3: { SPI_set_cpha_capture(inst); SPI_set_cpol_high(inst); break; }
	}

	SPI_set_clock_div(inst, config->clock_div);
}

/**
 * @brief Transmitir dato
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir, con controles asociados
 */
void hal_spi_master_mode_tx_data(hal_spi_sel_en inst, const hal_spi_master_mode_tx_data_t *data)
{
	SPI_set_data_and_control(inst, (SPI_TXDATCTL_reg_t *) data);
	SPI_enable_irq(inst, SPI_IRQ_TXRDY);
}

/**
 * @brief Actualizar callback en TXRDY
 * @param[in] inst Instancia a configurar
 * @param[in] new_callback Nuevo callback a ejecutar en TXRDY
 */
void hal_spi_master_mode_register_tx_callback(hal_spi_sel_en inst, void (*new_callback)(void))
{
	if(new_callback == NULL)
	{
		spi_tx_callback[inst] = dummy_irq;
	}
	else
	{
		spi_tx_callback[inst] = new_callback;
	}
}

/**
 * @brief Actualizar callback en RXRDY
 * @param[in] inst Instancia a configurar
 * @param[in] new_callback Nuevo callback a ejecutar en RXRDY
 */
void hal_spi_master_mode_register_rx_callback(hal_spi_sel_en inst, void (*new_callback)(void))
{
	if(new_callback == NULL)
	{
		SPI_disable_irq(inst, SPI_IRQ_RXRDY);
		spi_rx_callback[inst] = dummy_irq;
	}
	else
	{
		SPI_enable_irq(inst, SPI_IRQ_RXRDY);
		spi_rx_callback[inst] = new_callback;
	}
}

/*
 * @brief Funcion dummy para inicialiar punteros de interrupcion
 */
static void dummy_irq(void)
{
	return;
}

/**
 * @brief Manejador generico de interrupciones de SPI
 * @param[in] inst Instancia que genero la interrupcion
 */
static void spi_irq_handler(uint8_t inst)
{
	if(SPI_get_irq_flag_status(inst, SPI_IRQ_RXRDY) && SPI_get_status_flag(inst, SPI_STATUS_FLAG_RXRDY))
	{
		spi_rx_callback[inst]();

		// Lectura dummy para evitar loops infinitos si no importaba leer la informacion
		(void) SPI_read_rx_data(inst);
	}

	if(SPI_get_irq_flag_status(inst, SPI_IRQ_TXRDY) && SPI_get_status_flag(inst, SPI_STATUS_FLAG_TXRDY))
	{
		// Inhabilitacion de interrupcion de TX por si no se envia mas informacion
		SPI_disable_irq(inst, SPI_IRQ_TXRDY);

		// Si se envia nueva informacion, en este llamado se activa de nuevo la interrupcion
		spi_tx_callback[inst]();
	}
}

/**
 * @brief Manejador de interrupcion de SPI0
 */
void SPI0_IRQHandler(void)
{
	spi_irq_handler(0);
}

/**
 * @brief Manejador de interrupcion de SPI1
 */
void SPI1_IRQHandler(void)
{
	spi_irq_handler(1);
}
