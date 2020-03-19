/**
 * @file HRI_SPI.h
 * @brief Definiciones a nivel de registros del periferico SPI (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HRI_SPI_H_
#define HRI_SPI_H_

#include <stdint.h>

#define	SPI0_BASE		0x40058000
#define	SPI1_BASE		0x4005C000

typedef struct
{
	uint32_t ENABLE : 1;
	uint32_t : 1;
	uint32_t MASTER : 1;
	uint32_t LSBF : 1;
	uint32_t CPHA : 1;
	uint32_t CPOL : 1;
	uint32_t : 1;
	uint32_t LOOP : 1;
	uint32_t SPOL0 : 1;
	uint32_t SPOL1 : 1;
	uint32_t SPOL2 : 1;
	uint32_t SPOL3 : 1;
	uint32_t : 20;
}SPI_CFG_reg_t;

typedef struct
{
	uint32_t PRE_DELAY : 4;
	uint32_t POST_DELAY : 4;
	uint32_t TRANSFER_DELAY : 4;
	uint32_t : 20;
}SPI_DLY_reg_t;

typedef struct
{
	const uint32_t RXRDY : 1;
	const uint32_t TXRDY : 1;
	uint32_t RXOV : 1;
	uint32_t TXUR : 1;
	uint32_t SSA : 1;
	uint32_t SSD : 1;
	const uint32_t STALLED : 1;
	uint32_t ENDTRANSFER : 1;
	const uint32_t MSTIDLE : 1;
	uint32_t : 21;
}SPI_STAT_reg_t;

typedef struct
{
	uint32_t RXRDYEN : 1;
	uint32_t TXRDYEN : 1;
	uint32_t RXOVEN : 1;
	uint32_t TXUREN : 1;
	uint32_t SSAEN : 1;
	uint32_t SSDEN : 1;
	uint32_t : 26;
}SPI_INTENSET_reg_t;

typedef struct
{
	uint32_t RXRDYEN : 1;
	uint32_t TXRDYEN : 1;
	uint32_t RXOVEN : 1;
	uint32_t TXUREN : 1;
	uint32_t SSAEN : 1;
	uint32_t SSDEN : 1;
	uint32_t : 26;
}SPI_INTENCLR_reg_t;

typedef struct
{
	uint32_t RXDAT : 16;
	uint32_t RXSSEL0_N : 1;
	uint32_t RXSSEL1_N : 1;
	uint32_t RXSSEL2_N : 1;
	uint32_t RXSSEL3_N : 1;
	uint32_t SOT : 1;
	uint32_t : 11;
}SPI_RXDAT_reg_t;

typedef struct
{
	uint32_t TXDAT : 16;
	uint32_t TXSSEL0_N : 1;
	uint32_t TXSSEL1_N : 1;
	uint32_t TXSSEL2_N : 1;
	uint32_t TXSSEL3_N : 1;
	uint32_t EOT : 1;
	uint32_t EOf : 1; // El caracter en minuscula es a proposito
	uint32_t RXIGNORE : 1;
	uint32_t : 1;
	uint32_t LEN : 4;
	uint32_t : 4;
}SPI_TXDATCTL_reg_t;

typedef struct
{
	uint32_t DATA : 16;
	uint32_t : 16;
}SPI_TXDAT_reg_t;

typedef struct
{
	uint32_t : 16;
	uint32_t TXSSEL0_N : 1;
	uint32_t TXSSEL1_N : 1;
	uint32_t TXSSEL2_N : 1;
	uint32_t TXSSEL3_N : 1;
	uint32_t EOT : 1;
	uint32_t EOf : 1; // El caracter en minuscula es a proposito
	uint32_t RXIGNORE : 1;
	uint32_t : 1;
	uint32_t LEN : 4;
	uint32_t : 4;
}SPI_TXCTL_reg_t;

typedef struct
{
	uint32_t DIVVAL : 16;
	uint32_t : 16;
}SPI_DIV_reg_t;

typedef struct
{
	uint32_t RXRDY : 1;
	uint32_t TXRDY : 1;
	uint32_t RXOV : 1;
	uint32_t TXUR : 1;
	uint32_t SSA : 1;
	uint32_t SSD : 1;
	uint32_t : 26;
}SPI_INTSTAT_reg_t;

typedef struct
{
	SPI_CFG_reg_t CFG;
	SPI_DLY_reg_t DLY;
	SPI_STAT_reg_t STAT;
	SPI_INTENSET_reg_t INTENSET;
	SPI_INTENCLR_reg_t INTENCLR;
	const SPI_RXDAT_reg_t RXDAT;
	SPI_TXDATCTL_reg_t TXDATCTL;
	SPI_TXDAT_reg_t TXDAT;
	SPI_TXCTL_reg_t TXCTL;
	SPI_DIV_reg_t DIV;
	const SPI_INTSTAT_reg_t INTSTAT;
}SPI_per_t;

volatile SPI_per_t * const SPI[] = { //!< Perifericos SPI
		(SPI_per_t *) SPI0_BASE,
		(SPI_per_t *) SPI1_BASE
};

#endif /* HRI_SPI_H_ */
