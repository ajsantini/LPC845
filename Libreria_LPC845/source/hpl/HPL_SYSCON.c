/**
 * @file HPL_SYSCON.c
 * @brief Funciones a nivel de abstraccion de periferico para el SYSCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#include <stdint.h>
#include <stdio.h>
#include <HPL_IOCON.h>
#include <HPL_SYSCON.h>
#include <HPL_SWM.h>
#include <HRI_SYSCON.h>
#include <HRI_SWM.h>

#define		XTALIN_PORT							0
#define		XTALIN_PIN							8

#define		XTALOUT_PORT						0
#define		XTALOUT_PIN							9

#define		SYSOSCCTRL_FREQRANGE_MINUS_20MHZ	0
#define		SYSOSCCTRL_FREQRANGE_PLUS_20MHZ		1

#define		PDRUNCFG_PD_POWER_UP				0
#define		PDRUNCFG_PD_POWER_DOWN				1

#define		EXTCLKSEL_SYS_OSC					0
#define		EXTCLKSEL_CLK_IN					1

#define		MAINCLKPLLSEL_PRE_PLL				0
#define		MAINCLKPLLSEL_SYS_PLL				1

#define		SYSPLLSTAT_PLL_NOT_LOCKED			0
#define		SYSPLLSTAT_PLL_LOCKED				1

#define		DEFAULT_SYSTEM_CLOCK				12e6

static uint32_t current_crystal_clock = 0; //!< Frecuencia del cristal actual
static uint32_t current_ext_in_clock = 0; //!< Frecuencia del EXT IN CLK
static uint32_t current_fro_clock = DEFAULT_SYSTEM_CLOCK; //!< Frecuencia por default del FRO
static uint32_t current_fro_div_clock = DEFAULT_SYSTEM_CLOCK / 2; //!< Frecuencia por default del FRO_DIV
static uint32_t current_watchdog_clock = 0; //!< Frecuencia del watchdog oscillator
static uint32_t current_pll_clock = 0; //!< Frecuencia del PLL
static uint32_t current_frg0_clock = 0; //!< Frecuencia del FRG0
static uint32_t current_frg1_clock = 0; //!< Frecuencia del FRG1

static uint32_t *current_system_clock = &current_fro_clock; //!< Frecuencia del MAINCLK actual (12MHz en startup)

static const uint32_t watchdog_ana_clock[] = //!< Frecuencias FCLKANA posibles
{
	0e6, 0.6e6, 1.05e6, 1.4e6, 1.75e6, 2.1e6, 2.4e6, 2.7e6,
	3e6, 3.25e6, 3.5e6, 3.75e6, 4e6, 4.2e6, 4.4e6, 4.6e6
};

/**
 * @brief Obtencion de la frecuencia de clock principal actual
 * @return Frecuencia del clock actual en Hz
 */
uint32_t SYSCON_get_system_clock(void)
{
	if(current_system_clock != NULL)
	{
		return (*current_system_clock) / SYSCON->SYSAHBCLKDIV.DIV;
	}
	else
	{
		return DEFAULT_SYSTEM_CLOCK / SYSCON->SYSAHBCLKDIV.DIV;
	}
}

/**
 * @brief Obtencion de la frecuencia del FRO actual
 * @return Frecuencia del FRO actual en Hz
 */
uint32_t SYSCON_get_fro_clock(void)
{
	return current_fro_clock;
}

/**
 * @brief Obtencion de la frecuencia del PLL actual
 * @return Frecuencia del PLL actual en Hz
 */
uint32_t SYSCON_get_pll_clock(void)
{
	return current_pll_clock;
}

/*
 * @brief Seleccion de fuente de clock para el clock principal
 * @param[in] clock_selection Seleccion de clock deseada
 */
void SYSCON_set_system_clock_source(SYSCON_system_clock_sel_en clock_selection)
{
	if(clock_selection != SYSCON_SYSTEM_CLOCK_SEL_PLL)
	{
		switch(SYSCON->MAINCLKSEL.SEL)
		{
		case SYSCON_SYSTEM_CLOCK_SEL_EXT_CLK:
		{
			if(SYSCON->EXTCLKSEL.SEL == EXTCLKSEL_SYS_OSC)
			{
				current_system_clock = &current_crystal_clock;
			}
			else
			{
				current_system_clock = &current_ext_in_clock;
			}

			break;
		}

		case SYSCON_SYSTEM_CLOCK_SEL_FRO:
		{
			current_system_clock = (uint32_t*)(&current_fro_clock);

			break;
		}

		case SYSCON_SYSTEM_CLOCK_SEL_FRO_DIV:
		{
			current_system_clock = (uint32_t*)(&current_fro_div_clock);

			break;
		}

		case SYSCON_SYSTEM_CLOCK_SEL_WATCHDOG:
		{
			current_system_clock = &current_watchdog_clock;

			break;
		}
		}

		SYSCON->MAINCLKSEL.SEL = clock_selection;
		SYSCON->MAINCLKUEN.ENA = 0;
		SYSCON->MAINCLKUEN.ENA = 1;

		SYSCON->MAINCLKPLLSEL.SEL = MAINCLKPLLSEL_PRE_PLL;
		SYSCON->MAINCLKPLLUEN.ENA = 0;
		SYSCON->MAINCLKPLLUEN.ENA = 1;
	}
	else
	{
		current_system_clock = &current_pll_clock;

		SYSCON->MAINCLKPLLSEL.SEL = MAINCLKPLLSEL_SYS_PLL;
		SYSCON->MAINCLKPLLUEN.ENA = 0;
		SYSCON->MAINCLKPLLUEN.ENA = 1;
	}
}

