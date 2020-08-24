/**
 * @file HPL_USART.h
 * @brief Declaraciones a nivel de abstraccion de periferico del USART (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_USART_H_
#define HPL_USART_H_

#include "HRI_USART.h"

#if defined (__cplusplus)
extern "C" {
#endif

extern volatile USART_per_t * const USART[]; //!< Perifericos USART

typedef enum
{
	USART_DATALEN_7BIT = 0,
	USART_DATALEN_8BIT,
	USART_DATALEN_9BIT
}USART_datalen_en;

typedef enum
{
	USART_PARITY_NO_PARITY = 0,
	USART_PARITY_EVEN = 2,
	USART_PARITY_ODD
}USART_parity_en;

typedef enum
{
	USART_STOPLEN_1BIT = 0,
	USART_STOPLEN_2BIT
}USART_stoplen_en;

typedef enum
{
	USART_SYNC_MODE_ASYNCHRONOUS = 0,
	USART_SYNC_MODE_SYNCHRONOUS
}USART_sync_mode_en;

typedef enum
{
	USART_POLARITY_FALLING_EDGE = 0,
	USART_POLARITY_RISING_EDGE
}USART_polarity_en;

typedef enum
{
	USART_MASTER_MODE_SLAVE = 0,
	USART_MASTER_MODE_MASTER
}USART_master_mode_en;

typedef enum
{
	USART_OUTPUT_ENABLE_POL_LOW = 0,
	USART_OUTPUT_ENABLE_POL_HIGH
}USART_output_enable_pol_en;

/**
 * @brief Habilitacion de una instancia de USART
 * @param[in] inst Instancia a habilitar
 */
static inline void USART_enable(uint8_t inst)
{
	USART[inst]->CFG.ENABLE = 1;
}

/**
 * @brief Inhabilitacion de una instancia de USART
 * @param[in] inst Instancia a inhabilitar
 */
static inline void USART_disable(uint8_t inst)
{
	USART[inst]->CFG.ENABLE = 0;
}

/**
 * @brief Configurar largo de palabra
 * @param[in] inst Instancia a configurar
 * @param[in] datalen Seleccion de largo de palabra deseado
 */
static inline void USART_config_data_length(uint8_t inst, USART_datalen_en datalen)
{
	USART[inst]->CFG.DATALEN = datalen;
}

/**
 * @brief Configurar paridad
 * @param[in] inst Instancia a configurar
 * @param[in] parity Seleccion de paridad deseada
 */
static inline void USART_config_parity(uint8_t inst, USART_parity_en parity)
{
	USART[inst]->CFG.PARITYSEL = parity;
}

/**
 * @brief Configurar bits de stop
 * @param[in] inst Instancia a configurar
 * @param[in] stop_bits Seleccion de bits de stop deseados
 */
static inline void USART_config_stop_bits(uint8_t inst, USART_stoplen_en stop_bits)
{
	USART[inst]->CFG.STOPLEN = stop_bits;
}

/**
 * @brief Habilitar CTS
 * @param[in] inst Instancia a habilitar
 */
static inline void USART_enable_CTS(uint8_t inst)
{
	USART[inst]->CFG.CTSEN = 1;
}

/**
 * @brief Inhabilitar CTS
 * @param[in] inst Instancia a inhabilitar
 */
static inline void USART_disable_CTS(uint8_t inst)
{
	USART[inst]->CFG.CTSEN = 0;
}

/**
 * @brief Configurar modo asincronico/sincronico
 * @param[in] inst Instancia a configurar
 * @param[in] sync_mode Modo desesado
 */
static inline void USART_config_sync_mode(uint8_t inst, USART_sync_mode_en sync_mode)
{
	USART[inst]->CFG.SYNCEN = sync_mode;
}

/**
 * @brief Configurar polaridad de clock y sampleo (modo sincronico)
 * @param[in] inst Instancia a configurar
 * @param[in] polarity Polaridad y sampleo desesado
 */
