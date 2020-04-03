/**
 * @file HPL_SPI.h
 * @brief Declaraciones a nivel de abstraccion de periferico del SPI (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HPL_SPI_H_
#define HPL_SPI_H_

#include <stdint.h>
#include <HRI_SPI.h>

extern volatile SPI_per_t * const SPI[]; //!< Perifericos SPI

typedef enum
{
	SPI_STATUS_FLAG_RXRDY = 0,
	SPI_STATUS_FLAG_TXRDY,
	SPI_STATUS_FLAG_RXOV,
	SPI_STATUS_FLAG_TXUR,
	SPI_STATUS_FLAG_SSA,
	SPI_STATUS_FLAG_SSD,
	SPI_STATUS_FLAG_STALLED,
	SPI_STATUS_FLAG_ENDTRANSFER,
	SPI_STATUS_FLAG_MSTIDLE,
}SPI_status_flag_en;

typedef enum
{
	SPI_IRQ_RXRDY = 0,
	SPI_IRQ_TXRDY,
	SPI_IRQ_RXOV,
	SPI_IRQ_TXUR,
	SPI_IRQ_SSA,
	SPI_IRQ_SSD
}SPI_irq_sel_en;

typedef enum
{
	SPI_DATA_LENGTH_1_BIT = 0,
	SPI_DATA_LENGTH_2_BIT,
	SPI_DATA_LENGTH_3_BIT,
	SPI_DATA_LENGTH_4_BIT,
	SPI_DATA_LENGTH_5_BIT,
	SPI_DATA_LENGTH_6_BIT,
	SPI_DATA_LENGTH_7_BIT,
	SPI_DATA_LENGTH_8_BIT,
	SPI_DATA_LENGTH_9_BIT,
	SPI_DATA_LENGTH_10_BIT,
	SPI_DATA_LENGTH_11_BIT,
	SPI_DATA_LENGTH_12_BIT,
	SPI_DATA_LENGTH_13_BIT,
	SPI_DATA_LENGTH_14_BIT,
	SPI_DATA_LENGTH_15_BIT,
	SPI_DATA_LENGTH_16_BIT
}SPI_data_length_en;

/**
 * @brief Habilitar modulo
 * @param[in] inst Instancia a habilitar
 */
static inline void SPI_enable(uint8_t inst)
{
	SPI[inst]->CFG.ENABLE = 1;
}

/**
 * @brief Inhabilitar modulo
 * @param[in] inst Instancia a inhabilitar
 */
static inline void SPI_disable(uint8_t inst)
{
	SPI[inst]->CFG.ENABLE = 0;
}

/**
 * @brief Configurar como modo master
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_master_mode(uint8_t inst)
{
	SPI[inst]->CFG.MASTER = 1;
}

/**
 * @brief Configurar como modo slave
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_slave_mode(uint8_t inst)
{
	SPI[inst]->CFG.MASTER = 0;
}

/**
 * @brief Configurar orden de datos MSB primero
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_data_order_msb_first(uint8_t inst)
{
	SPI[inst]->CFG.LSBF = 0;
}

/**
 * @brief Configurar orden de datos LSB primero
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_data_order_lsb_first(uint8_t inst)
{
	SPI[inst]->CFG.LSBF = 1;
}

/**
 * @brief Configurar fase del clock (modo change)
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_cpha_change(uint8_t inst)
{
	SPI[inst]->CFG.CPHA = 0;
}

/**
 * @brief Configurar fase del clock (modo capture)
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_cpha_capture(uint8_t inst)
{
	SPI[inst]->CFG.CPHA = 1;
}

/**
 * @brief Configurar polaridad del clock (polaridad baja)
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_cpol_low(uint8_t inst)
{
	SPI[inst]->CFG.CPOL = 0;
}

/**
 * @brief Configurar polaridad del clock (polaridad alta)
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_cpol_high(uint8_t inst)
{
	SPI[inst]->CFG.CPOL = 1;
}

/**
 * @brief Habilitar modo loopback
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_enable_loopback_mode(uint8_t inst)
{
	SPI[inst]->CFG.LOOP = 1;
}

/**
 * @brief Inhabilitar modo loopback
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_disable_loopback_mode(uint8_t inst)
{
	SPI[inst]->CFG.LOOP = 0;
}

/**
 * @brief Fijar polaridad de slave select como activa baja
 * @param[in] inst Instancia a configurar
 * @param[in] channel Canal de SSEL a configurar
 */
