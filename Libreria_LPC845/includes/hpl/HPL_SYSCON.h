/**
 * @file HPL_SYSCON.h
 * @brief Declaraciones a nivel de abstraccion de periferico del SYSCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_SYSCON_H_
#define HPL_SYSCON_H_

#include <stdint.h>
#include <HRI_SYSCON.h>

typedef enum
{
	SYSCON_BYPASS_DISABLED = 0,
	SYSCON_BYPASS_ENABLED
}SYSCON_bypass_sel_en;

typedef enum
{
	SYSCON_FREQRANGE_MINUS_20MHZ = 0,
	SYSCON_FREQRANGE_PLUS_20MHZ
}SYSCON_freqrange_sel_en;

typedef enum
{
	SYSCON_WATCHDOG_CLKANA_0KHZ = 0,
	SYSCON_WATCHDOG_CLKANA_600KHZ,
	SYSCON_WATCHDOG_CLKANA_1050KHZ,
	SYSCON_WATCHDOG_CLKANA_1400KHZ,
	SYSCON_WATCHDOG_CLKANA_1750KHZ,
	SYSCON_WATCHDOG_CLKANA_2100KHZ,
	SYSCON_WATCHDOG_CLKANA_2400KHZ,
	SYSCON_WATCHDOG_CLKANA_3000KHZ,
	SYSCON_WATCHDOG_CLKANA_3250KHZ,
	SYSCON_WATCHDOG_CLKANA_3500KHZ,
	SYSCON_WATCHDOG_CLKANA_3750KHZ,
	SYSCON_WATCHDOG_CLKANA_4000KHZ,
	SYSCON_WATCHDOG_CLKANA_4200KHZ,
	SYSCON_WATCHDOG_CLKANA_4400KHZ,
	SYSCON_WATCHDOG_CLKANA_4600KHZ
}SYSCON_watchdog_clkana_sel_en;

typedef enum
{
	SYSCON_PLL_SOURCE_SEL_FRO = 0,
	SYSCON_PLL_SOURCE_EXT_SEL_OSC
}SYSCON_pll_source_sel_en;

typedef enum
{
	SYSCON_MAIN_CLOCK_SEL_FRO = 0,
	SYSCON_MAIN_CLOCK_SEL_EXT_CLK,
	SYSCON_MAIN_CLOCK_SEL_WATCHDOG,
	SYSCON_MAIN_CLOCK_SEL_FRO_DIV,
	SYSCON_MAIN_CLOCK_SEL_PLL
}SYSCON_main_clock_sel_en;

typedef enum
{
	SYSCON_CAPACITIVE_CLOCK_SEL_FRO = 0,
	SYSCON_CAPACITIVE_CLOCK_SEL_MAIN_CLOCK,
	SYSCON_CAPACITIVE_CLOCK_SEL_SYS_PLL,
	SYSCON_CAPACITIVE_CLOCK_SEL_FRO_DIV,
	SYSCON_CAPACITIVE_CLOCK_SEL_WATCHDOG_OSC
}SYSCON_capacitive_clock_sel_en;

typedef enum
{
	SYSCON_ADC_CLOCK_SEL_FRO = 0,
	SYSCON_ADC_CLOCK_SEL_SYS_PLL
}SYSCON_adc_clock_sel_en;

typedef enum
{
	SYSCON_SCT_CLOCK_SEL_FRO = 0,
	SYSCON_SCT_CLOCK_SEL_MAIN_CLOCK,
	SYSCON_SCT_CLOCK_SEL_SYS_PLL
}SYSCON_sct_clock_sel_en;

typedef enum
{
	SYSCON_EXT_CLOCK_SOURCE_SEL_CRYSTAL = 0,
	SYSCON_EXT_CLOCK_SOURCE_SEL_CLK_IN
}SYSCON_ext_clock_source_sel_en;

typedef enum
{
	SYSCON_ENABLE_CLOCK_SEL_ROM = 1,
	SYSCON_ENABLE_CLOCK_SEL_RAM,
	SYSCON_ENABLE_CLOCK_SEL_FLASH = 4,
	SYSCON_ENABLE_CLOCK_SEL_IIC0,
	SYSCON_ENABLE_CLOCK_SEL_GPIO0,
	SYSCON_ENABLE_CLOCK_SEL_SWM,
	SYSCON_ENABLE_CLOCK_SEL_SCT,
	SYSCON_ENABLE_CLOCK_SEL_WKT,
	SYSCON_ENABLE_CLOCK_SEL_MRT,
	SYSCON_ENABLE_CLOCK_SEL_SPI0,
	SYSCON_ENABLE_CLOCK_SEL_SPI1,
	SYSCON_ENABLE_CLOCK_SEL_CRC,
	SYSCON_ENABLE_CLOCK_SEL_UART0,
	SYSCON_ENABLE_CLOCK_SEL_UART1,
	SYSCON_ENABLE_CLOCK_SEL_UART2,
	SYSCON_ENABLE_CLOCK_SEL_WWDT,
	SYSCON_ENABLE_CLOCK_SEL_IOCON,
	SYSCON_ENABLE_CLOCK_SEL_ACMP,
	SYSCON_ENABLE_CLOCK_SEL_GPIO1,
	SYSCON_ENABLE_CLOCK_SEL_IIC1,
	SYSCON_ENABLE_CLOCK_SEL_IIC2,
	SYSCON_ENABLE_CLOCK_SEL_IIC3,
	SYSCON_ENABLE_CLOCK_SEL_ADC,
	SYSCON_ENABLE_CLOCK_SEL_CTIMER,
	SYSCON_ENABLE_CLOCK_SEL_MTB,
	SYSCON_ENABLE_CLOCK_SEL_DAC0,
	SYSCON_ENABLE_CLOCK_SEL_GPIO_INT,
	SYSCON_ENABLE_CLOCK_SEL_DMA,
	SYSCON_ENABLE_CLOCK_SEL_UART3,
	SYSCON_ENABLE_CLOCK_SEL_UART4,
	SYSCON_ENABLE_CLOCK_SEL_CAPT,
	SYSCON_ENABLE_CLOCK_SEL_DAC1
}SYSCON_enable_clock_sel_en;

typedef enum
{
	SYSCON_RESET_SEL_FLASH = 4,
	SYSCON_RESET_SEL_IIC0,
	SYSCON_RESET_SEL_GPIO0,
	SYSCON_RESET_SEL_SWM,
	SYSCON_RESET_SEL_SCT,
	SYSCON_RESET_SEL_WKT,
	SYSCON_RESET_SEL_MRT,
	SYSCON_RESET_SEL_SPI0,
	SYSCON_RESET_SEL_SPI1,
	SYSCON_RESET_SEL_CRC,
	SYSCON_RESET_SEL_UART0,
	SYSCON_RESET_SEL_UART1,
	SYSCON_RESET_SEL_UART2,
	SYSCON_RESET_SEL_IOCON = 18,
	SYSCON_RESET_SEL_ACMP,
	SYSCON_RESET_SEL_GPIO1,
	SYSCON_RESET_SEL_IIC1,
	SYSCON_RESET_SEL_IIC2,
	SYSCON_RESET_SEL_IIC3,
	SYSCON_RESET_SEL_ADC,
	SYSCON_RESET_SEL_CTIMER,
	SYSCON_RESET_SEL_DAC0 = 27,
	SYSCON_RESET_SEL_GPIOINT,
	SYSCON_RESET_SEL_DMA,
	SYSCON_RESET_SEL_UART3,
	SYSCON_RESET_SEL_UART4,
	SYSCON_RESET_SEL_CAPT,
	SYSCON_RESET_SEL_DAC1,
	SYSCON_RESET_SEL_FRG0 = 35,
	SYSCON_RESET_SEL_FRG1
}SYSCON_reset_sel_en;

typedef enum
{
	SYSCON_PERIPHERAL_SEL_UART0 = 0,
	SYSCON_PERIPHERAL_SEL_UART1,
	SYSCON_PERIPHERAL_SEL_UART2,
	SYSCON_PERIPHERAL_SEL_UART3,
	SYSCON_PERIPHERAL_SEL_UART4,
	SYSCON_PERIPHERAL_SEL_I2C0,
	SYSCON_PERIPHERAL_SEL_I2C1,
	SYSCON_PERIPHERAL_SEL_I2C2,
	SYSCON_PERIPHERAL_SEL_I2C3,
	SYSCON_PERIPHERAL_SEL_SPI0,
	SYSCON_PERIPHERAL_SEL_SPI1
}SYSCON_peripheral_sel_en;

typedef enum
{
	SYSCON_PERIPHERAL_CLOCK_SEL_FRO = 0,
	SYSCON_PERIPHERAL_CLOCK_SEL_MAIN,
	SYSCON_PERIPHERAL_CLOCK_SEL_FRG0,
	SYSCON_PERIPHERAL_CLOCK_SEL_FRG1,
	SYSCON_PERIPHERAL_CLOCK_SEL_FRO_DIV,
	SYSCON_PERIPHERAL_CLOCK_SEL_NONE = 7
}SYSCON_peripheral_clock_sel_en;

typedef enum
{
	SYSCON_FRG_CLOCK_SEL_FRO = 0,
	SYSCON_FRG_CLOCK_SEL_MAIN_CLOCK,
	SYSCON_FRG_CLOCK_SEL_SYS_PLL,
	SYSCON_FRG_CLOCK_SEL_NONE
}SYSCON_frg_clock_sel_en;

typedef enum
{
	SYSCON_CLKOUT_SOURCE_SEL_FRO = 0,
	SYSCON_CLKOUT_SOURCE_SEL_MAIN_CLOCK,
	SYSCON_CLKOUT_SOURCE_SEL_SYS_PLL,
	SYSCON_CLKOUT_SOURCE_SEL_EXT_CLOCK,
	SYSCON_CLKOUT_SOURCE_SEL_WATCHDOG_OSC
}SYSCON_clkout_source_sel_en;

typedef enum
{
	SYSCON_BOD_LEVEL_1 = 1,
	SYSCON_BOD_LEVEL_2,
	SYSCON_BOD_LEVEL_3
}SYSCON_bod_level_en;

typedef enum
{
	SYSCON_BOD_DISABLE = 0,
	SYSCON_BOD_ENABLE
}SYSCON_bod_enale_en;

typedef enum
{
	SYSCON_NMI_DISABLE = 0,
	SYSCON_NMI_ENABLE
}SYSCON_nmi_enable_en;

typedef enum
{
	SYSCON_WAKEUP_ENABLE_SEL_PINT0 = 0,
	SYSCON_WAKEUP_ENABLE_SEL_PINT1,
	SYSCON_WAKEUP_ENABLE_SEL_PINT2,
	SYSCON_WAKEUP_ENABLE_SEL_PINT3,
	SYSCON_WAKEUP_ENABLE_SEL_PINT4,
	SYSCON_WAKEUP_ENABLE_SEL_PINT5,
	SYSCON_WAKEUP_ENABLE_SEL_PINT6,
	SYSCON_WAKEUP_ENABLE_SEL_PINT7,
	SYSCON_WAKEUP_ENABLE_SEL_SPI0 = 32,
	SYSCON_WAKEUP_ENABLE_SEL_SPI1,
	SYSCON_WAKEUP_ENABLE_SEL_USART0 = 35,
	SYSCON_WAKEUP_ENABLE_SEL_USART1,
	SYSCON_WAKEUP_ENABLE_SEL_USART2,
	SYSCON_WAKEUP_ENABLE_SEL_IIC1 = 39,
	SYSCON_WAKEUP_ENABLE_SEL_IIC0,
	SYSCON_WAKEUP_ENABLE_SEL_CAPTOUCH = 43,
	SYSCON_WAKEUP_ENABLE_SEL_WWDT,
	SYSCON_WAKEUP_ENABLE_SEL_BOD,
	SYSCON_WAKEUP_ENABLE_SEL_WKT = 47,
	SYSCON_WAKEUP_ENABLE_SEL_IIC2 = 53,
	SYSCON_WAKEUP_ENABLE_SEL_IIC3,
	SYSCON_WAKEUP_ENABLE_SEL_USART3 = 62,
	SYSCON_WAKEUP_ENABLE_SEL_USART4
}SYSCON_enable_wakeup_sel_en;

typedef enum
{
	SYSCON_DEEP_SLEEP_POWERED = 0,
	SYSCON_DEEP_SLEEP_POWERED_DOWN
}SYSCON_deep_sleep_power_en;

typedef enum
{
	SYSCON_WAKEUP_POWER_SEL_FROOUT = 0,
	SYSCON_WAKEUP_POWER_SEL_FRO,
	SYSCON_WAKEUP_POWER_SEL_FLASH,
	SYSCON_WAKEUP_POWER_SEL_BOD,
	SYSCON_WAKEUP_POWER_SEL_ADC,
	SYSCON_WAKEUP_POWER_SEL_SYSOSC,
	SYSCON_WAKEUP_POWER_SEL_WDTOSC,
	SYSCON_WAKEUP_POWER_SEL_SYSPLL,
	SYSCON_WAKEUP_POWER_SEL_VREF2 = 10,
	SYSCON_WAKEUP_POWER_SEL_DAC0 = 13,
	SYSCON_WAKEUP_POWER_SEL_DAC1,
	SYSCON_WAKEUP_POWER_SEL_ACMP
}SYSCON_wakeup_power_sel_en;

typedef enum
{
	SYSCON_POWER_SEL_FROOUT = 0,
	SYSCON_POWER_SEL_FRO,
	SYSCON_POWER_SEL_FLASH,
	SYSCON_POWER_SEL_BOD,
	SYSCON_POWER_SEL_ADC,
	SYSCON_POWER_SEL_SYSOSC,
	SYSCON_POWER_SEL_WDTOSC,
	SYSCON_POWER_SEL_SYSPLL,
	SYSCON_POWER_SEL_DAC0 = 13,
	SYSCON_POWER_SEL_DAC1,
	SYSCON_POWER_SEL_ACMP
}SYSCON_power_sel_en;

/**
 * @brief Configuracion del registro de control del PLL
 * @param[in] m Valor del divisor del feedback
 * @param[in] p Valor del post divisor
 */