/**
 * @brief Inicializacion del clock con cristal externo
 * @param[in] crystal_frequency Frecuencia del cristal externo utilizado en Hz
 */
void SYSCON_set_crystal_clock(uint32_t crystal_frequency)
{
	uint32_t counter_delay;
	const IOCON_config_t xtal_pin_config =
	{
		.mode = IOCON_PULL_NONE,
		.clk_sel = IOCON_CLK_DIV_0,
		.dac_mode = 0,
		.hysteresis = 0,
		.iic_mode = 0,
		.invert_input = 0,
		.open_drain = 0,
		.sample_mode = IOCON_SAMPLE_MODE_BYPASS
	};

	IOCON_init();
	IOCON_config_io(XTALIN_PORT, XTALIN_PIN, &xtal_pin_config);
	IOCON_config_io(XTALOUT_PORT, XTALOUT_PIN, &xtal_pin_config);
	IOCON_deinit();

	// Habilitacion de los pines XTALIN y XTALOUT
	SWM_init();
	SWM->PINENABLE0.XTALIN = PINENABLE_XTALIN_ON;
	SWM->PINENABLE0.XTALOUT = PINENABLE_XTALOUT_ON;
	SWM_deinit();

	SYSCON->SYSOSCCTRL.BYPASS = 0;

	// Seleccion de rango de frecuencia
	if(crystal_frequency < 20e6)
	{
		SYSCON->SYSOSCCTRL.FREQRANGE = SYSOSCCTRL_FREQRANGE_MINUS_20MHZ;
	}
	else
	{
		SYSCON->SYSOSCCTRL.FREQRANGE = SYSOSCCTRL_FREQRANGE_PLUS_20MHZ;
	}

	// Encendido del sistema de oscilacion a cristal
	SYSCON->PDRUNCFG.SYSOSC_PD = PDRUNCFG_PD_POWER_UP;

	// Retraso de aproximadamente 1000useg (la hoja de datos dice 500useg, pero para estar seguros)
	counter_delay = ((*current_system_clock)/(1e3)) + 1;

	// El "+1" en la cuenta superior, esta para que si o si por lo menos haya un ciclo de clock de delay

	while(--counter_delay);

	// Se asume que el valor pasado es correcto, no hay forma de chequearlo
	current_crystal_clock = crystal_frequency;
}

/**
 * @brief Inicializacion del clock externo (EXT CLK IN)
 * @param[in] ext_clk_frequency Frecuencia de la fuente externa utilizada, en Hz
 */
void SYSCON_set_ext_in_clock(uint32_t ext_clk_frequency)
{
	current_ext_in_clock = ext_clk_frequency;
}

/**
 * @brief Configuracion del FRO para que sea el oscilador directo (sin division)
 */
void SYSCON_set_fro_direct(void)
{
	SYSCON->FROOSCCTRL.FRO_DIRECT = 1;
	SYSCON->FRODIRECTCLKUEN.ENA = 0;
	SYSCON->FRODIRECTCLKUEN.ENA = 1;

	#warning Ojo, en realidad esto tendria que cambiar dependiendo de las configuraciones FAIM
	current_fro_clock = DEFAULT_SYSTEM_CLOCK * 2;
}

/**
 * @brief Configuracion del FRO para que sea el oscilador dividido (con division dependiente de FAIM)
 */
void SYSCON_clear_fro_direct(void)
{
	SYSCON->FROOSCCTRL.FRO_DIRECT = 0;
	SYSCON->FRODIRECTCLKUEN.ENA = 0;
	SYSCON->FRODIRECTCLKUEN.ENA = 1;

	#warning Ojo, en realidad esto tendria que cambiar dependiendo de las configuraciones FAIM
	current_fro_clock = DEFAULT_SYSTEM_CLOCK;
}

/**
 * @brief Inicializacion del clock watchdog oscillator
 * @param[in] clkana_selection Seleccion de la frecuencia base
 * @param[in] divider Divisor deseado (se redondeara al multiplo de 2 mas cercano)
 */
void SYSCON_set_watchdog_osc_clock(SYSCON_watchdog_clkana_sel_en clkana_selection, uint8_t divider)
{
	uint8_t divsel_calculated = (divider / 2) - 1;

	SYSCON->WDTOSCCTRL.FREQSEL = clkana_selection;
	SYSCON->WDTOSCCTRL.DIVSEL = divsel_calculated;

	current_watchdog_clock = watchdog_ana_clock[clkana_selection] / (2 * (1 + divsel_calculated));
}

