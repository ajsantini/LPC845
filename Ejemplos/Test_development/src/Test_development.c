/**
 * Ejemplo utilizando nivel HPL (sin usar la capa de aplicacion)
 */

#include <HPL_SYSCON.h>
#include <HPL_SWM.h>
#include <HPL_IOCON.h>

#define		CRYSTAL_CLOCK		12e6

#define		XTALIN_PORT			0
#define		XTALIN_PIN			8

#define		XTALOUT_PORT		0
#define		XTALOUT_PIN			9

int main(void)
{
	uint32_t counter;

	// Remocion de pull ups en los pines XTAL
	IOCON_init();
	IOCON_config_pull_mode(XTALIN_PORT, XTALIN_PIN, IOCON_PULL_NONE);
	IOCON_config_pull_mode(XTALOUT_PORT, XTALOUT_PIN, IOCON_PULL_NONE);

	// Habilitacion de los pines XTAL
	SWM_init();
	SWM_enable_XTALIN(SWM_ENABLE);
	SWM_enable_XTALOUT(SWM_ENABLE);
	SWM_deinit();

	SYSCON_set_oscillator_control(SYSCON_BYPASS_DISABLED, SYSCON_FREQRANGE_MINUS_20MHZ);
	SYSCON_power_up_peripheral(SYSCON_POWER_SEL_SYSOSC);

	counter = 12e3; // Delay de aprox 1mseg a lo guaso
	while(counter) counter--; // Estabilizacion del cristal



	while(1)
	{

	}

	return 0;
}
