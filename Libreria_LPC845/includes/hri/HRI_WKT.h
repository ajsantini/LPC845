/**
 * @file HRI_WKT.h
 * @brief Definiciones a nivel de registros del periferico WKT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HRI_WKT_H_
#define HRI_WKT_H_

#include <stdint.h>

#if defined (__cplusplus)
extern "C" {
#endif

#define			WKT_BASE		0x40008000

typedef struct
{
	uint32_t CLKSEL : 1;
	uint32_t ALARMFLAG : 1;
	uint32_t CLEARCTR : 1;
	uint32_t SEL_EXTCLK : 1;
	uint32_t : 28;
}WKT_CTRL_reg_t;

typedef struct
{
	uint32_t VALUE;
}WKT_COUNT_reg_t;

typedef struct
{
	WKT_CTRL_reg_t CTRL;
	const uint32_t RESERVED[2];
	WKT_COUNT_reg_t COUNT;
}WKT_per_t;

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HRI_WKT_H_ */