static inline void SYSCON_set_pll_control(uint8_t m, uint8_t p)
{
	SYSCON->SYSPLLCTRL.MSEL = m;
	SYSCON->SYSPLLCTRL.PSEL = p;
}

/**
 * @brief Obtener estado de lock del PLL
 * @return Valor del estado de lock del PLL
 */
static inline uint8_t SYSCON_get_pll_lock_status(void)
{
	return SYSCON->SYSPLLSTAT.LOCK;
}

/**
 * @brief Configurar el registro de control del sistema del oscilador
 * @param[in] bypass Seleccion de bypass
 * @param[in] freqrange Rango de frecuencia a utilizar
 */
static inline void SYSCON_set_oscillator_control(SYSCON_bypass_sel_en bypass, SYSCON_freqrange_sel_en freqrange)
{
	SYSCON->SYSOSCCTRL.BYPASS = bypass;
	SYSCON->SYSOSCCTRL.FREQRANGE = freqrange;
}

/**
 * @brief Configuracion del registro de control del oscilador del watchdog
 * @param[in] divsel Seleccion de divsel
 * @param[in] clkana_sel Seleccion de frecuencia base
 */
static inline void SYSCON_set_watchdog_oscillator_control(uint8_t divsel, SYSCON_watchdog_clkana_sel_en clkana_sel)
{
	SYSCON->WDTOSCCTRL.DIVSEL = divsel;
	SYSCON->WDTOSCCTRL.FREQSEL = clkana_sel;
}