static inline void USART_config_clock_polarity(uint8_t inst, USART_polarity_en polarity)
{
	USART[inst]->CFG.CLKPOL = polarity;
}

/**
 * @brief Configurar modo master o slave (modo sincronico)
 * @param[in] inst Instancia a configurar
 * @param[in] master_mode Modo deseado
 */
static inline void USART_config_master_mode(uint8_t inst, USART_master_mode_en master_mode)
{
	USART[inst]->CFG.SYNCMST = master_mode;
}

/**
 * @brief Habilitar modo loopback
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_loopback(uint8_t inst)
{
	USART[inst]->CFG.LOOP = 1;
}

/**
 * @brief Inhabilitar modo loopback
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_loopback(uint8_t inst)
{
	USART[inst]->CFG.LOOP = 0;
}

/**
 * @brief Habilitar turnarround para RS-485
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_OETA(uint8_t inst)
{
	USART[inst]->CFG.OETA = 1;
}

/**
 * @brief Inhabilitar turnarround para RS-485
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_OETA(uint8_t inst)
{
	USART[inst]->CFG.OETA = 0;
}

/**
 * @brief Habilitar auto address
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_auto_address(uint8_t inst)
{
	USART[inst]->CFG.AUTOADDR = 1;
}

/**
 * @brief Inhabilitar auto address
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_auto_address(uint8_t inst)
{
	USART[inst]->CFG.AUTOADDR = 0;
}

/**
 * @brief Habilitar output enable para RS-485
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_OESEL(uint8_t inst)
{
	USART[inst]->CFG.OESEL = 1;
}

/**
 * @brief Inhabilitar output enable para RS-485
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_OESEL(uint8_t inst)
{
	USART[inst]->CFG.OESEL = 0;
}

/**
 * @brief Configurar polaridad de output enable para RS-485
 * @param[in] inst Instancia a configurar
 */
static inline void USART_config_OEPOL(uint8_t inst, USART_output_enable_pol_en polarity)
{
	USART[inst]->CFG.OEPOL = polarity;
}

/**
 * @brief Habilitar inversion para recepcion
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_rx_invert(uint8_t inst)
{
	USART[inst]->CFG.RXPOL = 1;
}

/**
 * @brief Inhabilitar inversion para recepcion
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_rx_invert(uint8_t inst)
{
	USART[inst]->CFG.RXPOL = 0;
}

/**
 * @brief Habilitar inversion para transmision
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_tx_invert(uint8_t inst)
{
	USART[inst]->CFG.TXPOL = 1;
}

/**
 * @brief Inhabilitar inversion para transmision
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_tx_invert(uint8_t inst)
{
	USART[inst]->CFG.TXPOL = 0;
}

/**
 * @brief Fijar condicion de break
 * @param[in] inst Instancia a configurar
 */
static inline void USART_assert_break(uint8_t inst)
{
	USART[inst]->CTL.TXBRKEN = 1;
}

/**
 * @brief Liberar condicion de break
 * @param[in] inst Instancia a configurar
 */
static inline void USART_clear_break(uint8_t inst)
{
	USART[inst]->CTL.TXBRKEN = 0;
}

