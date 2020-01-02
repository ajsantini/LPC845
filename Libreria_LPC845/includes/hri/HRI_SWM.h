/**
 * @file HRI_SWM.h
 * @brief Definiciones a nivel de registros del modulo SWM (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HRI_SWM_H_
#define HRI_SWM_H_

#include <stdint.h>

#define	SWM_BASE	0x4000C000 //!< Base del eriferico SWM

typedef struct
{
	uint32_t U0_TXD_O : 8;
	uint32_t U0_RXD_I : 8;
	uint32_t U0_RTS_O : 8;
	uint32_t U0_CTS_I : 8;
}SWM_PINASSIGN0_reg_t;

typedef struct
{
	uint32_t U0_SCLK_IO : 8;
	uint32_t U1_TXD_O : 8;
	uint32_t U1_RXD_I : 8;
	uint32_t U1_RTS_O : 8;
}SWM_PINASSIGN1_reg_t;

typedef struct
{
	uint32_t U1_CTS_I : 8;
	uint32_t U1_SCLK_IO : 8;
	uint32_t U2_TXD_O : 8;
	uint32_t U2_RXD_I : 8;
}SWM_PINASSIGN2_reg_t;

typedef struct
{
	uint32_t U2_RTS_O : 8;
	uint32_t U2_CTS_I : 8;
	uint32_t U2_SCLK_IO : 8;
	uint32_t SPI0_SCK_IO : 8;
}SWM_PINASSIGN3_reg_t;

typedef struct
{
	uint32_t SPI0_MOSI_IO : 8;
	uint32_t SPI0_MISO_IO : 8;
	uint32_t SPI0_SSEL0_IO : 8;
	uint32_t SPI0_SSEL1_IO : 8;
}SWM_PINASSIGN4_reg_t;

typedef struct
{
	uint32_t SPI0_SSEL2_IO : 8;
	uint32_t SPI0_SSEL3_IO : 8;
	uint32_t SPI1_SCK_IO : 8;
	uint32_t SPI1_MOSI_IO : 8;
}SWM_PINASSIGN5_reg_t;

typedef struct
{
	uint32_t SPI1_MISO_IO : 8;
	uint32_t SPI1_SSEL0_IO : 8;
	uint32_t SPI1_SSEL1_IO : 8;
	uint32_t SCT0_GPIO_IN_A_I : 8;
}SWM_PINASSIGN6_reg_t;

typedef struct
{
	uint32_t SCT0_GPIO_IN_B_I : 8;
	uint32_t SCT0_GPIO_IN_C_I : 8;
	uint32_t SCT0_GPIO_IN_D_I : 8;
	uint32_t SCT_OUT0_O : 8;
}SWM_PINASSIGN7_reg_t;

typedef struct
{
	uint32_t SCT_OUT1_O : 8;
	uint32_t SCT_OUT2_O : 8;
	uint32_t SCT_OUT3_O : 8;
	uint32_t SCT_OUT4_O : 8;
}SWM_PINASSIGN8_reg_t;

typedef struct
{
	uint32_t SCT_OUT5_O : 8;
	uint32_t SCT_OUT6_O : 8;
	uint32_t I2C1_SDA_IO : 8;
	uint32_t I2C1_SCL_IO : 8;
}SWM_PINASSIGN9_reg_t;

typedef struct
{
	uint32_t I2C2_SDA_IO : 8;
	uint32_t I2C2_SCL_IO : 8;
	uint32_t I2C3_SDA_IO : 8;
	uint32_t I2C3_SCL_IO : 8;
}SWM_PINASSIGN10_reg_t;

typedef struct
{
	uint32_t COMP0_OUT_O : 8;
	uint32_t CLKOUT_O : 8;
	uint32_t GPIO_INT_BMAT_O : 8;
	uint32_t UART3_TXD : 8;
}SWM_PINASSIGN11_reg_t;

typedef struct
{
	uint32_t UART3_RXD : 8;
	uint32_t UART3_SCLK : 8;
	uint32_t UART4_TXD : 8;
	uint32_t UART4_RXD : 8;
}SWM_PINASSIGN12_reg_t;

typedef struct
{
	uint32_t UART4_SCLK : 8;
	uint32_t T0_MAT0 : 8;
	uint32_t T0_MAT1 : 8;
	uint32_t T0_MAT2 : 8;
}SWM_PINASSIGN13_reg_t;

typedef struct
{
	uint32_t T0_MAT3 : 8;
	uint32_t T0_CAP0 : 8;
	uint32_t T0_CAP1 : 8;
	uint32_t T0_CAP2 : 8;
}SWM_PINASSIGN14_reg_t;

typedef struct
{
	uint32_t ACMP_I1 : 1;
	uint32_t ACMP_I2 : 1;
	uint32_t ACMP_I3 : 1;
	uint32_t ACMP_I4 : 1;
	uint32_t ACMP_I5 : 1;
	uint32_t SWCLK : 1;
	uint32_t SWDIO : 1;
	uint32_t XTALIN : 1;
	uint32_t XTALOUT : 1;
	uint32_t RESETN : 1;
	uint32_t CLKIN : 1;
	uint32_t VDDCMP : 1;
	uint32_t I2C0_SDA : 1;
	uint32_t I2C0_SCL : 1;
	uint32_t ADC_0 : 1;
	uint32_t ADC_1 : 1;
	uint32_t ADC_2 : 1;
	uint32_t ADC_3 : 1;
	uint32_t ADC_4 : 1;
	uint32_t ADC_5 : 1;
	uint32_t ADC_6 : 1;
	uint32_t ADC_7 : 1;
	uint32_t ADC_8 : 1;
	uint32_t ADC_9 : 1;
	uint32_t ADC_10 : 1;
	uint32_t ADC_11 : 1;
	uint32_t DACOUT0 : 1;
	uint32_t DACOUT1 : 1;
	uint32_t CAPT_X0 : 1;
	uint32_t CAPT_X1 : 1;
	uint32_t CAPT_X2 : 1;
	uint32_t CAPT_X3 : 1;
}SWM_PINENABLE0_reg_t;

typedef struct
{
	uint32_t CAPT_X4 : 1;
	uint32_t CAPT_X5 : 1;
	uint32_t CAPT_X6 : 1;
	uint32_t CAPT_X7 : 1;
	uint32_t CAPT_X8 : 1;
	uint32_t CAPT_YL : 1;
	uint32_t CAPT_YH : 1;
	uint32_t RESERVED : 26;
}SWM_PINENABLE1_reg_t;

typedef struct
{
	SWM_PINASSIGN0_reg_t PINASSIGN0;
	SWM_PINASSIGN1_reg_t PINASSIGN1;
	SWM_PINASSIGN2_reg_t PINASSIGN2;
	SWM_PINASSIGN3_reg_t PINASSIGN3;
	SWM_PINASSIGN4_reg_t PINASSIGN4;
	SWM_PINASSIGN5_reg_t PINASSIGN5;
	SWM_PINASSIGN6_reg_t PINASSIGN6;
	SWM_PINASSIGN7_reg_t PINASSIGN7;
	SWM_PINASSIGN8_reg_t PINASSIGN8;
	SWM_PINASSIGN9_reg_t PINASSIGN9;
	SWM_PINASSIGN10_reg_t PINASSIGN10;
	SWM_PINASSIGN11_reg_t PINASSIGN11;
	SWM_PINASSIGN12_reg_t PINASSIGN12;
	SWM_PINASSIGN13_reg_t PINASSIGN13;
	SWM_PINASSIGN14_reg_t PINASSIGN14;
	uint32_t RESERVED[(0x188 - 4) / 4];
	SWM_PINENABLE0_reg_t PINENABLE0;
	SWM_PINENABLE1_reg_t PINENABLE1;
}SWM_per_t;

extern volatile SWM_per_t * const SWM; //!< Periferico SWM

#endif /* HRI_SWM_H_ */