/**
 * @brief Configuracion del FRO para que sea el oscilador directo (sin division)
 */
static inline void SYSCON_set_fro_direct(void)
{
	SYSCON->FROOSCCTRL.FRO_DIRECT = 1;
	SYSCON->FRODIRECTCLKUEN.ENA = 0;
	SYSCON->FRODIRECTCLKUEN.ENA = 1;
}

/**
 * @brief Configuracion del FRO para que sea el oscilador dividido (con division dependiente de FAIM)
 */
static inline void SYSCON_clear_fro_direct(void)
{
	SYSCON->FROOSCCTRL.FRO_DIRECT = 0;
	SYSCON->FRODIRECTCLKUEN.ENA = 0;
	SYSCON->FRODIRECTCLKUEN.ENA = 1;
}

/**
 * @brief Configuracion de la fuente de clock para el PLL
 * @param[in] pll_source Fuente de entrada para el PLL
 */
static inline void SYSCON_set_pll_clk_source(SYSCON_pll_source_sel_en pll_source)
{
	SYSCON->SYSPLLCLKSEL.SEL = pll_source;

	SYSCON->SYSPLLCLKUEN.ENA = 0;
	SYSCON->SYSPLLCLKUEN.ENA = 1;
}

/*
 * @brief Seleccion de fuente de clock para el clock principal
 * @param[in] clock_selection Seleccion de clock deseada
 */