/**
 * @brief Inicializacion del PLL
 * @param[in] pll_source Fuente de entrada para el PLL
 * @param[in] pll_multiplier Multiplicador de frecuencia del PLL
 */
void SYSCON_set_PLL(SYSCON_pll_source_sel_en pll_source, uint8_t pll_multiplier)
{
	uint32_t pll_in_freq, p;

	// Identificacion de la frecuencia de entrada del PLL
	if(pll_source == SYSCON_PLL_SOURCE_SEL_FRO)
	{
		pll_in_freq = current_fro_clock;
	}
	else
	{
		if(SYSCON->EXTCLKSEL.SEL == EXTCLKSEL_CLK_IN)
		{
			pll_in_freq = current_ext_in_clock;
		}
		else
		{
			pll_in_freq = current_crystal_clock;
		}
	}

	// Seleccion de fuente de entrada para el PLL
	SYSCON->SYSPLLCLKSEL.SEL = pll_source;

	// Actualizacion de seleccion de fuente de entrada para el PLL
	SYSCON->SYSPLLCLKUEN.ENA = 1;

	// Apagado del PLL
	SYSCON->PDRUNCFG.SYSPLL_PD = PDRUNCFG_PD_POWER_DOWN;

	// Configuracion de M y P
	SYSCON->SYSPLLCTRL.MSEL = pll_multiplier - 1;

	p = 0;

	while((((2 * (p + 1)) + 1) * (pll_in_freq * pll_multiplier)) < 156e6)
	{
		p++;
	}

	if(p <= 3)
	{
		SYSCON->SYSPLLCTRL.PSEL = p;

		// Encendio del PLL
		SYSCON->PDRUNCFG.SYSPLL_PD = PDRUNCFG_PD_POWER_UP;

		// Espera a que el PLL se lockee
		while(SYSCON->SYSPLLSTAT.LOCK == SYSPLLSTAT_PLL_NOT_LOCKED);

		current_pll_clock = pll_in_freq * pll_multiplier;
	}
}


/**
 * @brief Obtencion del clock de los distintos perifericos
 * @param[in] peripheral Periferico cuya fuente seleccionar
 * @return Frecuencia del clock del periferico, en Hz
 */
uint32_t SYSCON_get_peripheral_clock(SYSCON_peripheral_sel_en peripheral)
{
	switch(SYSCON->PERCLKSEL[peripheral].SEL)
	{
	case SYSCON_PERIPHERAL_CLOCK_SEL_MAIN:
	{
		return *current_system_clock;

		break;
	}

	case SYSCON_PERIPHERAL_CLOCK_SEL_FRO:
	{
		return current_fro_clock;

		break;
	}

	case SYSCON_PERIPHERAL_CLOCK_SEL_FRO_DIV:
	{
		return current_fro_div_clock;

		break;
	}

	case SYSCON_PERIPHERAL_CLOCK_SEL_NONE:
	{
		return 0;

		break;
	}

	case SYSCON_PERIPHERAL_CLOCK_SEL_FRG0:
	{
		return current_frg0_clock;

		break;
	}

	case SYSCON_PERIPHERAL_CLOCK_SEL_FRG1:
	{
		return current_frg1_clock;

		break;
	}
	}

	return 0;
}

/**
 * @brief Configuracion del FRG
 * @param[in] frg_selection Cual de los FRG configurar, cero o uno
 * @param[in] clock_source Fuente de clock del FRG
 * @param[in] mul Multiplicador del FRG 0~255
 * @param[in] div Divisor del FRG 1~256
 */
void SYSCON_set_frg_config(uint8_t frg_selection, SYSCON_frg_clock_sel_en clock_source, uint8_t mul, uint16_t div)
{
	uint32_t frg_clock_source_freq;

	switch(clock_source)
	{
	case SYSCON_FRG_CLOCK_SEL_FRO:
	{
		frg_clock_source_freq = current_fro_clock;

		break;
	}

	case SYSCON_FRG_CLOCK_SEL_MAIN_CLOCK:
	{
		frg_clock_source_freq = *current_system_clock;

		break;
	}

	case SYSCON_FRG_CLOCK_SEL_SYS_PLL:
	{
		frg_clock_source_freq = current_pll_clock;

		break;
	}

	case SYSCON_FRG_CLOCK_SEL_NONE:
	{
		frg_clock_source_freq = 0;

		break;
	}

	default: break;
	}

	if(frg_selection == 0)
	{
		SYSCON->FRG0CLKSEL.SEL = clock_source;
		SYSCON->FRG0DIV.DIV = div - 1;
		SYSCON->FRG0MUL.MULT = mul;

		current_frg0_clock = frg_clock_source_freq /(double)(1.0 + ((double)(mul)/(double)(div)));
	}
	else if(frg_selection == 1)
	{
		SYSCON->FRG1CLKSEL.SEL = clock_source;
		SYSCON->FRG1DIV.DIV = div - 1;
		SYSCON->FRG1MUL.MULT = mul;

		current_frg1_clock = frg_clock_source_freq /(double)(1.0 + ((double)(mul)/(double)(div)));
	}
}
