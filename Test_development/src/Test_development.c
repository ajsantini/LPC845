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

	const IOCON_config_t xtal_pin_config =
	{
		.pull_mode = IOCON_PULL_NONE,
		.hysteresis = 0,
		.invert_input = 0,
		.open_drain = 0,
		.sample_mode = IOCON_SAMPLE_MODE_BYPASS,
		.clk_sel = IOCON_CLK_DIV_0,
		.dac_mode = 0,
		.iic_mode = IOCON_IIC_MODE_GPIO
	};

	// Remocion de pull ups en los pines XTAL
	IOCON_init();
	IOCON_config_io(XTALIN_PORT, XTALIN_PIN, &xtal_pin_config);
	IOCON_config_io(XTALOUT_PORT, XTALOUT_PIN, &xtal_pin_config);

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
