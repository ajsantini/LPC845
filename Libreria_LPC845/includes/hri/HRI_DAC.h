/**
 * @file HRI_DAC.h
 * @brief Declaraciones a nivel de registros del DAC (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HRI_DAC_H_
#define HRI_DAC_H_

#include <stdint.h>

#if defined (__cplusplus)
extern "C" {
#endif

#define	DAC0_BASE	0x40014000
#define	DAC1_BASE	0x40018000

typedef struct
{
	uint32_t : 6;
	uint32_t VALUE : 10;
	uint32_t BIAS : 1;
	uint32_t : 16;
}DAC_CR_reg_t;

typedef struct
{
	uint32_t INT_DMA_REQ : 1;
	uint32_t DBLBUF_ENA : 1;
	uint32_t CNT_ENA : 1;
	uint32_t DMA_ENA : 1;
	uint32_t : 28;
}DAC_CTRL_reg_t;

typedef struct
{
	uint32_t VALUE : 16;
	uint32_t : 16;
}DAC_CNTVAL_reg_t;

typedef struct
{
	DAC_CR_reg_t CR;
	DAC_CTRL_reg_t CTRL;
	DAC_CNTVAL_reg_t CNTVAL;
}DAC_per_t;

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HRI_DAC_H_ */