/**
 * @brief Habilitar address detect
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_address_detect(uint8_t inst)
{
	USART[inst]->CTL.ADDRDET = 1;
}

/**
 * @brief Inhabilitar address detect
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_address_detect(uint8_t inst)
{
	USART[inst]->CTL.ADDRDET = 0;
}

/**
 * @brief Habilitar TX
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_tx(uint8_t inst)
{
	USART[inst]->CTL.TXDIS = 0;
}

/**
 * @brief Inhabilitar TX
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_tx(uint8_t inst)
{
	USART[inst]->CTL.TXDIS = 1;
}

/**
 * @brief Habilitar clock continuo (modo sincronico)
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_continuous_clock(uint8_t inst)
{
	USART[inst]->CTL.CC = 1;
}

/**
 * @brief Inhabilitar clock continuo (modo sincronico)
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_continuous_clock(uint8_t inst)
{
	USART[inst]->CTL.CC = 0;
}

/**
 * @brief Habilitar parada de clock continuo en rx (modo sincronico)
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_autoclear_continuous_clock(uint8_t inst)
{
	USART[inst]->CTL.CLRCONRX = 1;
}

/**
 * @brief Inhabilitar parada de clock continuo en rx (modo sincronico)
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_autoclear_continuous_clock(uint8_t inst)
{
	USART[inst]->CTL.CLRCONRX = 0;
}

/**
 * @brief Habilitar auto baud
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_autobaud(uint8_t inst)
{
	USART[inst]->CTL.AUTOBAUD = 1;
}

/**
 * @brief Inhabilitar auto baud
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_autobaud(uint8_t inst)
{
	USART[inst]->CTL.AUTOBAUD = 0;
}

/**
 * @brief Obtener estado del flag RXRDY
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_RXRDY(uint8_t inst)
{
	return USART[inst]->STAT.RXRDY;
}

/**
 * @brief Obtener estado del flag RXIDLE
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_RXIDLE(uint8_t inst)
{
	return USART[inst]->STAT.RXIDLE;
}

/**
 * @brief Obtener estado del flag TXRDY
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_TXRDY(uint8_t inst)
{
	return USART[inst]->STAT.TXRDY;
}

/**
 * @brief Obtener estado del flag TXIDLE
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_TXIDLE(uint8_t inst)
{
	return USART[inst]->STAT.TXIDLE;
}

/**
 * @brief Obtener estado del flag CTS
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_CTS(uint8_t inst)
{
	return USART[inst]->STAT.CTS;
}

/**
 * @brief Obtener estado del flag DELTACTS
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_DELTACTS(uint8_t inst)
{
	return USART[inst]->STAT.DELTACTS;
}

/**
 * @brief Obtener estado del flag TXDISSTAT
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_TXDISSTAT(uint8_t inst)
{
	return USART[inst]->STAT.TXDISSTAT;
}

/**
 * @brief Obtener estado del flag OVERRUNINT
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_OVERRUNINT(uint8_t inst)
{
	return USART[inst]->STAT.OVERRUNINT;
}

/**
 * @brief Obtener estado del flag RXBRK
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_RXBRK(uint8_t inst)
{
	return USART[inst]->STAT.RXBRK;
}

/**
 * @brief Obtener estado del flag DELTARXBRK
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_DELTARXBRK(uint8_t inst)
{
	return USART[inst]->STAT.DELTARXBRK;
}

/**
 * @brief Obtener estado del flag START
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_START(uint8_t inst)
{
	return USART[inst]->STAT.START;
}

/**
 * @brief Obtener estado del flag FRAMERRINT
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_FRAMERRINT(uint8_t inst)
{
	return USART[inst]->STAT.FRAMERRINT;
}

/**
 * @brief Obtener estado del flag PARITYERRINT
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_PARITYERRINT(uint8_t inst)
{
	return USART[inst]->STAT.PARITYERRINT;
}

/**
 * @brief Obtener estado del flag RXNOISEINT
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_RXNOISEINT(uint8_t inst)
{
	return USART[inst]->STAT.RXNOISEINT;
}

/**
 * @brief Obtener estado del flag ABERR
 * @param[in] inst Instancia a consultar
 * @return Estado del flag
 */
static inline uint8_t USART_get_flag_ABERR(uint8_t inst)
{
	return USART[inst]->STAT.ABERR;
}

