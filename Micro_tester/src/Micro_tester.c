/*
 * Descripcion del programa de testeo:
 *
 * Unicamente se utilizan las librerias de la capa de aplicacion (excepto para el CTIMER)
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
#include <stddef.h>

#include <HAL_ADC.h>
#include <HAL_SYSCON.h>
#include <HAL_SYSTICK.h>
#include <HAL_UART.h>
#include <HAL_GPIO.h>
#include <HAL_CTIMER.h>

#define		CTIMER_IN_PWM_MODE

#define		LED_PORT			HAL_GPIO_PORT_1
#define		LED_PORT_PIN		HAL_GPIO_PORTPIN_1_2

#define		TICK_PERIOD_US		1000
#define		CONVERSION_TIME_MS	50

#define		ADC_CHANNEL			0
#define		ADC_FREQUENCY		500e3

#define		ADC_SEQUENCE		HAL_ADC_SEQUENCE_SEL_A

#define		RX_PORT				0
#define		RX_PIN				8

#define		TX_PORT				0
#define		TX_PIN				9

#define		UART_NUMBER			0
#define		UART_BAUDRATE		115200

#define		CLOCKOUT_DIVIDER	10
#define		CLOCKOUT_PORT		0
#define		CLOCKOUT_PIN		18

#define		CTIMER_PORT			HAL_GPIO_PORT_0
#define		CTIMER_PORT_PIN		HAL_GPIO_PORTPIN_0_16

static void tick_callback(void);

static void adc_callback(void);

static void rx_callback(void);

static void tx_callback(void);

static void match_callback(void);

static const hal_adc_sequence_config_t adc_config =
{
	.channels = (1 << ADC_CHANNEL),
	.trigger = HAL_ADC_TRIGGER_SEL_NONE,
	.trigger_pol = HAL_ADC_TRIGGER_POL_SEL_NEGATIVE_EDGE,
	.sync_bypass = HAL_ADC_SYNC_SEL_BYPASS_SYNC,
	.mode = HAL_ADC_INTERRUPT_MODE_EOS,
	.burst = 0,
	.single_step = 1,
	.low_priority = 0,
	.callback = adc_callback
};

static const hal_uart_config_t uart_config =
{
	.data_length = HAL_UART_DATALEN_8BIT,
	.parity = HAL_UART_PARITY_NO_PARITY,
	.stop_bits = HAL_UART_STOPLEN_1BIT,
	.oversampling = HAL_UART_OVERSAMPLING_X16,
	.clock_selection = HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRG0,
	.baudrate = UART_BAUDRATE,
	.tx_port = TX_PORT,
	.tx_pin = TX_PIN,
	.rx_port = RX_PORT,
	.rx_pin = RX_PIN,
	.rx_ready_callback = rx_callback,
	.tx_ready_callback = tx_callback
};

#ifndef 	CTIMER_IN_PWM_MODE
static const hal_ctimer_match_config_t match_config =
{
	.interrupt_on_match = 0,
	.reset_on_match = 1,
	.stop_on_match = 0,
	.match_value_useg = 2500,
	.match_action = HAL_CTIMER_MATCH_TOGGLE_PIN,
	.enable_external_pin = 1,
	.match_pin = CTIMER_PORT_PIN,
	.callback = match_callback
};

static const hal_ctimer_timer_config_t ctimer_config =
{
	.clock_div = 0,
	.match_config[0] = &match_config,
	.match_config[1] = NULL,
	.match_config[2] = NULL,
	.match_config[3] = NULL
};

#else

static const hal_ctimer_pwm_channel_config_t pwm_channel_config =
{
	.interrupt_on_action = 0,
	.duty = 1000,
	.channel_pin = CTIMER_PORT_PIN
};

static const hal_ctimer_pwm_config_t pwm_config =
{
	.clock_div = 0,
	.pwm_period_useg = 1000,
	.interrupt_on_period = 0,
	.channels[0] = &pwm_channel_config,
	.channels[1] = NULL,
	.channels[2] = NULL,
};

#endif

static uint32_t blink_time_ms = 0;
static uint32_t adc_conversion = 0;

int main(void)
{
	hal_syscon_config_fro_direct(1, 1);

	// Clock principal en un pin (utilizando un divisor)
	hal_syscon_config_clkout(CLOCKOUT_PORT, CLOCKOUT_PIN, HAL_SYSCON_CLKOUT_SOURCE_SEL_MAIN_CLOCK, CLOCKOUT_DIVIDER);

	// Hasta aca queda el clock configurado con el FRO interno en 24MHz
	// Configuro el fraccional para poder tener buena presicion para un baudrate de 115200bps
	// El DIV siempre debe estar en 256 (especificacion del manual de usuario)
	// Como fuente utilizo el FRO a 24MHz ya configurado
	// 24MHz / (1 + (47 / 256)) = 20.2772272MHz
	hal_syscon_config_frg(0, HAL_SYSCON_FRG_CLOCK_SEL_MAIN_CLOCK, 47);

	hal_gpio_init(LED_PORT);
	hal_gpio_init(CTIMER_PORT);

	hal_gpio_set_dir(LED_PORT_PIN, HAL_GPIO_DIR_OUTPUT, 0);
	hal_gpio_set_dir(CTIMER_PORT_PIN, HAL_GPIO_DIR_OUTPUT, 1);

	hal_adc_init(ADC_FREQUENCY);
	hal_adc_config_sequence(ADC_SEQUENCE, &adc_config);

	hal_uart_init(UART_NUMBER, &uart_config);

#ifndef	CTIMER_IN_PWM_MODE
	hal_ctimer_timer_mode_init(&ctimer_config);

	hal_ctimer_timer_mode_run();
#else
	hal_ctimer_pwm_mode_init(&pwm_config);

	hal_ctimer_pwm_mode_run();
#endif

	hal_systick_init(TICK_PERIOD_US, tick_callback);
	hal_adc_enable_sequence(ADC_SEQUENCE);

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
		hal_gpio_toggle_pin(LED_PORT_PIN);
	}

	if(adc_counter == 0)
	{
		hal_adc_start_sequence(ADC_SEQUENCE);
	}
}

static void adc_callback(void)
{
	hal_adc_sequence_result_t adc_result;
	hal_adc_sequence_result_t *adc_result_p = &adc_result;

	if(hal_adc_get_sequence_result(ADC_SEQUENCE, &adc_result_p) == HAL_ADC_SEQUENCE_RESULT_VALID)
	{
		adc_conversion = adc_result.result;
	}

	adc_conversion /= 4; // 0 ~ 1023

	blink_time_ms = adc_conversion; // 0mseg ~ 1023mseg

	//CTIMER_update_mr_value_on_finish(0, 1e6 - ((adc_conversion * 1e6) / 1023));
}

static char trama[] = "Trama de prueba para ver que onda\n";
static uint32_t trama_counter = 0;

static void rx_callback(void)
{
	uint32_t data;

	hal_uart_rx_byte(UART_NUMBER, &data);

	if((char) data == ' ')
	{
		hal_uart_tx_byte(UART_NUMBER, trama[trama_counter++]);
	}
}

static void tx_callback(void)
{
	if(trama[trama_counter] != '\0')
	{
		hal_uart_tx_byte(UART_NUMBER, trama[trama_counter++]);
	}
	else
	{
		trama_counter = 0;
	}
}

static void match_callback(void)
{
	//hal_gpio_toggle_pin(CTIMER_PORT_PIN);
}
