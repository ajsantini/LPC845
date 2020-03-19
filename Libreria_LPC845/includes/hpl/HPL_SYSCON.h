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

/*
 * @brief Seleccion de fuente de clock para el clock principal
 * @param[in] clock_selection Seleccion de clock deseada
 */
void SYSCON_set_system_clock_source(SYSCON_system_clock_sel_en clock_selection);

/**
 * @brief Obtencion de la frecuencia de clock actual
 * @return Frecuencia del clock actual en Hz
 */
uint32_t SYSCON_get_system_clock(void);

/**
 * @brief Obtencion de la frecuencia del FRO actual
 * @return Frecuencia del FRO actual en Hz
 */
uint32_t SYSCON_get_fro_clock(void);

/**
 * @brief Obtencion de la frecuencia del PLL actual
 * @return Frecuencia del PLL actual en Hz
 */
uint32_t SYSCON_get_pll_clock(void);

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
 * @brief Inicializacion del clock con cristal externo
 * @param[in] crystal_frequency Frecuencia del cristal externo utilizado en Hz
 */
void SYSCON_set_crystal_clock(uint32_t crystal_frequency);

/**
 * @brief Inicializacion del clock externo (EXT CLK IN)
 * @param[in] ext_clk_frequency Frecuencia de la fuente externa utilizada, en Hz
 */
void SYSCON_set_ext_in_clock(uint32_t ext_clk_frequency);

/**
 * @brief Configuracion del FRO para que sea el oscilador directo (sin division)
 */
void SYSCON_set_fro_direct(void);

/**
 * @brief Configuracion del FRO para que sea el oscilador dividido (con division dependiente de FAIM)
 */
void SYSCON_clear_fro_direct(void);

/**
 * @brief Inicializacion del clock watchdog oscillator
 * @param[in] clkana_selection Seleccion de la frecuencia base
 * @param[in] divider Divisor deseado (se redondeara al multiplo de 2 mas cercano)
 */
void SYSCON_set_watchdog_osc_clock(SYSCON_watchdog_clkana_sel_en clkana_selection, uint8_t divider);

/**
 * @brief Inicializacion del PLL
 * @param[in] pll_source Fuente de entrada para el PLL
 * @param[in] pll_multiplier Multiplicador de frecuencia del PLL
 */
void SYSCON_set_PLL(SYSCON_pll_source_sel_en pll_source, uint8_t pll_multiplier);

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
 * @brief Obtencion del clock de los distintos perifericos
 * @param[in] peripheral Periferico cuya fuente seleccionar
 * @return Frecuencia del clock del periferico, en Hz
 */
uint32_t SYSCON_get_peripheral_clock(SYSCON_peripheral_sel_en peripheral);

/**
 * @brief Configuracion del FRG
 * @param[in] frg_selection Cual de los FRG configurar, cero o uno
 * @param[in] clock_source Fuente de clock del FRG
 * @param[in] mul Multiplicador del FRG 0 ~ 255
 * @param[in] div Divisor del FRG 1~256
 */
void SYSCON_set_frg_config(uint8_t frg_selection, SYSCON_frg_clock_sel_en clock_source, uint8_t mul, uint16_t div);

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
