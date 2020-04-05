/**
 * @file HRI_PMU.h
 * @brief Definiciones a nivel de registros del modulo PMU (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#ifndef HRI_PMU_H_
#define HRI_PMU_H_

#include <stdint.h>

#define		PMU_BASE		0x40020000

#define		SCR_REG_BASE	0xE000ED10

typedef struct
{
	uint32_t : 1;
	uint32_t SLEEPONEXIT : 1;
	uint32_t SLEEPDEEP : 1;
	uint32_t : 1;
	uint32_t SEVONPEND : 1;
	uint32_t : 27;
}SCR_reg_t;

typedef struct
{
	uint32_t PM : 2;
	uint32_t NODPD : 1;
	uint32_t : 4;
	uint32_t SLEEPFLAG : 1;
	uint32_t : 2;
	uint32_t DPDFLAG : 1;
	uint32_t : 21;
}PMU_PCON_reg_t;

typedef struct
{
	uint32_t GPDATA;
}PMU_GPREG_reg_t;

typedef struct
{
	uint32_t WAKEUPHYS : 1;
	uint32_t WAKEPAD_DISABLE : 1;
	uint32_t LPOSCEN : 1;
	uint32_t LPOSCDPDEN : 1;
	uint32_t WAKEUPCLKHYS : 1;
	uint32_t WAKECLKPAD_DISABLE : 1;
	uint32_t RESETHYS : 1;
	uint32_t RESET_DISABLE : 1;
	uint32_t : 24;
}PMU_DPDCTRL_reg_t;

typedef struct
{
	PMU_PCON_reg_t PCON;
	PMU_GPREG_reg_t GPREG[4];
	PMU_DPDCTRL_reg_t DPDCTRL;
}PMU_per_t;

#endif /* HRI_PMU_H_ */