/**
 * @brief Habilitar interrupcion en RXRDY
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_RXRDY(uint8_t inst)
{
	USART[inst]->INTENSET.RXRDYEN = 1;
}

/**
 * @brief Habilitar interrupcion en TXRDY
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_TXRDY(uint8_t inst)
{
	USART[inst]->INTENSET.TXRDYEN = 1;
}

/**
 * @brief Habilitar interrupcion en TXIDLE
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_TXIDLE(uint8_t inst)
{
	USART[inst]->INTENSET.TXIDLEEN = 1;
}

/**
 * @brief Habilitar interrupcion en DELTACTS
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_DELTACTS(uint8_t inst)
{
	USART[inst]->INTENSET.DELTACTSEN = 1;
}

/**
 * @brief Habilitar interrupcion en TXDISEN
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_TXDISEN(uint8_t inst)
{
	USART[inst]->INTENSET.TXDISEN = 1;
}

/**
 * @brief Habilitar interrupcion en OVERRUN
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_OVERRUN(uint8_t inst)
{
	USART[inst]->INTENSET.OVERRUNEN = 1;
}

/**
 * @brief Habilitar interrupcion en DELTARXBRK
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_DELTARXBRK(uint8_t inst)
{
	USART[inst]->INTENSET.DELTARXBRKEN = 1;
}

/**
 * @brief Habilitar interrupcion en START
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_START(uint8_t inst)
{
	USART[inst]->INTENSET.STARTEN = 1;
}

/**
 * @brief Habilitar interrupcion en FRAMERR
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_FRAMERR(uint8_t inst)
{
	USART[inst]->INTENSET.FRAMERREN = 1;
}

/**
 * @brief Habilitar interrupcion en PARITYERR
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_PARITYERR(uint8_t inst)
{
	USART[inst]->INTENSET.PARITYERREN = 1;
}

/**
 * @brief Habilitar interrupcion en RXNOISE
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_RXNOISE(uint8_t inst)
{
	USART[inst]->INTENSET.RXNOISEEN = 1;
}

/**
 * @brief Habilitar interrupcion en ABERR
 * @param[in] inst Instancia a configurar
 */
static inline void USART_enable_irq_ABERR(uint8_t inst)
{
	USART[inst]->INTENSET.ABERREN = 1;
}

/**
 * @brief Inhabilitar interrupcion en RXRDY
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_RXRDY(uint8_t inst)
{
	USART[inst]->INTENCLR.RXRDYCLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en TXRDY
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_TXRDY(uint8_t inst)
{
	USART[inst]->INTENCLR.TXRDYCLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en TXIDLE
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_TXIDLE(uint8_t inst)
{
	USART[inst]->INTENCLR.TXIDLECLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en DELTACTS
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_DELTACTS(uint8_t inst)
{
	USART[inst]->INTENCLR.DELTACTSCLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en TXDISEN
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_TXDISEN(uint8_t inst)
{
	USART[inst]->INTENCLR.TXDISCLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en OVERRUN
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_OVERRUN(uint8_t inst)
{
	USART[inst]->INTENCLR.OVERRUNCLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en DELTARXBRK
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_DELTARXBRK(uint8_t inst)
{
	USART[inst]->INTENCLR.DELTARXBRKCLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en START
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_START(uint8_t inst)
{
	USART[inst]->INTENCLR.STARTCLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en FRAMERR
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_FRAMERR(uint8_t inst)
{
	USART[inst]->INTENCLR.FRAMERRCLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en PARITYERR
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_PARITYERR(uint8_t inst)
{
	USART[inst]->INTENCLR.PARITYERRCLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en RXNOISE
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_RXNOISE(uint8_t inst)
{
	USART[inst]->INTENCLR.RXNOISECLR = 1;
}

/**
 * @brief Inhabilitar interrupcion en ABERR
 * @param[in] inst Instancia a configurar
 */
static inline void USART_disable_irq_ABERR(uint8_t inst)
{
	USART[inst]->INTENCLR.ABERRCLR = 1;
}

/**
 * @brief Obtener ultimo dato recibido
 * @param[in] inst Instancia a consultar
 * @return Ultimo dato recibido
 */
static inline uint32_t USART_get_data(uint8_t inst)
{
	return USART[inst]->RXDAT.RXDAT;
}

/**
 * @brief Obtener ultimo dato recibido con flags de errores
 * @param[in] inst Instancia a consultar
 * @param[out] frame Flag correspondiente a frame error
 * @param[out] parity Flag correspondiente a parity error
 * @param[out] noise Flag correspondiente a noise error
 * @return Ultimo dato recibido
 */