static inline void SPI_set_ssel_active_low(uint8_t inst, uint8_t channel)
{
	switch(channel)
	{
	case 0: { SPI[inst]->CFG.SPOL0 = 0; break; }
	case 1: { SPI[inst]->CFG.SPOL1 = 0; break; }
	case 2: { SPI[inst]->CFG.SPOL2 = 0; break; }
	case 3: { SPI[inst]->CFG.SPOL3 = 0; break; }
	}
}

/**
 * @brief Fijar polaridad de slave select como activa alta
 * @param[in] inst Instancia a configurar
 * @param[in] channel Canal de SSEL a configurar
 */
static inline void SPI_set_ssel_active_high(uint8_t inst, uint8_t channel)
{
	switch(channel)
	{
	case 0: { SPI[inst]->CFG.SPOL0 = 1; break; }
	case 1: { SPI[inst]->CFG.SPOL1 = 1; break; }
	case 2: { SPI[inst]->CFG.SPOL2 = 1; break; }
	case 3: { SPI[inst]->CFG.SPOL3 = 1; break; }
	}
}

/**
 * @brief Configurar ciclos de clock entre la activacion de SSEL y la transmision de datos
 * @param[in] inst Instancia a configurar
 * @param[in] delay Clocks de delay deseados
 */
static inline void SPI_set_pre_delay(uint8_t inst, uint8_t delay)
{
	SPI[inst]->DLY.PRE_DELAY = delay;
}

/**
 * @brief Configurar ciclos de clock entre la finalizacion de transmision y desactivacion de SSEL
 * @param[in] inst Instancia a configurar
 * @param[in] delay Clocks de delay deseados
 */
static inline void SPI_set_post_delay(uint8_t inst, uint8_t delay)
{
	SPI[inst]->DLY.POST_DELAY = delay;
}

/**
 * @brief Configurar ciclos de clock entre transmisiones sin desactivar SSEL
 * @param[in] inst Instancia a configurar
 * @param[in] delay Clocks de delay deseados
 */
static inline void SPI_set_frame_delay(uint8_t inst, uint8_t delay)
{
	SPI[inst]->DLY.FRAME_DELAY = delay;
}

/**
 * @brief Configurar ciclos de clock entre desactivacion/activacion de SSEL
 * @param[in] inst Instancia a configurar
 * @param[in] delay Clocks de delay deseados
 */
static inline void SPI_set_transfer_delay(uint8_t inst, uint8_t delay)
{
	SPI[inst]->DLY.TRANSFER_DELAY = delay;
}

/**
 * @brief Leer un flag de status
 * @param[in] inst Instancia a consultar
 * @param[in] flag Flag a consultar
 * @return Estado del flag actual
 */
static inline uint8_t SPI_get_status_flag(uint8_t inst, SPI_status_flag_en flag)
{
	switch(flag)
	{
	case SPI_STATUS_FLAG_RXRDY: { return SPI[inst]->STAT.RXRDY; break; }
	case SPI_STATUS_FLAG_TXRDY: { return SPI[inst]->STAT.TXRDY; break; }
	case SPI_STATUS_FLAG_RXOV: { return SPI[inst]->STAT.RXOV; break; }
	case SPI_STATUS_FLAG_TXUR: { return SPI[inst]->STAT.TXUR; break; }
	case SPI_STATUS_FLAG_SSA: { return SPI[inst]->STAT.SSA; break; }
	case SPI_STATUS_FLAG_SSD: { return SPI[inst]->STAT.SSD; break; }
	case SPI_STATUS_FLAG_STALLED: { return SPI[inst]->STAT.STALLED; break; }
	case SPI_STATUS_FLAG_ENDTRANSFER: { return SPI[inst]->STAT.ENDTRANSFER; break; }
	case SPI_STATUS_FLAG_MSTIDLE: { return SPI[inst]->STAT.MSTIDLE; break; }
	}

	return 0;
}

