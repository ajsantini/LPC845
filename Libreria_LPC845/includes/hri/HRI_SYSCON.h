/**
 * @file HRI_SYSCON.h
 * @brief Definiciones a nivel de registros del modulo SYSCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HRI_SYSCON_H_
#define HRI_SYSCON_H_

#include <stdint.h>

#define	SYSCON_BASE						0x40048000

#define	SYSCON_PER_CLK_SOURCE_FRO		0
#define	SYSCON_PER_CLK_SOURCE_MAIN		1
#define	SYSCON_PER_CLK_SOURCE_FRG0		2
#define	SYSCON_PER_CLK_SOURCE_FRG1		3
#define	SYSCON_PER_CLK_SOURCE_FRO_DIV	4
#define	SYSCON_PER_CLK_SOURCE_NONE		7

typedef struct
{
	uint32_t RESERVED;
}SYSCON_RESERVED_reg_t;

typedef struct
{
	uint32_t MAP : 2;
	uint32_t : 30;
}SYSCON_SYSMEMREMAP_reg_t;

typedef struct
{
	uint32_t MSEL : 4;
	uint32_t PSEL : 2;
	uint32_t : 26;
}SYSCON_SYSPLLCTRL_reg_t;

typedef struct
{
	uint32_t LOCK : 1;
	uint32_t : 31;
}SYSCON_SYSPLLSTAT_reg_t;

typedef struct
{
	uint32_t BYPASS : 1;
	uint32_t FREQRANGE : 1;
	uint32_t : 30;
}SYSCON_SYSOSCCTRL_reg_t;

typedef struct
{
	uint32_t DIVSEL : 5;
	uint32_t FREQSEL : 4;
	uint32_t : 23;
}SYSCON_WDTOSCCTRL_reg_t;

typedef struct
{
	uint32_t : 17;
	uint32_t FRO_DIRECT : 1;
	uint32_t : 14;
}SYSCON_FROOSCCTRL_reg_t;

typedef struct
{
	uint32_t ENA : 1;
	uint32_t : 31;
}SYSCON_FRODIRECTCLKUEN_reg_t;

typedef struct
{
	uint32_t POR : 1;
	uint32_t EXTRST : 1;
	uint32_t WDT : 1;
	uint32_t BOD : 1;
	uint32_t SYSRST : 1;
	uint32_t : 27;
}SYSCON_SYSRSTSTAT_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_SYSPLLCLKSEL_reg_t;

typedef struct
{
	uint32_t ENA : 1;
	uint32_t : 31;
}SYSCON_SYSPLLCLKUEN_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_MAINCLKPLLSEL_reg_t;

typedef struct
{
	uint32_t ENA : 1;
	uint32_t : 31;
}SYSCON_MAINCLKPLLUEN_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_MAINCLKSEL_reg_t;

typedef struct
{
	uint32_t ENA : 1;
	uint32_t : 31;
}SYSCON_MAINCLKUEN_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_SYSAHBCLKDIV_reg_t;

typedef struct
{
	uint32_t SEL : 3;
	uint32_t : 29;
}SYSCON_CAPTCLKSEL_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_ADCCLKSEL_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_ADCCLKDIV_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_SCTCLKSEL_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_SCTCLKDIV_reg_t;

typedef struct
{
	uint32_t SEL : 1;
	uint32_t : 31;
}SYSCON_EXTCLKSEL_reg_t;

typedef struct
{
	uint32_t SYS : 1;
	uint32_t ROM : 1;
	uint32_t RAM0_1 : 1;
	uint32_t : 1;
	uint32_t FLASH : 1;
	uint32_t I2C0 : 1;
	uint32_t GPIO0 : 1;
	uint32_t SWM : 1;
	uint32_t SCT : 1;
	uint32_t WKT : 1;
	uint32_t MRT : 1;
	uint32_t SPI0 : 1;
	uint32_t SPI1 : 1;
	uint32_t CRC : 1;
	uint32_t UART0 : 1;
	uint32_t UART1 : 1;
	uint32_t UART2 : 1;
	uint32_t WWDT : 1;
	uint32_t IOCON : 1;
	uint32_t ACMP : 1;
	uint32_t GPIO1 : 1;
	uint32_t I2C1 : 1;
	uint32_t I2C2 : 1;
	uint32_t I2C3 : 1;
	uint32_t ADC : 1;
	uint32_t CTIMER0 : 1;
	uint32_t MTB : 1;
	uint32_t DAC0 : 1;
	uint32_t GPIO_INT : 1;
	uint32_t DMA : 1;
	uint32_t UART3 : 1;
	uint32_t UART4 : 1;
}SYSCON_SYSAHBCLKCTRL0_reg_t;

typedef struct
{
	uint32_t CAPT : 1;
	uint32_t DAC1 : 1;
	uint32_t : 30;
}SYSCON_SYSAHBCLKCTRL1_reg_t;

typedef struct
{
	uint32_t : 4;
	uint32_t FLASH_RST_N : 1;
	uint32_t I2C0_RST_N : 1;
	uint32_t GPIO0_RST_N : 1;
	uint32_t SWM_RST_N : 1;
	uint32_t SCT_RST_N : 1;
	uint32_t WKT_RST_N : 1;
	uint32_t MRT_RST_N : 1;
	uint32_t SPI0_RST_N : 1;
	uint32_t SPI1_RST_N : 1;
	uint32_t CRC_RST_N : 1;
	uint32_t UART0_RST_N : 1;
	uint32_t UART1_RST_N : 1;
	uint32_t UART2_RST_N : 1;
	uint32_t : 1;
	uint32_t IOCON_RST_N : 1;
	uint32_t ACMP_RST_N : 1;
	uint32_t GPIO1_RST_N : 1;
	uint32_t I2C1_RST_N : 1;
	uint32_t I2C2_RST_N : 1;
	uint32_t I2C3_RST_N : 1;
	uint32_t ADC_RST_N : 1;
	uint32_t CTIMER0_RST_N : 1;
	uint32_t : 1;
	uint32_t DAC0_RST_N : 1;
	uint32_t GPIOINT_RST_N : 1;
	uint32_t DMA_RST_N : 1;
	uint32_t UART3_RST_N : 1;
	uint32_t UART4_RST_N : 1;
}SYSCON_PRESETCTRL0_reg_t;

typedef struct
{
	uint32_t CAPT_RST_N : 1;
	uint32_t DAC1_RST_N : 1;
	uint32_t : 1;
	uint32_t FRG0_RST_N : 1;
	uint32_t FRG1_RST_N : 1;
	uint32_t : 27;
}SYSCON_PRESETCTRL1_reg_t;

typedef struct
{
	uint32_t SEL : 3;
	uint32_t : 29;
}SYSCON_PERCLKSEL_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_FRGDIV_reg_t;

typedef struct
{
	uint32_t MULT : 8;
	uint32_t : 24;
}SYSCON_FRGMULT_reg_t;

typedef struct
{
	uint32_t SEL : 2;
	uint32_t : 30;
}SYSCON_FRGCLKSEL_reg_t;

typedef struct
{
	uint32_t SEL : 3;
	uint32_t : 29;
}SYSCON_CLKOUTSEL_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_CLKOUTDIV_reg_t;

typedef struct
{
	uint32_t START : 1;
	uint32_t STOP : 1;
	uint32_t : 30;
}SYSCON_EXTTRACECMD_reg_t;

typedef struct
{
	uint32_t PIOSTAT;
}SYSCON_PIOPORCAP_reg_t;

typedef struct
{
	uint32_t DIV : 8;
	uint32_t : 24;
}SYSCON_IOCONCLKDIV_reg_t;

typedef struct
{
	uint32_t BODRSTLEV : 2;
	uint32_t BODINTVAL : 2;
	uint32_t BODRSTENA : 1;
	uint32_t : 27;
}SYSCON_BODCTRL_reg_t;

typedef struct
{
	uint32_t CAL : 26;
	uint32_t : 6;
}SYSCON_SYSTCKCAL_reg_t;

typedef struct
{
	uint32_t LATENCY : 8;
	uint32_t : 24;
}SYSCON_IRQLATENCY_reg_t;

typedef struct
{
	uint32_t IRQN : 5;
	uint32_t : 26;
	uint32_t NMIEN : 1;
}SYSCON_NMISRC_reg_t;

typedef struct
{
	uint32_t INTPIN : 6;
	uint32_t : 26;
}SYSCON_PINTSEL_reg_t;

typedef struct
{
	uint32_t PINT0 : 1;
	uint32_t PINT1 : 1;
	uint32_t PINT2 : 1;
	uint32_t PINT3 : 1;
	uint32_t PINT4 : 1;
	uint32_t PINT5 : 1;
	uint32_t PINT6 : 1;
	uint32_t PINT7 : 1;
	uint32_t : 24;
}SYSCON_STARTERP0_reg_t;

typedef struct
{
	uint32_t SPI0 : 1;
	uint32_t SPI1 : 1;
	uint32_t : 1;
	uint32_t USART0 : 1;
	uint32_t USART1 : 1;
	uint32_t USART2 : 1;
	uint32_t : 1;
	uint32_t I2C1 : 1;
	uint32_t I2C0 : 1;
	uint32_t : 2;
	uint32_t CAP_TOUCH : 1;
	uint32_t WWDT : 1;
	uint32_t BOD : 1;
	uint32_t : 1;
	uint32_t WKT : 1;
	uint32_t : 5;
	uint32_t I2C2 : 1;
	uint32_t I2C3 : 1;
	uint32_t : 7;
	uint32_t USART3 : 1;
	uint32_t USART4 : 1;
}SYSCON_STARTERP1_reg_t;

typedef struct
{
	uint32_t : 3;
	uint32_t BOD_PD : 1;
	uint32_t : 2;
	uint32_t WDTOSC_PD : 1;
	uint32_t : 25;
}SYSCON_PDSLEEPCFG_reg_t;

typedef struct
{
	uint32_t FRO_OUT_PD : 1;
	uint32_t FRO_PD : 1;
	uint32_t FLASH_PD : 1;
	uint32_t BOD_PD : 1;
	uint32_t ADC_PD : 1;
	uint32_t SYSOSC_PD : 1;
	uint32_t WDTOSC_PD : 1;
	uint32_t SYSPLL_PD : 1;
	uint32_t : 2;
	uint32_t VREF2_PD : 1;
	uint32_t : 2;
	uint32_t DAC0 : 1;
	uint32_t DAC1 : 1;
	uint32_t ACMP : 1;
	uint32_t : 16;
}SYSCON_PDAWAKECFG_reg_t;

typedef struct
{
	uint32_t FROOUT_PD : 1;
	uint32_t FRO_PD : 1;
	uint32_t FLASH_PD : 1;
	uint32_t BOD_PD : 1;
	uint32_t ADC_PD : 1;
	uint32_t SYSOSC_PD : 1;
	uint32_t WDTOSC_PD : 1;
	uint32_t SYSPLL_PD : 1;
	uint32_t : 5;
	uint32_t DAC0 : 1;
	uint32_t DAC1 : 1;
	uint32_t ACMP : 1;
	uint32_t : 16;
}SYSCON_PDRUNCFG_reg_t;

typedef struct
{
	uint32_t DEVICE_ID;
}SYSCON_DEVICE_ID_reg_t;

typedef struct
{
	SYSCON_SYSMEMREMAP_reg_t SYSMEMREMAP;
	const SYSCON_RESERVED_reg_t RESERVED_1;
	SYSCON_SYSPLLCTRL_reg_t SYSPLLCTRL;
	const SYSCON_SYSPLLSTAT_reg_t SYSPLLSTAT;
	const SYSCON_RESERVED_reg_t RESERVED_2;
	const SYSCON_RESERVED_reg_t RESERVED_3;
	const SYSCON_RESERVED_reg_t RESERVED_4;
	const SYSCON_RESERVED_reg_t RESERVED_5;
	SYSCON_SYSOSCCTRL_reg_t SYSOSCCTRL;
	SYSCON_WDTOSCCTRL_reg_t WDTOSCCTRL;
	SYSCON_FROOSCCTRL_reg_t FROOSCCTRL;
	const SYSCON_RESERVED_reg_t RESERVED_6;
	SYSCON_FRODIRECTCLKUEN_reg_t FRODIRECTCLKUEN;
	const SYSCON_RESERVED_reg_t RESERVED_7;
	SYSCON_SYSRSTSTAT_reg_t SYSRSTSTAT;
	const SYSCON_RESERVED_reg_t RESERVED_8;
	SYSCON_SYSPLLCLKSEL_reg_t SYSPLLCLKSEL;
	SYSCON_SYSPLLCLKUEN_reg_t SYSPLLCLKUEN;
	SYSCON_MAINCLKPLLSEL_reg_t MAINCLKPLLSEL;
	SYSCON_MAINCLKPLLUEN_reg_t MAINCLKPLLUEN;
	SYSCON_MAINCLKSEL_reg_t MAINCLKSEL;
	SYSCON_MAINCLKUEN_reg_t MAINCLKUEN;
	SYSCON_SYSAHBCLKDIV_reg_t SYSAHBCLKDIV;
	const SYSCON_RESERVED_reg_t RESERVED_9;
	SYSCON_CAPTCLKSEL_reg_t CAPTCLKSEL;
	SYSCON_ADCCLKSEL_reg_t ADCCLKSEL;
	SYSCON_ADCCLKDIV_reg_t ADCCLKDIV;
	SYSCON_SCTCLKSEL_reg_t SCTCLKSEL;
	SYSCON_SCTCLKDIV_reg_t SCTCLKDIV;
	SYSCON_EXTCLKSEL_reg_t EXTCLKSEL;
	const SYSCON_RESERVED_reg_t RESERVED_10;
	const SYSCON_RESERVED_reg_t RESERVED_11;
	SYSCON_SYSAHBCLKCTRL0_reg_t SYSAHBCLKCTRL0;
	SYSCON_SYSAHBCLKCTRL1_reg_t SYSAHBCLKCTRL1;
	SYSCON_PRESETCTRL0_reg_t PRESETCTRL0;
	SYSCON_PRESETCTRL1_reg_t PRESETCTRL1;
	SYSCON_PERCLKSEL_reg_t PERCLKSEL[11];
	const SYSCON_RESERVED_reg_t RESERVED_12;
	const SYSCON_RESERVED_reg_t RESERVED_13;
	const SYSCON_RESERVED_reg_t RESERVED_14;
	const SYSCON_RESERVED_reg_t RESERVED_15;
	const SYSCON_RESERVED_reg_t RESERVED_16;
	SYSCON_FRGDIV_reg_t FRG0DIV;
	SYSCON_FRGMULT_reg_t FRG0MUL;
	SYSCON_FRGCLKSEL_reg_t FRG0CLKSEL;
	const SYSCON_RESERVED_reg_t RESERVED_17;
	SYSCON_FRGDIV_reg_t FRG1DIV;
	SYSCON_FRGMULT_reg_t FRG1MUL;
	SYSCON_FRGCLKSEL_reg_t FRG1CLKSEL;
	const SYSCON_RESERVED_reg_t RESERVED_18;
	SYSCON_CLKOUTSEL_reg_t CLKOUTSEL;
	SYSCON_CLKOUTDIV_reg_t CLKOUTDIV;
	const SYSCON_RESERVED_reg_t RESERVED_19;
	SYSCON_EXTTRACECMD_reg_t EXTTRACECMD;
	const SYSCON_PIOPORCAP_reg_t PIOPORCAP[2];
	const SYSCON_RESERVED_reg_t RESERVED_20[0x2C / 4];
	SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV6;
	SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV5;
	SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV4;
	SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV3;
	SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV2;
	SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV1;
	SYSCON_IOCONCLKDIV_reg_t IOCONCLKDIV0;
	SYSCON_BODCTRL_reg_t BODCTRL;
	SYSCON_SYSTCKCAL_reg_t SYSTCKCAL;
	const SYSCON_RESERVED_reg_t RESERVED_21[0x18 / 4];
	SYSCON_IRQLATENCY_reg_t IRQLATENCY;
	SYSCON_NMISRC_reg_t NMISRC;
	SYSCON_PINTSEL_reg_t PINTSEL[8];
	const SYSCON_RESERVED_reg_t RESERVED_22[0x6C / 4];
	SYSCON_STARTERP0_reg_t STARTERP0;
	const SYSCON_RESERVED_reg_t RESERVED_23[0x0C / 4];
	SYSCON_STARTERP1_reg_t STARTERP1;
	const SYSCON_RESERVED_reg_t RESERVED_24[0x18 / 4];
	SYSCON_PDSLEEPCFG_reg_t PDSLEEPCFG;
	SYSCON_PDAWAKECFG_reg_t PDAWAKECFG;
	SYSCON_PDRUNCFG_reg_t PDRUNCFG;
	const SYSCON_RESERVED_reg_t RESERVED_25[0x1BC / 4];
	const SYSCON_DEVICE_ID_reg_t DEVICE_ID;
}SYSCON_per_t;

extern volatile SYSCON_per_t * const SYSCON; //!< Periferico SYSCON

#endif /* HRI_SYSCON_H_ */