static inline uint32_t USART_get_data_and_status(uint8_t inst, uint8_t * frame, uint8_t * parity, uint8_t * noise)
{
	USART_RXDATSTAT_reg_t rxdatstat = USART[inst]->RXDATSTAT;

	*frame = rxdatstat.FRAMERR;
	*parity = rxdatstat.PARITYERR;
	*noise = rxdatstat.RXNOISE;

	return rxdatstat.RXDAT;
}

/**
 * @brief Iniciar transmision de dato
 * @param[in] inst Instancia a utilizar
 * @param[in] data Dato a transmitir
 */
static inline void USART_write_data(uint8_t inst, uint32_t data)
{
	USART[inst]->TXDAT.TXDAT = data;
}

/**
 * @brief Escribir el registro BRG
 * @param[in] inst Instancia a configurar
 * @param[in] brg Valor a escribir en el registro
 */
static inline void USART_set_BRGVAL(uint8_t inst, uint32_t brg)
{
	USART[inst]->BRG.BRGVAL = brg;
}

/**
 * @brief Leer estado de interrupcion RXRDY
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_RXRDY(uint8_t inst)
{
	return USART[inst]->INTSTAT.RXRDY;
}

/**
 * @brief Leer estado de interrupcion TXRDY
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_TXRDY(uint8_t inst)
{
	return USART[inst]->INTSTAT.TXRDY;
}

/**
 * @brief Leer estado de interrupcion TXIDLE
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_TXIDLE(uint8_t inst)
{
	return USART[inst]->INTSTAT.TXIDLE;
}

/**
 * @brief Leer estado de interrupcion DELTACTS
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_DELTACTS(uint8_t inst)
{
	return USART[inst]->INTSTAT.DELTACTS;
}

/**
 * @brief Leer estado de interrupcion TXDIS
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_TXDIS(uint8_t inst)
{
	return USART[inst]->INTSTAT.TXDISINT;
}

/**
 * @brief Leer estado de interrupcion OVERRUN
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_OVERRUN(uint8_t inst)
{
	return USART[inst]->INTSTAT.OVERRUNINT;
}

/**
 * @brief Leer estado de interrupcion DELTARXBRK
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_DELTARXBRK(uint8_t inst)
{
	return USART[inst]->INTSTAT.DELTARXBRK;
}

/**
 * @brief Leer estado de interrupcion START
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_START(uint8_t inst)
{
	return USART[inst]->INTSTAT.START;
}

/**
 * @brief Leer estado de interrupcion FRAMERR
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_FRAMERR(uint8_t inst)
{
	return USART[inst]->INTSTAT.FRAMERRINT;
}

/**
 * @brief Leer estado de interrupcion PARITYERR
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_PARITYERR(uint8_t inst)
{
	return USART[inst]->INTSTAT.PARITYERRINT;
}

/**
 * @brief Leer estado de interrupcion RXNOISE
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_RXNOISE(uint8_t inst)
{
	return USART[inst]->INTSTAT.RXNOISEINT;
}

/**
 * @brief Leer estado de interrupcion ABERR
 * @param[in] inst Instancia a consultar
 */
static inline uint8_t USART_get_irq_status_ABERR(uint8_t inst)
{
	return USART[inst]->INTSTAT.ABERR;
}

/**
 * @brief Escribir el registro OSR
 * @param[in] inst Instancia a configurar
 * @param[in] osr Valor a escribir en el registro
 */
static inline void USART_set_OSRVAL(uint8_t inst, uint32_t osr)
{
	USART[inst]->OSR.OSRVAL = osr;
}

/**
 * @brief Escribir el registro ADDR
 * @param[in] inst Instancia a configurar
 * @param[in] brg Valor a escribir en el registro
 */
static inline void USART_set_address(uint8_t inst, uint32_t addr)
{
	USART[inst]->ADDR.ADDRESS = addr;
}

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HPL_USART_H_ */
