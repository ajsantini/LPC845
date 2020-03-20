/*
 * Descripcion del programa de testeo:
 *
 * En primer lugar se configura el sistema de clock con las siguientes caracteristicas:
 * -) Configuracion de cristal externo (utilizar un cristal de 12MHz)
 * -) Se configura el PLL para incrementar la frecuencia a 24MHz y se lo selecciona como clock de sistema
 *
 * Luego se configura en un pin, la salida del clock del sistema post division.
 * Esto es util para corroborar la presicion y los valores del clock del sistema.
 *
 * Luego se configura el clock con divisor fraccional, para lograr mejor presicion para
 * el periferico UART. Las cuentas se realizaron externamente para lograr una buena presicion
 * con un baudrate de 115200bps.
 *
 * Se configura el periferico GPIO para generar un blink de periodo variable (duty 50%) por software
 * utilizando como base de tiempo el SYSTICK, y como manejador del periodo el ADC, con el preset
 * propio del stick.
 *
 * Tambien se configura el periferico CTIMER para controlar un PWM sin intervencion de software, cuyo
 * ajuste de duty tambien se controla mediante el mismo ADC mencionado anteriormente.
 */

#include <cr_section_macros.h>
#include <stdio.h>

#include <HAL_ADC.h>
#include <HAL_SYSTICK.h>

#include <HPL_GPIO.h>
#include <HPL_ADC.h>
#include <HPL_SYSCON.h>
#include <HPL_SWM.h>
#include <HRI_SWM.h>
#include <HPL_SYSTICK.h>
#include <HPL_UART.h>
#include <HPL_CTIMER.h>

#define		LED_PORT			0
#define		LED_PIN				1

#define		TICK_PERIOD_US		1000
#define		CONVERSION_TIME_MS	50

#define		ADC_CHANNEL			0
#define		ADC_FREQUENCY		500e3

#define		RX_PORT				0
#define		RX_PIN				17

#define		TX_PORT				0
#define		TX_PIN				16

#define		UART_NUMBER			0
#define		UART_BAUDRATE		115200

#define		CLOCKOUT_DIVIDER	10
#define		CLOCKOUT_PORT		0
#define		CLOCKOUT_PIN		18

#define		CTIMER_PORT			0
#define		CTIMER_PIN			19

#define		CTIMER_PRESCALER	23

static void tick_callback(void);

static void adc_callback(void);

static void rx_callback(void);

static const hal_adc_sequence_config_t adc_config =
{
	.channels = (1 << ADC_CHANNEL),
	.trigger = ADC_TRIGGER_SEL_NONE,
	.trigger_pol = ADC_TRIGGER_POL_SEL_NEGATIVE_EDGE,
	.sync_bypass = ADC_SYNC_SEL_BYPASS_SYNC,
	.burst = 0,
	.single_step = 0,
	.low_priority = 0,
	.callback = adc_callback
};

static const UART_config_t uart_config =
{
	.data_length = UART_DATALEN_8BIT,
	.parity = UART_PARITY_NO_PARITY,
	.stop_bits = UART_STOPLEN_1BIT,
	.oversampling = UART_OVERSAMPLING_X16,
	.clock_selection = SYSCON_PERIPHERAL_CLOCK_SEL_FRG0,
	.baudrate = UART_BAUDRATE,
	.tx_port = TX_PORT,
	.tx_pin = TX_PIN,
	.rx_port = RX_PORT,
	.rx_pin = RX_PIN,
	.rx_ready_callback = rx_callback,
	.tx_ready_callback = NULL
};

static const CTIMER_CTCR_config_t ctcr_config =
{
	.CTMODE = CTIMER_CTMODE_CONFIG_TIMER_MODE,
	.ENCC = 0
};

static const CTIMER_MR_config_t mr_config_0 =
{
	.interrupt_on_match = 0, .reset_on_match = 0, .stop_on_match = 0,
	.reload_on_match = 0, .match_callback = NULL, .match_value = 0
};

