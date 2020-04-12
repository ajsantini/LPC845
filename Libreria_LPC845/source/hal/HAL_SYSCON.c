/**
 * @file HAL_SYSCON.c
 * @brief Funciones a nivel de aplicacion para el SYSCON (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_SYSCON.h>
#include <HPL_SYSCON.h>
#include <HPL_IOCON.h>
#include <HPL_SWM.h>

/** Número de puerto del XTALIN */
#define		XTALIN_PORT			0

/** Número de pin del XTALIN */
#define		XTALIN_PIN			8

/** Número de puerto del XTALOUT */
#define		XTALOUT_PORT		0

/** Número de pin del XTALOUT */
#define		XTALOUT_PIN			9

/** Frecuencia del FRO base */
#define		FRO_DIRECT_FREQ		24e6

static uint32_t current_main_freq = FRO_DIRECT_FREQ / 2; //!< Frecuencia actual del main clock
static uint8_t current_main_div = 1; //!< Divisor actual del clock principal
static uint32_t current_fro_freq = FRO_DIRECT_FREQ / 2; //!< Frecuencia actual del FRO
static uint32_t current_crystal_freq = 0; //!< Frecuencia del cristal configurada
static uint32_t current_frg_freq[2] = { 0, 0 }; //!< Frecuencia de los FRG
static uint32_t current_pll_freq = 0; //!< Frecuencia del PLL

/**
 * @brief Obtener la frecuencia actual del main clock
 * @return Frecuencia del main clock en Hz
 */
uint32_t hal_syscon_system_clock_get(void)
{
	return current_main_freq / current_main_div;
}

/*
 * @brief Fijar el divisor del clock principal
 * @param[in] div Divisor deseado. Cero inhabilita el clock principal
 */
void hal_syscon_system_clock_set_divider(uint8_t div)
{
	current_main_div = div;
	SYSCON_set_system_clock_divider(div);
}

/**
 * @brief Obtener la frecuencia actual del FRO
 * @return Frecuencia del FRO en Hz
 */
uint32_t hal_syscon_fro_clock_get(void)
{
	return current_fro_freq;
}

/**
 * @brief Configurar el ext clock a partir de un cristal externo
 * @param[in] crystal_freq Frecuencia del cristal externo utilizado
 * @param[in] use_as_main Si es distinto de cero, se utilizara el oscilador a cristal como main clock
 */
void hal_syscon_external_crystal_config(uint32_t crystal_freq, uint8_t use_as_main)
{
	uint8_t counter;

	// Remocion de pull ups en los pines XTAL
	IOCON_init();
	IOCON_config_pull_mode(XTALIN_PORT, XTALIN_PIN, IOCON_PULL_NONE);
	IOCON_config_pull_mode(XTALOUT_PORT, XTALOUT_PIN, IOCON_PULL_NONE);

	// Habilitacion de los pines XTAL
	SWM_init();
	SWM_enable_XTALIN(SWM_ENABLE);
	SWM_enable_XTALOUT(SWM_ENABLE);
	SWM_deinit();

	if(crystal_freq < 20e6)
	{
		SYSCON_set_oscillator_control(SYSCON_BYPASS_DISABLED, SYSCON_FREQRANGE_MINUS_20MHZ);
	}
	else
	{
		SYSCON_set_oscillator_control(SYSCON_BYPASS_DISABLED, SYSCON_FREQRANGE_PLUS_20MHZ);
	}

	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_SYSOSC);

	counter = crystal_freq / 100; // Delay de aprox 1mseg a lo guaso
	while(counter) counter--; // Estabilizacion del cristal

	SYSCON_set_ext_clock_source(SYSCON_EXT_CLOCK_SOURCE_SEL_CRYSTAL);
	current_crystal_freq = crystal_freq;

	if(use_as_main)
	{
		SYSCON_set_system_clock_source(SYSCON_MAIN_CLOCK_SEL_EXT_CLK);
		current_main_freq = current_crystal_freq;
	}
}

/**
 * @brief Configurar el clock FRO
 * @param[in] direct Si es distinto de cero se omite el divisor del FRO
 * @param[in] use_as_main Si es distinto de cero, se utilizará el FRO como main clock
 */
