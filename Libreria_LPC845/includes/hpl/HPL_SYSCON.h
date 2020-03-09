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

typedef enum
{
	SYSTEM_CLOCK_SEL_FRO = 0,
	SYSTEM_CLOCK_SEL_EXT_CLK,
	SYSTEM_CLOCK_SEL_WATCHDOG,
	SYSTEM_CLOCK_SEL_FRO_DIV,
	SYSTEM_CLOCK_SEL_PLL
}system_clock_selection_en;

typedef enum
{
	EXT_CLOCK_SOURCE_SELECTION_CRYSTAL = 0,
	EXT_CLOCK_SOURCE_SELECTION_CLK_IN
}ext_clock_source_selection_en;

typedef enum
{
	WATCHDOG_CLKANA_0KHZ = 0,
	WATCHDOG_CLKANA_600KHZ,
	WATCHDOG_CLKANA_1050KHZ,
	WATCHDOG_CLKANA_1400KHZ,
	WATCHDOG_CLKANA_1750KHZ,
	WATCHDOG_CLKANA_2100KHZ,
	WATCHDOG_CLKANA_2400KHZ,
	WATCHDOG_CLKANA_3000KHZ,
	WATCHDOG_CLKANA_3250KHZ,
	WATCHDOG_CLKANA_3500KHZ,
	WATCHDOG_CLKANA_3750KHZ,
	WATCHDOG_CLKANA_4000KHZ,
	WATCHDOG_CLKANA_4200KHZ,
	WATCHDOG_CLKANA_4400KHZ,
	WATCHDOG_CLKANA_4600KHZ
}watchdog_clkana_selection_en;

typedef enum
{
	PLL_SOURCE_FRO = 0,
	PLL_SOURCE_EXT_OSC
}pll_source_selection_en;

typedef enum
{
	PERIPHERAL_SELECTION_UART0 = 0,
	PERIPHERAL_SELECTION_UART1,
	PERIPHERAL_SELECTION_UART2,
	PERIPHERAL_SELECTION_UART3,
	PERIPHERAL_SELECTION_UART4,
	PERIPHERAL_SELECTION_I2C0,
	PERIPHERAL_SELECTION_I2C1,
	PERIPHERAL_SELECTION_I2C2,
	PERIPHERAL_SELECTION_I2C3,
	PERIPHERAL_SELECTION_SPI0,
	PERIPHERAL_SELECTION_SPI1
}peripheral_selection_en;

typedef enum
{
	PERIPHERAL_CLOCK_SELECTION_FRO = 0,
	PERIPHERAL_CLOCK_SELECTION_MAIN,
	PERIPHERAL_CLOCK_SELECTION_FRG0,
	PERIPHERAL_CLOCK_SELECTION_FRG1,
	PERIPHERAL_CLOCK_SELECTION_FRO_DIV,
	PERIPHERAL_CLOCK_SELECTION_NONE = 7
}peripheral_clock_selection_en;

typedef enum
{
	FRG_CLOCK_SELECTION_FRO = 0,
	FRG_CLOCK_SELECTION_MAIN_CLOCK,
	FRG_CLOCK_SELECTION_SYS_PLL,
	FRG_CLOCK_SELECTION_NONE
}frg_clock_selection_en;

typedef enum
{
	CLKOUT_SOURCE_SELECTION_FRO = 0,
	CLKOUT_SOURCE_SELECTION_MAIN_CLOCK,
	CLKOUT_SOURCE_SELECTION_SYS_PLL,
	CLKOUT_SOURCE_SELECTION_EXT_CLOCK,
	CLKOUT_SOURCE_SELECTION_WATCHDOG_OSC
}clkout_source_selection_en;

/*
 * @brief Seleccion de fuente de clock para el clock principal
 * @param[in] clock_selection Seleccion de clock deseada
 */
void SYSCON_set_system_clock_source(system_clock_selection_en clock_selection);

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
void SYSCON_set_system_clock_divider(uint8_t divider);

/**
 * @brief Seleccion de fuente para el clock externo
 * @param[in] source_selection Seleccion deseada
 */
void SYSCON_set_ext_clock_source(ext_clock_source_selection_en source_selection);

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
void SYSCON_set_watchdog_osc_clock(watchdog_clkana_selection_en clkana_selection, uint8_t divider);

/**
 * @brief Inicializacion del PLL
 * @param[in] pll_source Fuente de entrada para el PLL
 * @param[in] pll_multiplier Multiplicador de frecuencia del PLL
 */
void SYSCON_set_PLL(pll_source_selection_en pll_source, uint8_t pll_multiplier);

/**
 * @brief Seleccion de fuente de clock para los distintos perifericos
 * @param[in] peripheral Periferico cuya fuente seleccionar
 * @param[in] clock Fuente de clock para el periferico seleccionada
 */
void SYSCON_set_peripheral_clock_source(peripheral_selection_en peripheral, peripheral_clock_selection_en clock);

/**
 * @brief Obtencion del clock de los distintos perifericos
 * @param[in] peripheral Periferico cuya fuente seleccionar
 * @return Frecuencia del clock del periferico, en Hz
 */
uint32_t SYSCON_get_peripheral_clock(peripheral_selection_en peripheral);

/**
 * @brief Configuracion del FRG
 * @param[in] frg_selection Cual de los FRG configurar, cero o uno
 * @param[in] clock_source Fuente de clock del FRG
 * @param[in] mul Multiplicador del FRG 0 ~ 255
 * @param[in] div Divisor del FRG 1~256
 */
void SYSCON_set_frg_config(uint8_t frg_selection, frg_clock_selection_en clock_source, uint8_t mul, uint16_t div);

/**
 * @brief Seleccion de fuente para el CLOCK OUT
 * @param[in] clock_source Fuente deseada
 * @param[in] divider Divisor del CLOCK OUT
 * @param[in] port Puerto por el cual sacar el CLOCK OUT
 * @param[in] pin Pin por el cual sacar el CLOCK_OUT
 */
void SYSCON_set_clkout_config(clkout_source_selection_en clock_source, uint8_t divider, uint8_t port, uint8_t pin);

#endif /* HPL_SYSCON_H_ */