static inline void SYSCON_set_system_clock_source(SYSCON_main_clock_sel_en clock_selection)
{
	if(clock_selection != SYSCON_MAIN_CLOCK_SEL_PLL)
	{
		SYSCON->MAINCLKSEL.SEL = clock_selection;
		SYSCON->MAINCLKUEN.ENA = 0;
		SYSCON->MAINCLKUEN.ENA = 1;

		SYSCON->MAINCLKPLLSEL.SEL = 0;
		SYSCON->MAINCLKPLLUEN.ENA = 0;
		SYSCON->MAINCLKPLLUEN.ENA = 1;
	}
	else
	{
		SYSCON->MAINCLKPLLSEL.SEL = 1;
		SYSCON->MAINCLKPLLUEN.ENA = 0;
		SYSCON->MAINCLKPLLUEN.ENA = 1;
	}
}

/*
 * @brief Seleccion del divisor del system clock
 * @param[in] divider division deseada (cero desactiva el system clock)
 */
static inline void SYSCON_set_system_clock_divider(uint8_t divider)
{
	SYSCON->SYSAHBCLKDIV.DIV = divider;
}

/**
 * @brief Seleccion de la fuente de clock para el periferico de control de touch capacitivo
 * @param[in] source_sel Seleccion de fuente de clock deseada
 */
