/**
 * @file HRI_CTIMER.h
 * @brief Definiciones a nivel de registros del periferico CTIMER (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HRI_CTIMER_H_
#define HRI_CTIMER_H_

#include <stdint.h>

#if defined (__cplusplus)
extern "C" {
#endif

#define		CTIMER_BASE		0x40038000

typedef struct
{
	uint32_t MR0INT : 1;
	uint32_t MR1INT : 1;
	uint32_t MR2INT : 1;
	uint32_t MR3INT : 1;
	uint32_t CR0INT : 1;
	uint32_t CR1INT : 1;
	uint32_t CR2INT : 1;
	uint32_t CR3INT : 1;
	uint32_t : 24;
}CTIMER_IR_reg_t;

typedef struct
{
	uint32_t CEN : 1;
	uint32_t CRST : 1;
	uint32_t : 30;
}CTIMER_TCR_reg_t;

typedef struct
{
	uint32_t TCVAL;
}CTIMER_TC_reg_t;

typedef struct
{
	uint32_t PRVAL;
}CTIMER_PR_reg_t;

typedef struct
{
	uint32_t PCVAL;
}CTIMER_PC_reg_t;

typedef struct
{
	uint32_t MR0I : 1;
	uint32_t MR0R : 1;
	uint32_t MR0S : 1;
	uint32_t MR1I : 1;
	uint32_t MR1R : 1;
	uint32_t MR1S : 1;
	uint32_t MR2I : 1;
	uint32_t MR2R : 1;
	uint32_t MR2S : 1;
	uint32_t MR3I : 1;
	uint32_t MR3R : 1;
	uint32_t MR3S : 1;
	uint32_t : 12;
	uint32_t MR0RL : 1;
	uint32_t MR1RL : 1;
	uint32_t MR2RL : 1;
	uint32_t MR3RL : 1;
	uint32_t : 4;
}CTIMER_MCR_reg_t;

typedef struct
{
	uint32_t MATCH;
}CTIMER_MR_reg_t;

typedef struct
{
	uint32_t CAP0RE : 1;
	uint32_t CAP0FE : 1;
	uint32_t CAP0I : 1;
	uint32_t CAP1RE : 1;
	uint32_t CAP1FE : 1;
	uint32_t CAP1I : 1;
	uint32_t CAP2RE : 1;
	uint32_t CAP2FE : 1;
	uint32_t CAP2I : 1;
	uint32_t CAP3RE : 1;
	uint32_t CAP3FE : 1;
	uint32_t CAP3I : 1;
	uint32_t : 20;
}CTIMER_CCR_reg_t;

typedef struct
{
	uint32_t CAP;
}CTIMER_CR_reg_t;

typedef struct
{
	uint32_t EM0 : 1;
	uint32_t EM1 : 1;
	uint32_t EM2 : 1;
	uint32_t EM3 : 1;
	uint32_t EMC0 : 2;
	uint32_t EMC1 : 2;
	uint32_t EMC2 : 2;
	uint32_t EMC3 : 2;
	uint32_t : 20;
}CTIMER_EMR_reg_t;

typedef struct
{
	uint32_t CTMODE : 2;
	uint32_t CINSEL : 2;
	uint32_t ENCC : 1;
	uint32_t SELCC : 3;
	uint32_t : 24;
}CTIMER_CTCR_reg_t;

typedef struct
{
	uint32_t PWMEN0 : 1;
	uint32_t PWMEN1 : 1;
	uint32_t PWMEN2 : 1;
	uint32_t PWMEN3 : 1;
	uint32_t : 28;
}CTIMER_PWMC_reg_t;

typedef struct
{
	uint32_t SHADOW;
}CTIMER_MSR_reg_t;

typedef struct
{
	CTIMER_IR_reg_t IR;
	CTIMER_TCR_reg_t TCR;
	CTIMER_TC_reg_t TC;
	CTIMER_PR_reg_t PR;
	CTIMER_PC_reg_t PC;
	CTIMER_MCR_reg_t MCR;
	CTIMER_MR_reg_t MR[4];
	CTIMER_CCR_reg_t CCR;
	const CTIMER_CR_reg_t CR[4];
	CTIMER_EMR_reg_t EMR;
	const uint32_t RESERVED[12];
	CTIMER_CTCR_reg_t CTCR;
	CTIMER_PWMC_reg_t PWMC;
	CTIMER_MSR_reg_t MSR[4];
}CTIMER_per_t;

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HRI_CTIMER_H_ */
