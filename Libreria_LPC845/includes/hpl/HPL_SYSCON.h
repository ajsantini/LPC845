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
	SYSCON_SYSTEM_CLOCK_SEL_FRO = 0,
	SYSCON_SYSTEM_CLOCK_SEL_EXT_CLK,
	SYSCON_SYSTEM_CLOCK_SEL_WATCHDOG,
	SYSCON_SYSTEM_CLOCK_SEL_FRO_DIV,
	SYSCON_SYSTEM_CLOCK_SEL_PLL
}SYSCON_system_clock_sel_en;

typedef enum
{
	SYSCON_EXT_CLOCK_SOURCE_SEL_CRYSTAL = 0,
	SYSCON_EXT_CLOCK_SOURCE_SEL_CLK_IN
}SYSCON_ext_clock_source_sel_en;

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
	SYSCON_BYPASS_DISABLED = 0,
	SYSCON_BYPASS_ENABLED
}SYSCON_bypass_sel_en;

typedef enum
{
	SYSCON_FREQRANGE_MINUS_20MHZ = 0,
	SYSCON_FREQRANGE_PLUS_20MHZ
}SYSCON_freqrange_sel_en;

/*
 * @brief Seleccion de fuente de clock para el clock principal
 * @param[in] clock_selection Seleccion de clock deseada
 */
static inline void SYSCON_set_system_clock_source(SYSCON_system_clock_sel_en clock_selection)
{
	if(clock_selection != SYSCON_SYSTEM_CLOCK_SEL_PLL)
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
 * @brief Inicializacion del clock watchdog oscillator
 * @param[in] clkana_selection Seleccion de la frecuencia base
 * @param[in] divider Divisor deseado (se redondeara al multiplo de 2 mas cercano)
 */
static inline void SYSCON_set_watchdog_osc_clock(SYSCON_watchdog_clkana_sel_en clkana_selection, uint8_t divider)
{
	SYSCON->WDTOSCCTRL.FREQSEL = clkana_selection;
	SYSCON->WDTOSCCTRL.DIVSEL = divider;
}

/**
 * @brief Configuracion del FRO para que sea el oscilador directo (sin division)
 */
void SYSCON_set_fro_direct(void)
{
	SYSCON->FROOSCCTRL.FRO_DIRECT = 1;
	SYSCON->FRODIRECTCLKUEN.ENA = 0;
	SYSCON->FRODIRECTCLKUEN.ENA = 1;
}

/**
 * @brief Configuracion del FRO para que sea el oscilador dividido (con division dependiente de FAIM)
 */
void SYSCON_clear_fro_direct(void)
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
 * @brief Seleccion del divisor del system clock
 * @param[in] divider division deseada (cero desactiva el system clock)
 */
static inline void SYSCON_set_system_clock_divider(uint8_t divider)
{
	SYSCON->SYSAHBCLKDIV.DIV = divider;
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
void SYSCON_set_frg_config(uint8_t frg_selection, SYSCON_frg_clock_sel_en clock_source, uint8_t mul, uint8_t div)
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
 * @brief Configurar algun registro PINTSEL
 * @param[in] pintsel Cual de los registros configurar 0~7
 * @param[in] port Numero de puerto al cual asignar la funcion
 * @param[in] pin Numero de pin al cual asignar la funcion
 */
static inline void SYSCON_set_pintsel(uint8_t pintsel, uint8_t port, uint8_t pin)
{
	SYSCON->PINTSEL[pintsel].INTPIN = (port * 32) + pin;
}

#endif /* HPL_SYSCON_H_ */