static inline void SYSCON_set_capacitive_clock_source(SYSCON_capacitive_clock_sel_en source_sel)
{
	SYSCON->CAPTCLKSEL.SEL = source_sel;
}

/**
 * @brief Seleccion de clock y divisor para el periferico ADC
 * @param[in] clock_sel Fuente de clock deseada para el periferico
 * @param[in] div Divisor deseado
 */
static inline void SYSCON_set_adc_clock(SYSCON_adc_clock_sel_en source_sel, uint8_t div)
{
	SYSCON->ADCCLKSEL.SEL = source_sel;
	SYSCON->ADCCLKDIV.DIV = div;
}

/**
 * @brief Seleccion de clock y divisor para el periferico SCT
 * @param[in] clock_sel Fuente de clock deseada para el periferico
 * @param[in] div Divisor deseado
 */
static inline void SYSCON_set_sct_clock(SYSCON_sct_clock_sel_en source_sel, uint8_t div)
{
	SYSCON->SCTCLKSEL.SEL = source_sel;
	SYSCON->SCTCLKDIV.DIV = div;
}

/**
 * @brief Seleccion de fuente para el clock externo
 * @param[in] source_selection Seleccion deseada
 */
static inline void SYSCON_set_ext_clock_source(SYSCON_ext_clock_source_sel_en source_selection)
{
	SYSCON->EXTCLKSEL.SEL = source_selection;
}

/**
 * @brief Habilitacion del clock de un periferico
 * @param[in] peripheral Periferico en el cual habilitar el clock
 */
static inline void SYSCON_enable_clock(SYSCON_enable_clock_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->SYSAHBCLKCTRL0) |= (1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->SYSAHBCLKCTRL1) |= (1 << peripheral);
	}
}

/**
 * @brief Inhabilitacion del clock de un periferico
 * @param[in] peripheral Periferico en el cual habilitar el clock
 */
static inline void SYSCON_disable_clock(SYSCON_enable_clock_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->SYSAHBCLKCTRL0) |= (1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->SYSAHBCLKCTRL1) |= (1 << peripheral);
	}
}

/**
 * @brief Generar el reset en el periferico seleccionado
 * @param[in] peripheral Periferico a generar el reset
 */
static inline void SYSCON_assert_reset(SYSCON_reset_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->PRESETCTRL0) &= ~(1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->PRESETCTRL1) &= ~(1 << peripheral);
	}
}