static const CTIMER_MR_config_t mr_config_1 =
{
	.interrupt_on_match = 0, .reset_on_match = 1, .stop_on_match = 0,
	.reload_on_match = 0, .match_callback = NULL, .match_value = 1e6
};

static const CTIMER_EMR_config_t emr_config =
{
	.EMC = CTIMER_EMC_CONFIG_DO_NOTHING,
	.mat_enable = 1,
	.mat_port = CTIMER_PORT,
	.mat_pin = CTIMER_PIN
};

static uint32_t blink_time_ms = 0;
static uint32_t adc_conversion = 0;

int main(void)
{
	SYSCON_set_fro_direct(); // FRO sin divisor previo (FRO = 24MHz)
	SYSCON_set_system_clock_source(SYSCON_MAIN_CLOCK_SEL_FRO);

	// Clock principal en un pin (utilizando un divisor)
	SYSCON_set_clkout_config(SYSCON_CLKOUT_SOURCE_SEL_MAIN_CLOCK, CLOCKOUT_DIVIDER);
	SWM_init();
	SWM_assign_CLKOUT(CLOCKOUT_PORT, CLOCKOUT_PIN);
	SWM_deinit();

	// Hasta aca queda el clock configurado con el FRO interno en 24MHz
	// Configuro el fraccional para poder tener buena presicion para un baudrate de 115200bps
	// El DIV siempre debe estar en 256 (especificacion del manual de usuario)
	// Como fuente utilizo el PLL a 24MHz ya configurado
	// 24MHz / (1 + (29 / 256)) = 21.55789474Hz
	SYSCON_set_frg_config(0, SYSCON_FRG_CLOCK_SEL_MAIN_CLOCK, 29, 255);

	GPIO_init(LED_PORT);

	GPIO_set_dir(LED_PORT, LED_PIN, GPIO_DIR_OUTPUT, 0);

	hal_adc_init(ADC_FREQUENCY);
	hal_adc_config_sequence(ADC_SEQUENCE_SEL_A, &adc_config);

	UART_init(UART_NUMBER, &uart_config);

	CTIMER_init(CTIMER_PRESCALER);

	CTIMER_config_ctcr(&ctcr_config);

	CTIMER_config_mr(0, &mr_config_0);
	CTIMER_config_mr(3, &mr_config_1);

	CTIMER_config_emr(0, &emr_config);

	CTIMER_enable_pwm(0);

	CTIMER_run();

	hal_systick_init(TICK_PERIOD_US, tick_callback);

	while(1)
	{

	}

	return 0;
}

static void tick_callback(void)
{
	static uint32_t tick_counter = 0;
	static uint32_t adc_counter = 0;

	tick_counter = (tick_counter + 1) % blink_time_ms;
	adc_counter = (adc_counter + 1) % CONVERSION_TIME_MS;

	if(tick_counter == 0)
	{
		GPIO_toggle_pin(LED_PORT, LED_PIN);
	}

	if(adc_counter == 0)
	{
		hal_adc_start_sequence(ADC_SEQUENCE_SEL_A);
	}
}

static void adc_callback(void)
{
	hal_adc_sequence_result_t adc_result;
	hal_adc_sequence_result_t *adc_result_p = &adc_result;

	if(hal_adc_get_sequence_result(ADC_SEQUENCE_SEL_A, &adc_result_p) == HAL_ADC_SEQUENCE_RESULT_VALID)
	{
		adc_conversion = adc_result.result;
	}

	adc_conversion /= 4; // 0 ~ 1023

	blink_time_ms = adc_conversion; // 0mseg ~ 1023mseg

	CTIMER_update_mr_value_on_finish(0, 1e6 - ((adc_conversion * 1e6) / 1023));
}

static void rx_callback(void)
{
	uint32_t data;

	UART_rx_byte(UART_NUMBER, &data);

	if((char) data == ' ')
	{
		UART_tx_byte(UART_NUMBER, 'a');
	}
}