/**
 * @brief Limpiar un flag de status
 * @param[in] inst Instancia a limpiar
 * @param[in] flag Flag a limpiar
 */
static inline uint8_t SPI_clear_status_flag(uint8_t inst, SPI_status_flag_en flag)
{
	switch(flag)
	{
	case SPI_STATUS_FLAG_RXOV: { SPI[inst]->STAT.RXOV = 1; break; }
	case SPI_STATUS_FLAG_TXUR: { SPI[inst]->STAT.TXUR = 1; break; }
	case SPI_STATUS_FLAG_SSA: { SPI[inst]->STAT.SSA = 1; break; }
	case SPI_STATUS_FLAG_SSD: { SPI[inst]->STAT.SSD = 1; break; }
	case SPI_STATUS_FLAG_ENDTRANSFER: { SPI[inst]->STAT.ENDTRANSFER = 1; break; }
	case SPI_STATUS_FLAG_MSTIDLE: break;
	case SPI_STATUS_FLAG_RXRDY: break;
	case SPI_STATUS_FLAG_STALLED: break;
	case SPI_STATUS_FLAG_TXRDY: break;
	}

	return 0;
}

/**
 * @brief Habilitar interrupcion
 * @param[in] inst Instancia a configurar
 * @param[in] irq Interrupcion a habilitar
 */
static inline uint8_t SPI_enable_irq(uint8_t inst, SPI_irq_sel_en irq)
{
	switch(irq)
	{
	case SPI_IRQ_RXRDY: { SPI[inst]->INTENSET.RXRDYEN = 1; break; }
	case SPI_IRQ_TXRDY: { SPI[inst]->INTENSET.TXRDYEN = 1; break; }
	case SPI_IRQ_RXOV: { SPI[inst]->INTENSET.RXOVEN = 1; break; }
	case SPI_IRQ_TXUR: { SPI[inst]->INTENSET.TXUREN = 1; break; }
	case SPI_IRQ_SSA: { SPI[inst]->INTENSET.SSAEN = 1; break; }
	case SPI_IRQ_SSD: { SPI[inst]->INTENSET.SSDEN = 1; break; }
	}

	return 0;
}

/**
 * @brief Inhabilitar interrupcion
 * @param[in] inst Instancia a configurar
 * @param[in] irq Interrupcion a inhabilitar
 */
static inline uint8_t SPI_disable_irq(uint8_t inst, SPI_irq_sel_en irq)
{
	switch(irq)
	{
	case SPI_IRQ_RXRDY: { SPI[inst]->INTENCLR.RXRDYEN = 1; break; }
	case SPI_IRQ_TXRDY: { SPI[inst]->INTENCLR.TXRDYEN = 1; break; }
	case SPI_IRQ_RXOV: { SPI[inst]->INTENCLR.RXOVEN = 1; break; }
	case SPI_IRQ_TXUR: { SPI[inst]->INTENCLR.TXUREN = 1; break; }
	case SPI_IRQ_SSA: { SPI[inst]->INTENCLR.SSAEN = 1; break; }
	case SPI_IRQ_SSD: { SPI[inst]->INTENCLR.SSDEN = 1; break; }
	}

	return 0;
}

/**
 * @brief Leer resultado de la recepcion
 * @param[in] inst Instancia a consultar
 */
static inline uint16_t SPI_read_rx_data(uint8_t inst)
{
	return SPI[inst]->RXDAT.RXDAT;
}

/**
 * @brief Obtener slave select activo
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t SPI_get_active_ssl(uint8_t inst)
{
	if(SPI[inst]->RXDAT.RXSSEL0_N == 0)
		return 0;

	if(SPI[inst]->RXDAT.RXSSEL1_N == 0)
		return 1;

	if(SPI[inst]->RXDAT.RXSSEL2_N == 0)
		return 2;

	if(SPI[inst]->RXDAT.RXSSEL3_N == 0)
		return 3;
}

/**
 * @brief Obtener estado del flag de start of transfer
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t SPI_get_sot_flag(uint8_t inst)
{
	return SPI[inst]->RXDAT.SOT;
}

/**
 * @brief Escribir registro de datos de transmision
 * @param[in] inst Instancia a escribir
 * @param[in] data Dato a escribir
 */