/**
 * @brief Liberar el reset en el periferico seleccionado
 * @param[in] peripheral Periferico a liberar el reset
 */
static inline void SYSCON_clear_reset(SYSCON_reset_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->PRESETCTRL0) |= (1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->PRESETCTRL1) |= (1 << peripheral);
	}
}

/**
 * @brief Seleccion de fuente de clock para los distintos perifericos
 * @param[in] peripheral Periferico cuya fuente seleccionar
 * @param[in] clock Fuente de clock para el periferico seleccionada
 */
static inline void SYSCON_set_peripheral_clock_source(SYSCON_peripheral_sel_en peripheral, SYSCON_peripheral_clock_sel_en clock)
{
	SYSCON->PERCLKSEL[peripheral].SEL = clock;
}

/**
 * @brief Configuracion del FRG
 * @param[in] frg_selection Cual de los FRG configurar, cero o uno
 * @param[in] clock_source Fuente de clock del FRG
 * @param[in] mul Multiplicador del FRG 0 ~ 255
 * @param[in] div Divisor del FRG 0~255
 */
static inline void SYSCON_set_frg_config(uint8_t frg_selection, SYSCON_frg_clock_sel_en clock_source, uint8_t mul, uint8_t div)
{
	if(frg_selection == 0)
	{
		SYSCON->FRG0CLKSEL.SEL = clock_source;
		SYSCON->FRG0DIV.DIV = div;
		SYSCON->FRG0MUL.MULT = mul;
	}
	else if(frg_selection == 1)
	{
		SYSCON->FRG1CLKSEL.SEL = clock_source;
		SYSCON->FRG1DIV.DIV = div;
		SYSCON->FRG1MUL.MULT = mul;
	}
}

/**
 * @brief Seleccion de fuente para el CLOCK OUT
 * @param[in] clock_source Fuente deseada
 * @param[in] divider Divisor del CLOCK OUT
 */
static inline void SYSCON_set_clkout_config(SYSCON_clkout_source_sel_en clock_source, uint8_t divider)
{
	SYSCON->CLKOUTSEL.SEL = clock_source;
	SYSCON->CLKOUTDIV.DIV = divider;
}

/**
 * @brief Leer el contenido de PIOPORCAPn
 * @param[in] inst Instancia a leer (0 o 1)
 * @return Valor del registro leido
 */
static inline uint32_t SYSCON_get_por_pio_status_register(uint8_t inst)
{
	return SYSCON->PIOPORCAP[inst].PIOSTAT;
}

/**
 * @brief Fijar el divisor para el filtro de glitches del IOCON
 * @param[in] inst Instancia a escribir
 * @param[in] div Divisor deseado
 */
static inline void SYSCON_set_iocon_glitch_divider(uint8_t inst, uint8_t div)
{
	switch(inst)
	{
	case 0: { SYSCON->IOCONCLKDIV0.DIV = div; break; }
	case 1: { SYSCON->IOCONCLKDIV1.DIV = div; break; }
	case 2: { SYSCON->IOCONCLKDIV2.DIV = div; break; }
	case 3: { SYSCON->IOCONCLKDIV3.DIV = div; break; }
	case 4: { SYSCON->IOCONCLKDIV4.DIV = div; break; }
	case 5: { SYSCON->IOCONCLKDIV5.DIV = div; break; }
	case 6: { SYSCON->IOCONCLKDIV6.DIV = div; break; }
	}
}

/**
 * @brief Configurar el registro de control del brown-out detector
 * @param[in] reset_level Nivel deseado para el reset
 * @param[in] bod_level Nivel deseado para el BOD
 * @param[in] reset_enable Habilitacion deseada para el reset
 */
static inline void SYSCON_set_bod_control(SYSCON_bod_level_en reset_level, SYSCON_bod_level_en bod_level, SYSCON_bod_enale_en reset_enable)
{
	SYSCON->BODCTRL.BODRSTLEV = reset_level;
	SYSCON->BODCTRL.BODINTVAL = bod_level;
	SYSCON->BODCTRL.BODRSTENA = reset_enable;
}

/**
 * @brief Obtener el valor de calibracion del SYSTICK
 * @return Valor de calibracion del SYSTICK
 */
