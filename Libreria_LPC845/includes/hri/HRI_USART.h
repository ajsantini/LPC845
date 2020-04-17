/**
 * @file HRI_USART.h
 * @brief Definiciones a nivel de registros del modulo UART (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HRI_USART_H_
#define HRI_USART_H_

#include <stdint.h>

#define	UART0_BASE		0x40064000
#define	UART1_BASE		0x40068000
#define	UART2_BASE		0x4006C000
#define	UART3_BASE		0x40070000
#define	UART4_BASE		0x40074000

typedef struct
{
	uint32_t ENABLE : 1;
	uint32_t : 1;
	uint32_t DATALEN : 2;
	uint32_t PARITYSEL : 2;
	uint32_t STOPLEN : 1;
	uint32_t : 2;
	uint32_t CTSEN : 1;
	uint32_t : 1;
	uint32_t SYNCEN : 1;
	uint32_t CLKPOL : 1;
	uint32_t : 1;
	uint32_t SYNCMST : 1;
	uint32_t LOOP :1;
	uint32_t : 2;
	uint32_t OETA : 1;
	uint32_t AUTOADDR : 1;
	uint32_t OESEL : 1;
	uint32_t OEPOL : 1;
	uint32_t RXPOL : 1;
	uint32_t TXPOL : 1;
	uint32_t : 8;
}USART_CFG_reg_t;

typedef struct
{
	uint32_t : 1;
	uint32_t TXBRKEN : 1;
	uint32_t ADDRDET : 1;
	uint32_t : 3;
	uint32_t TXDIS : 1;
	uint32_t : 1;
	uint32_t CC : 1;
	uint32_t CLRCONRX : 1;
	uint32_t : 6;
	uint32_t AUTOBAUD : 1;
	uint32_t : 15;
}USART_CTL_reg_t;

typedef struct
{
	uint32_t RXRDY : 1;
	uint32_t RXIDLE : 1;
	uint32_t TXRDY : 1;
	uint32_t TXIDLE : 1;
	uint32_t CTS : 1;
	uint32_t DELTACTS : 1;
	uint32_t TXDISSTAT : 1;
	uint32_t : 1;
	uint32_t OVERRUNINT : 1;
	uint32_t : 1;
	uint32_t RXBRK : 1;
	uint32_t DELTARXBRK : 1;
	uint32_t START : 1;
	uint32_t FRAMERRINT : 1;
	uint32_t PARITYERRINT : 1;
	uint32_t RXNOISEINT : 1;
	uint32_t ABERR : 1;
	uint32_t : 14;
}USART_STAT_reg_t;

typedef struct
{
	uint32_t RXRDYEN : 1;
	uint32_t : 1;
	uint32_t TXRDYEN : 1;
	uint32_t TXIDLEEN : 1;
	uint32_t : 1;
	uint32_t DELTACTSEN : 1;
	uint32_t TXDISEN : 1;
	uint32_t : 1;
	uint32_t OVERRUNEN : 1;
	uint32_t : 2;
	uint32_t DELTARXBRKEN : 1;
	uint32_t STARTEN : 1;
	uint32_t FRAMERREN : 1;
	uint32_t PARITYERREN : 1;
	uint32_t RXNOISEEN : 1;
	uint32_t ABERREN : 1;
	uint32_t : 14;
}USART_INTENSET_reg_t;

typedef struct
{
	uint32_t RXRDYCLR : 1;
	uint32_t : 1;
	uint32_t TXRDYCLR : 1;
	uint32_t TXIDLECLR : 1;
	uint32_t : 1;
	uint32_t DELTACTSCLR : 1;
	uint32_t TXDISCLR : 1;
	uint32_t : 1;
	uint32_t OVERRUNCLR : 1;
	uint32_t : 2;
	uint32_t DELTARXBRKCLR : 1;
	uint32_t STARTCLR : 1;
	uint32_t FRAMERRCLR : 1;
	uint32_t PARITYERRCLR : 1;
	uint32_t RXNOISECLR : 1;
	uint32_t ABERRCLR : 1;
	uint32_t : 14;
}USART_INTENCLR_reg_t;

typedef struct
{
	uint32_t RXDAT : 9;
	uint32_t : 23;
}USART_RXDAT_reg_t;

typedef struct
{
	uint32_t RXDAT : 9;
	uint32_t : 3;
	uint32_t FRAMERR : 1;
	uint32_t PARITYERR : 1;
	uint32_t RXNOISE : 1;
	uint32_t : 17;
}USART_RXDATSTAT_reg_t;


typedef struct
{
	uint32_t TXDAT : 9;
	uint32_t : 23;
}USART_TXDAT_reg_t;

typedef struct
{
	uint32_t BRGVAL : 16;
	uint32_t : 16;
}USART_BRG_reg_t;

typedef struct
{
	uint32_t RXRDY : 1;
	uint32_t : 1;
	uint32_t TXRDY : 1;
	uint32_t TXIDLE : 1;
	uint32_t : 1;
	uint32_t DELTACTS : 1;
	uint32_t TXDISINT : 1;
	uint32_t : 1;
	uint32_t OVERRUNINT : 1;
	uint32_t : 2;
	uint32_t DELTARXBRK : 1;
	uint32_t START : 1;
	uint32_t FRAMERRINT : 1;
	uint32_t PARITYERRINT : 1;
	uint32_t RXNOISEINT : 1;
	uint32_t ABERR : 1;
	uint32_t : 15;
}USART_INTSTAT_reg_t;

typedef struct
{
	uint32_t OSRVAL : 4;
	uint32_t : 28;
}USART_OSR_reg_t;

typedef struct
{
	uint32_t ADDRESS : 8;
	uint32_t : 24;
}USART_ADDR_reg_t;

typedef struct
{
	USART_CFG_reg_t CFG;
	USART_CTL_reg_t CTL;
	USART_STAT_reg_t STAT;
	USART_INTENSET_reg_t INTENSET;
	USART_INTENCLR_reg_t INTENCLR;
	const USART_RXDAT_reg_t RXDAT;
	const USART_RXDATSTAT_reg_t RXDATSTAT;
	USART_TXDAT_reg_t TXDAT;
	USART_BRG_reg_t BRG;
	const USART_INTSTAT_reg_t INTSTAT;
	USART_OSR_reg_t OSR;
	USART_ADDR_reg_t ADDR;
}USART_per_t;

#endif /* HRI_USART_H_ */
