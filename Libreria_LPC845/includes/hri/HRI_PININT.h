/**
 * @file HRI_PININT.h
 * @brief Definiciones a nivel de registros del modulo PININT (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HRI_PININT_H_
#define HRI_PININT_H_

#include <stdint.h>

#define	PININT_BASE	0xA0004000 //!< Direccion base del PININT

typedef struct
{
	uint32_t PMODE : 8;
	uint32_t : 24;
}PININT_ISEL_reg_t;

typedef struct
{
	uint32_t ENRL : 8;
	uint32_t : 24;
}PININT_IENR_reg_t;

typedef struct
{
	uint32_t SETENRL : 8;
	uint32_t : 24;
}PININT_SIENR_reg_t;

typedef struct
{
	uint32_t CENRL : 8;
	uint32_t : 24;
}PININT_CIENR_reg_t;

typedef struct
{
	uint32_t ENAF : 8;
	uint32_t : 24;
}PININT_IENF_reg_t;

typedef struct
{
	uint32_t SETENAF : 8;
	uint32_t : 24;
}PININT_SIENF_reg_t;

typedef struct
{
	uint32_t CENAF : 8;
	uint32_t : 24;
}PININT_CIENF_reg_t;

typedef struct
{
	uint32_t RDET : 8;
	uint32_t : 24;
}PININT_RISE_reg_t;

typedef struct
{
	uint32_t FDET : 8;
	uint32_t : 24;
}PININT_FALL_reg_t;

typedef struct
{
	uint32_t PSTAT : 8;
	uint32_t : 24;
}PININT_IST_reg_t;

typedef struct
{
	uint32_t SEL_PMATCH : 1;
	uint32_t ENA_RXEV : 1;
	uint32_t : 22;
	uint32_t PMAT : 8;
}PININT_PMCTRL_reg_t;

typedef struct
{
	uint32_t : 8;
	uint32_t SRC0 : 3;
	uint32_t SRC1 : 3;
	uint32_t SRC2 : 3;
	uint32_t SRC3 : 3;
	uint32_t SRC4 : 3;
	uint32_t SRC5 : 3;
	uint32_t SRC6 : 3;
	uint32_t SRC7 : 3;
}PININT_PMSRC_reg_t;

typedef struct
{
	uint32_t PROD_ENDPTS0 : 1;
	uint32_t PROD_ENDPTS1 : 1;
	uint32_t PROD_ENDPTS2 : 1;
	uint32_t PROD_ENDPTS3 : 1;
	uint32_t PROD_ENDPTS4 : 1;
	uint32_t PROD_ENDPTS5 : 1;
	uint32_t PROD_ENDPTS6 : 1;
	uint32_t : 1;
	uint32_t CFG0 : 3;
	uint32_t CFG1 : 3;
	uint32_t CFG2 : 3;
	uint32_t CFG3 : 3;
	uint32_t CFG4 : 3;
	uint32_t CFG5 : 3;
	uint32_t CFG6 : 3;
	uint32_t CFG7 : 3;
}PININT_PMCFG_reg_t;

typedef struct
{
	PININT_ISEL_reg_t ISEL;
	PININT_IENR_reg_t IENR;
	PININT_SIENR_reg_t SIENR;
	PININT_CIENR_reg_t CIENR;
	PININT_IENF_reg_t IENF;
	PININT_SIENF_reg_t SIENF;
	PININT_CIENF_reg_t CIENF;
	PININT_RISE_reg_t RISE;
	PININT_FALL_reg_t FALL;
	PININT_IST_reg_t IST;
	PININT_PMCTRL_reg_t PMCTRL;
	PININT_PMSRC_reg_t PMSRC;
	PININT_PMCFG_reg_t PMCFG;
}PININT_per_t;

#endif /* HRI_PININT_H_ */
