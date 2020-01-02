/**
 * @file HRI_SYSTICK.h
 * @brief Definiciones a nivel de registros del modulo SYSTICK (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HRI_SYSTICK_H_
#define HRI_SYSTICK_H_

#include <stdint.h>

#define	SYSTICK_BASE	0xE000E000 //!< Base del periferico SYSTICK

typedef struct
{
	uint32_t RESERVED;
}SYSTICK_RESERVED_reg_t;

typedef struct
{
	uint32_t ENABLE : 1;
	uint32_t TICKINT : 1;
	uint32_t CLKSOURCE : 1;
	uint32_t RESERVED_1 : 13;
	uint32_t COUNTFLAG : 1;
	uint32_t RESERVED_2 : 15;
}SYSTICK_CSR_reg_t;

typedef struct
{
	uint32_t RELOAD : 23;
	uint32_t RESERVED : 9;
}SYSTICK_RVR_reg_t;

typedef struct
{
	uint32_t CURRENT : 23;
	uint32_t RESERVED : 9;
}SYSTICK_CVR_reg_t;

typedef struct
{
	uint32_t TENMS : 23;
	uint32_t RESERVED : 7;
	uint32_t SKEW : 1;
	uint32_t NOREF : 1;
}SYSTICK_CALIB_reg_t;

typedef struct
{
	volatile SYSTICK_RESERVED_reg_t RESERVED[4];
	volatile SYSTICK_CSR_reg_t CSR;
	volatile SYSTICK_RVR_reg_t RVR;
	volatile SYSTICK_CVR_reg_t CVR;
	volatile SYSTICK_CALIB_reg_t CALIB;
}SYSTICK_reg_t;

extern volatile SYSTICK_reg_t * const SYSTICK; //!< Periferico SYSTICK

#endif /* HRI_SYSTICK_H_ */
