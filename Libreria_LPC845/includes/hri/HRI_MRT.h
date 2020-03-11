/**
 * @file HRI_MRT.h
 * @brief Definiciones a nivel de registros del periferico MRT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HRI_MRT_H_
#define HRI_MRT_H_

#include <stdint.h>

#define		MRT_BASE		0x40004000

typedef struct
{
	uint32_t LVALUE : 31;
	uint32_t LOAD : 1;
}MRT_INTVAL_reg_t;

typedef struct
{
	uint32_t VALUE : 31;
	uint32_t : 1;
}MRT_TIMER_reg_t;

typedef struct
{
	uint32_t INTEN : 1;
	uint32_t MODE : 2;
	uint32_t : 29;
}MRT_CTRL_reg_t;

typedef struct
{
	uint32_t INTFLAG : 1;
	uint32_t RUN : 1;
	uint32_t : 30;
}MRT_STAT_reg_t;

typedef struct
{
	uint32_t : 4;
	uint32_t CHAN : 4;
	uint32_t : 24;
}MRT_IDLE_CH_reg_t;

typedef struct
{
	uint32_t CFLAG0 : 1;
	uint32_t CFLAG1 : 1;
	uint32_t CFLAG2 : 1;
	uint32_t CFLAG3 : 1;
	uint32_t : 28;
}MRT_IRQ_FLAG_reg_t;

typedef struct
{
	MRT_INTVAL_reg_t INTVAL;
	const MRT_TIMER_reg_t TIMER;
	MRT_CTRL_reg_t CTRL;
	MRT_STAT_reg_t STAT;
}MRT_CHN_reg_t;

typedef struct
{
	MRT_CHN_reg_t CHN[4];
	const uint32_t RESERVED;
	const MRT_IDLE_CH_reg_t IDLE_CH;
	MRT_IRQ_FLAG_reg_t IRQ_FLAG;
}MRT_per_t;

#endif /* HRI_MRT_H_ */