void hal_syscon_fro_clock_config(uint8_t direct, uint8_t use_as_main)
{
	if(direct)
	{
		SYSCON_set_fro_direct();
		current_fro_freq = (uint32_t) (FRO_DIRECT_FREQ);
	}
	else
	{
		SYSCON_clear_fro_direct();
		current_fro_freq = (uint32_t) (FRO_DIRECT_FREQ / 2);
	}

	if(use_as_main)
	{
		SYSCON_set_system_clock_source(SYSCON_MAIN_CLOCK_SEL_FRO);
		current_main_freq = current_fro_freq;
	}
}

/**
 * @brief Configurar el pin de clock out (salida de clock hacia afuera)
 * @param[in] portpin Número de puerto/pin por donde sacar el clock out
 * @param[in] clock_source Fuente deseada para la salida clock out
 * @param[in] divider Divisor deseado para la salida clock out
 */
void hal_syscon_clkout_config(hal_gpio_portpin_en portpin, hal_syscon_clkout_source_sel_en clock_source, uint8_t divider)
{
	SYSCON_set_clkout_config(clock_source, divider);

	SWM_init();
	SWM_assign_CLKOUT(portpin);
	SWM_deinit();
}

/**
 * @brief Configurar el divisor fraccional
 *
 * El divisor siempre se debe fijar en 256 para estos MCU.
 *
 * @param[in] inst Instancia de FRG a configurar
 * @param[in] clock_source Fuente de clock de entrada para el FRG
 * @param[in] mul Multiplicador deseado
 */
void hal_syscon_frg_config(uint8_t inst, hal_syscon_frg_clock_sel_en clock_source, uint32_t mul)
{
	uint32_t aux_freq;

	SYSCON_set_frg_config(inst, clock_source, mul, 0xFF);

	switch(clock_source)
	{
	case HAL_SYSCON_FRG_CLOCK_SEL_FRO: { aux_freq = current_fro_freq; break; }
	case HAL_SYSCON_FRG_CLOCK_SEL_MAIN_CLOCK: { aux_freq = current_main_freq; break; }
	case HAL_SYSCON_FRG_CLOCK_SEL_NONE: { aux_freq = 0; break; }
	case HAL_SYSCON_FRG_CLOCK_SEL_SYS_PLL: { aux_freq = current_pll_freq; break; }
	}

	current_frg_freq[inst] = (uint32_t) (aux_freq / (1 + ((float) mul / 256)));
}

/**
 * @brief Obtener la frecuencia de clock en Hz configurada para cierto periférico
 * @param[in] peripheral Periférico deseado
 * @return Frecuencia en Hz del clock del periférico
 */
uint32_t hal_syscon_peripheral_clock_get(hal_syscon_peripheral_sel_en peripheral)
{
	uint32_t ret;

	switch(SYSCON->PERCLKSEL[peripheral].SEL)
	{
	case HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRG0: { ret = current_frg_freq[0]; break; }
	case HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRG1: { ret = current_frg_freq[1]; break; }
	case HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRO: { ret = current_fro_freq; break; }
	case HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRO_DIV: { ret = current_fro_freq / 2; break; }
	case HAL_SYSCON_PERIPHERAL_CLOCK_SEL_MAIN: { ret = current_main_freq; break; }
	case HAL_SYSCON_PERIPHERAL_CLOCK_SEL_NONE: { ret = 0; break; }
	}

	return ret;
}

/**
 * @brief Configurar divisor para el clock de glitches del IOCON
 * @param[in] sel Selección de divisor
 * @param[in] div Valor de división deseado
 */
void hal_syscon_iocon_glitch_divider_set(hal_syscon_iocon_glitch_sel_en sel, uint32_t div)
{
	SYSCON_set_iocon_glitch_divider(sel, div);
}

/**
 * @brief Configurar el PLL
 * @param[in] clock_source Fuente de clock de referencia para el PLL
 * @param[in] freq Frecuencia deseada de salida del PLL
 */
void hal_syscon_pll_clock_config(hal_syscon_pll_source_sel_en clock_source, uint32_t freq)
{
	#warning Ojo, falta hacer esta funcion
	current_pll_freq = 0; // Tiene que dejar el valor correcto de la frecuencia del PLL, en Hz
}

/**
 * @brief Obtener frecuencia actual configurada del PLL
 * @return Frecuencia actual del PLL en Hz
 */
uint32_t hal_syscon_pll_clock_get(void)
{
	return current_pll_freq;
}
