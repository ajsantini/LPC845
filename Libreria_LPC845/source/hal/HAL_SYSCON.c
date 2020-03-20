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

#define		XTALIN_PORT			0
#define		XTALIN_PIN			8

#define		XTALOUT_PORT		0
#define		XTALOUT_PIN			9

#define		FRO_DIRECT_FREQ		24e6

static uint32_t current_main_clock = FRO_DIRECT_FREQ / 2; //!< Frecuencia actual del main clock
static uint32_t current_fro_clock = FRO_DIRECT_FREQ / 2; //!< Frecuencia actual del FRO
static uint32_t current_crystal_freq = 0; //!< Frecuencia del cristal configurada

/**
 * @brief Obtener la frecuencia actual del main clock
 * @return Frecuencia del main clock en Hz
 */
uint32_t hal_syscon_get_system_clock(void)
{
	return current_main_clock;
}

/**
 * @brief Obtener la frecuencia actual del FRO
 * @return Frecuencia del FRO en Hz
 */
uint32_t hal_syscon_get_fro_clock(void)
{
	return current_fro_clock;
}

/**
 * @brief Configurar el ext clock a partir de un cristal externo
 * @param[in] crystal_freq Frecuencia del cristal externo utilizado
 * @param[in] use_as_main Si es distinto de cero, se utilizara el oscilador a cristal como main clock
 */
void hal_syscon_config_external_crystal(uint32_t crystal_freq, uint8_t use_as_main)
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
		current_main_clock = current_crystal_freq;
	}
}

/**
 * @brief Configurar el clock FRO
 * @param[in] direct Si es distinto de cero se omite el divisor del FRO
 * @param[in] use_as_main Si es distinto de cero, se utilizara el FRO como main clock
 */
void hal_syscon_config_fro_direct(uint8_t direct, uint8_t use_as_main)
{
	if(direct)
	{
		SYSCON_set_fro_direct();
		current_fro_clock = (uint32_t) (FRO_DIRECT_FREQ);
	}
	else
	{
		SYSCON_clear_fro_direct();
		current_fro_clock = (uint32_t) (FRO_DIRECT_FREQ / 2);
	}

	if(use_as_main)
	{
		SYSCON_set_system_clock_source(SYSCON_MAIN_CLOCK_SEL_FRO);
	}
}

/**
 * @brief Configurar el pin de clock out (salida de clock hacia afuera)
 * @param[in] port Numero de puerto por donde sacar el clock out
 * @param[in] pin Numero de pin por donde sacar el clock out
 * @param[in] clock_source Fuente deseada para la salida clock out
 * @param[in] divider Divisor deseado para la salida clock out
 */
void hal_syscon_config_clkout(uint8_t port, uint8_t pin, hal_syscon_clkout_source_sel_en clock_source, uint8_t divider)
{
	SYSCON_set_clkout_config(clock_source, divider);

	SWM_init();
	SWM_assign_CLKOUT(port, pin);
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
void hal_syscon_config_frg(uint8_t inst, hal_syscon_frg_clock_sel_en clock_source, uint32_t mul)
{
	SYSCON_set_frg_config(inst, clock_source, mul, 0xFF);
}