static inline uint32_t SYSCON_get_systick_calib(void)
{
	return SYSCON->SYSTCKCAL.CAL;
}

/**
 * @brief Obtener el valor de latencia de interrupciones del MCU
 * @return Valor de latencia de interrupcion
 */
static inline uint8_t SYSCON_get_irq_latency(void)
{
	return SYSCON->IRQLATENCY.LATENCY;
}

/**
 * @brief Fijar que numero de interrupcion actuara como NMI
 * @param[in] irq Numero de interrupcion deseado
 * @param[in] enable Habilitacion/inhabilitacion de la NMI
 */
static inline void SYSCON_set_nmi_source(uint8_t irq, SYSCON_nmi_enable_en enable)
{
	SYSCON->NMISRC.IRQN = irq;
	SYSCON->NMISRC.NMIEN = enable;
}

/**
 * @brief Habilitar alguna de las interrupciones del periferico seleccionado como fuente de wakeup
 * @param[in] peripheral Periferico deseado
 */
static inline void SYSCON_enable_wakeup_source(SYSCON_enable_wakeup_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->STARTERP0) |= (1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->STARTERP1) |= (1 << peripheral);
	}
}

/**
 * @brief Inhabilitar alguna de las interrupciones del periferico seleccionado como fuente de wakeup
 * @param[in] peripheral Periferico deseado
 */
static inline void SYSCON_disable_wakeup_source(SYSCON_enable_wakeup_sel_en peripheral)
{
	if(peripheral < 32)
	{
		*((uint32_t *) &SYSCON->STARTERP0) &= ~(1 << peripheral);
	}
	else
	{
		peripheral -= 32;
		*((uint32_t *) &SYSCON->STARTERP1) &= ~(1 << peripheral);
	}
}

/**
 * @brief Habilitar o inhabilitacion de la alimentacion del BOD en deep sleep
 * @param[in] power Habilitacion o inhabilitacion de la alimentacion
 */
static inline void SYSCON_deep_sleep_power_bod(SYSCON_deep_sleep_power_en power)
{
	SYSCON->PDSLEEPCFG.BOD_PD = power;
}

/**
 * @brief Habilitar o inhabilitacion de la alimentacion del WDTOSC en deep sleep
 * @param[in] power Habilitacion o inhabilitacion de la alimentacion
 */
static inline void SYSCON_deep_sleep_power_wdtosc(SYSCON_deep_sleep_power_en power)
{
	SYSCON->PDSLEEPCFG.WDTOSC_PD = power;
}

/**
 * @brief Fijar que un periferico comience encendido al haber un wakeup
 * @param[in] peripheral Periferico que comenzara encendido al haber un wakeup
 */
static inline void SYSCON_set_powered_on_wakeup(SYSCON_wakeup_power_sel_en peripheral)
{
	*((uint32_t*) &SYSCON->PDAWAKECFG) &= ~(1 << peripheral);
}

/**
 * @brief Fijar que un periferico comience apagado al haber un wakeup
 * @param[in] peripheral Periferico que comenzara apagado al haber un wakeup
 */
static inline void SYSCON_clear_powered_on_wakeup(SYSCON_wakeup_power_sel_en peripheral)
{
	*((uint32_t*) &SYSCON->PDAWAKECFG) |= (1 << peripheral);
}

/**
 * @brief Encender el periferico seleccionado
 * @param[in] peripheral Periferico a encender
 */
static inline void SYSCON_power_up_peripheral(SYSCON_power_sel_en peripheral)
{
	*((uint32_t*) &SYSCON->PDRUNCFG) &= ~(1 << peripheral);
}

/**
 * @brief Apagar el periferico seleccionado
 * @param[in] peripheral Periferico a encender
 */
static inline void SYSCON_power_down_peripheral(SYSCON_power_sel_en peripheral)
{
	*((uint32_t*) &SYSCON->PDRUNCFG) |= (1 << peripheral);
}

/**
 * @brief Obtener el Device ID
 * @return Device ID
 */
static inline uint32_t SYSCON_get_device_id(void)
{
	return SYSCON->DEVICE_ID.DEVICE_ID;
}

#endif /* HPL_SYSCON_H_ */