static inline void SPI_write_txdata(uint8_t inst, uint16_t data)
{
	SPI[inst]->TXDAT.DATA = data;
}

/**
 * @brief Habilitar slave select para la proxima transmision
 * @param[in] inst Instancia a configurar
 * @param[in] channel Canal a utilizar en la proxima transmision
 */
static inline void SPI_select_slave(uint8_t inst, uint8_t channel)
{
	switch(channel)
	{
	case 0: { SPI[inst]->TXCTL.TXSSEL0_N = 0; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	case 1: { SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 0; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	case 2: { SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 0; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	case 3: { SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 0; break; }
	default:{ SPI[inst]->TXCTL.TXSSEL0_N = 1; SPI[inst]->TXCTL.TXSSEL1_N = 1; SPI[inst]->TXCTL.TXSSEL2_N = 1; SPI[inst]->TXCTL.TXSSEL3_N = 1; break; }
	}
}

/**
 * @brief Indicar fin de transmision en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_end_of_transmission(uint8_t inst)
{
	SPI[inst]->TXCTL.EOT = 1;
}

/**
 * @brief Limpiar fin de transmision en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_clear_end_of_transmission(uint8_t inst)
{
	SPI[inst]->TXCTL.EOT = 0;
}

/**
 * @brief Indicar fin de trama en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_end_of_frame(uint8_t inst)
{
	SPI[inst]->TXCTL.EOf = 1;
}

/**
 * @brief Limpiar fin de trama en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_clear_end_of_frame(uint8_t inst)
{
	SPI[inst]->TXCTL.EOf = 0;
}

/**
 * @brief Ignorar recepcion en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_set_rx_ignore(uint8_t inst)
{
	SPI[inst]->TXCTL.RXIGNORE = 1;
}

/**
 * @brief No ignorar recepcino en la proxima transmision
 * @param[in] inst Instancia a configurar
 */
static inline void SPI_clear_rx_ignore(uint8_t inst)
{
	SPI[inst]->TXCTL.RXIGNORE = 0;
}

/**
 * @brief Configurar largo de bits de palabra
 * @param[in] inst Instancia a configurar
 * @param[in] data_length Largo de palabra deseado
 */
static inline void SPI_set_data_length(uint8_t inst, SPI_data_length_en data_length)
{
	SPI[inst]->TXCTL.LEN = data_length;
}

/**
 * @brief Escribir data a transmitir y control al mismo tiempo (en una unica escritura)
 * @param[in] inst Instancia a utilizar
 * @param[in] data_and_control Dato a transmitir y control
 */
static inline void SPI_set_data_and_control(uint8_t inst, SPI_TXDATCTL_reg_t *data_and_control)
{
	SPI[inst]->TXDATCTL = *data_and_control;
}

/**
 * @brief Configurar divisor de clock
 * @param[in] inst Instancia a configurar
 * @param[in] div Divisor deseado (el valor efectivo es este valor +1)
 */
static inline void SPI_set_clock_div(uint8_t inst, uint16_t div)
{
	SPI[inst]->DIV.DIVVAL = div;
}

/**
 * @brief Leer flag de interrupcion actual
 * @param[in] inst Instancia a consultar
 * @param[in] irq Flag de interrupcion a consultar
 */
static inline uint8_t SPI_get_irq_flag_status(uint8_t inst, SPI_irq_sel_en irq)
{
	switch(irq)
	{
	case SPI_IRQ_RXRDY: { return SPI[inst]->INTSTAT.RXRDY; break; }
	case SPI_IRQ_TXRDY: { return SPI[inst]->INTSTAT.TXRDY; break; }
	case SPI_IRQ_RXOV: { return SPI[inst]->INTSTAT.RXOV; break; }
	case SPI_IRQ_TXUR: { return SPI[inst]->INTSTAT.TXUR; break; }
	case SPI_IRQ_SSA: { return SPI[inst]->INTSTAT.SSA; break; }
	case SPI_IRQ_SSD: { return SPI[inst]->INTSTAT.SSD; break; }
	}

	return 0;
}

#endif /* HPL_SPI_H_ */
