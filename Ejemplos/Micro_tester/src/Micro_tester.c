/**
 * TODO: Descripcion del programa de testeo
 */

#include <cr_section_macros.h>
#include <stddef.h>

#include <HAL_ADC.h>
#include <HAL_SYSCON.h>
#include <HAL_SYSTICK.h>
#include <HAL_USART.h>
#include <HAL_GPIO.h>
#include <HAL_CTIMER.h>
#include <HAL_PININT.h>
#include <HAL_IOCON.h>
#include <HAL_SPI.h>
#include <HAL_WKT.h>

#define		CTIMER_IN_PWM_MODE

#define		LED_PORT			HAL_GPIO_PORT_1
#define		LED_PORT_PIN		HAL_GPIO_PORTPIN_1_2

#define		TICK_PERIOD_US		1000
#define		CONVERSION_TIME_MS	50
#define		NRF_TIME_MS			1000

#define		ADC_CHANNEL			0
#define		ADC_FREQUENCY		500e3

#define		ADC_SEQUENCE		HAL_ADC_SEQUENCE_SEL_A

#define		UART_RX_PORTPIN		HAL_GPIO_PORTPIN_0_8
#define		UART_TX_PORTPIN		HAL_GPIO_PORTPIN_0_9

#define		CLOCKOUT_PORT_PIN	HAL_GPIO_PORTPIN_0_18
#define		CLOCKOUT_DIVIDER	10

#define		UART_NUMBER			0
#define		UART_BAUDRATE		115200

#define		CTIMER_PORT			HAL_GPIO_PORT_0
#define		CTIMER_PORT_PIN		HAL_GPIO_PORTPIN_0_16

#define		KEY_PORTPIN			HAL_GPIO_PORTPIN_0_4

#define		SPI_MOSI_PORTPIN	HAL_GPIO_PORTPIN_0_26
#define		SPI_MISO_PORTPIN	HAL_GPIO_PORTPIN_0_27
#define		SPI_SCK_PORTPIN		HAL_GPIO_PORTPIN_0_28
#define		SPI_SSEL_PORTPIN	HAL_GPIO_PORTPIN_0_29

#define		SPI_INSTANCE		0

#define		WKT_OUT_PORTPIN		HAL_GPIO_PORTPIN_0_17

#define		WKT_TIME_USEG		(5000)

static void tick_callback(void);

static void adc_callback(void);

static void rx_callback(void);

static void tx_callback(void);

static void pinint_callback(void);

static void match_callback(void);

static void spi_tx_callback(void);

static void spi_rx_callback(void);

static void wkt_callback(void);

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

static const hal_usart_config_t uart_config =
{
	.data_length = HAL_USART_DATALEN_8BIT,
	.parity = HAL_USART_PARITY_NO_PARITY,
	.stop_bits = HAL_USART_STOPLEN_1BIT,
	.oversampling = HAL_USART_OVERSAMPLING_X16,
	.clock_selection = HAL_SYSCON_PERIPHERAL_CLOCK_SEL_FRG0,
	.baudrate = UART_BAUDRATE,
	.tx_portpin = UART_TX_PORTPIN,
	.rx_portpin = UART_RX_PORTPIN,
	.tx_ready_callback = tx_callback,
	.rx_ready_callback = rx_callback
};

static const hal_iocon_config_t pin_config =
{
	.pull_mode = HAL_IOCON_PULL_UP,
	.hysteresis = 0,
	.invert_input = 0,
	.open_drain = 0,
	.sample_mode = HAL_IOCON_SAMPLE_MODE_3_CLOCK,
	.clk_sel = HAL_IOCON_CLK_DIV_0,
	.iic_mode = HAL_IOCON_IIC_MODE_GPIO
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

#else

static hal_ctimer_pwm_channel_config_t pwm_channel_config =
{
	.interrupt_on_action = 0,
	.duty = 0,
	.channel_pin = CTIMER_PORT_PIN
};

static const hal_ctimer_pwm_config_t pwm_config =
{
	.clock_div = 0,
	.pwm_period_useg = 1000,
	.interrupt_on_period = 1,
	.callback = match_callback
};

#endif

static const hal_spi_master_mode_config_t spi_master_config =
{
	.clock_source = HAL_SYSCON_PERIPHERAL_CLOCK_SEL_MAIN,
	.pre_delay = 4,
	.post_delay = 4,
	.transfer_delay = 4,
	.frame_delay = 4,
	.sck_portpin = SPI_SCK_PORTPIN,
	.mosi_portpin = SPI_MOSI_PORTPIN,
	.miso_portpin = SPI_MISO_PORTPIN,
	.ssel_portpin[0] = SPI_SSEL_PORTPIN,
	.ssel_portpin[1] = HAL_GPIO_PORTPIN_NOT_USED,
	.ssel_portpin[2] = HAL_GPIO_PORTPIN_NOT_USED,
	.ssel_portpin[3] = HAL_GPIO_PORTPIN_NOT_USED,
	.ssel_polarity[0] = HAL_SPI_SSEL_POLARITY_LOW,
	.ssel_polarity[1] = HAL_SPI_SSEL_POLARITY_LOW,
	.ssel_polarity[2] = HAL_SPI_SSEL_POLARITY_LOW,
	.ssel_polarity[3] = HAL_SPI_SSEL_POLARITY_LOW,
	.tx_free_callback = spi_tx_callback,
	.rx_ready_callback = spi_rx_callback
};

static const hal_spi_master_mode_tx_config_t spi_tx_config =
{
	.clock_mode = HAL_SPI_CLOCK_MODE_0,
	.clock_div = 23
};

static uint32_t blink_time_ms = 0;
static uint32_t adc_conversion = 0;

static uint32_t spi_tx_idx = 0;
static uint32_t spi_rx_idx = 0;

/*
 * Nota: En este caso se manda un buffer constante, lo cual hace parecer poco practico el metodo.
 * En aplicaciones reales, los datos a enviar probablemente se carguen en un buffer auxiliar,
 * de modo tal que en todos se deberia configurar el dato, el esclavo al cual enviar el mismo,
 * y tamaño en bits del dato, mientras que solo en el ultimo dato se debera indicar final de
 * transfer/frame. Tambien es util el bit rxignore, para comandos donde solo se desee escribir, o se
 * quiera aprovechar el tamaño del buffer de recepcion, ignorando los bytes de comando al dispositivo.
 */
static const hal_spi_master_mode_tx_data_t spi_tx_buff[2] =
{
	{
		.data = 0x05,
		.ssel0_n = 0,
		.ssel1_n = 1,
		.ssel2_n = 1,
		.ssel3_n = 1,
		.eot = 0,
		.eof = 0,
		.rxignore = 1,
		.data_length = HAL_SPI_DATA_LENGTH_8_BIT
	},

	{
		.data = HAL_SPI_DUMMY_BYTE,
		.ssel0_n = 0,
		.ssel1_n = 1,
		.ssel2_n = 1,
		.ssel3_n = 1,
		.eot = 1,
		.eof = 0,
		.rxignore = 0,
		.data_length = HAL_SPI_DATA_LENGTH_8_BIT
	},
};

static uint8_t spi_rx_buff[10];

static uint8_t spi_rx_complete_flag = 0;

int main(void)
{
	hal_syscon_fro_clock_config(1);
	hal_syscon_system_clock_set_source(HAL_SYSCON_SYSTEM_CLOCK_SEL_FRO);

	// Clock principal en un pin (utilizando un divisor)
	hal_syscon_clkout_config(CLOCKOUT_PORT_PIN, HAL_SYSCON_CLKOUT_SOURCE_SEL_MAIN_CLOCK, CLOCKOUT_DIVIDER);

	// Hasta aca queda el clock configurado con el FRO interno en 24MHz
	// Configuro el fraccional para poder tener buena presicion para un baudrate de 115200bps
	// El DIV siempre debe estar en 256 (especificacion del manual de usuario)
	// Como fuente utilizo el FRO a 24MHz ya configurado
	// 24MHz / (1 + (47 / 256)) = 20.2772272MHz
	hal_syscon_frg_config(0, HAL_SYSCON_FRG_CLOCK_SEL_MAIN_CLOCK, 47);

	// Divisor para glitches de IOCON
	hal_syscon_iocon_glitch_divider_set(HAL_SYSCON_IOCON_GLITCH_SEL_0, 255);

	hal_iocon_config_io(KEY_PORTPIN, &pin_config);

	hal_gpio_init(HAL_GPIO_PORT_0);
	hal_gpio_init(HAL_GPIO_PORT_1);

	hal_gpio_set_dir(LED_PORT_PIN, HAL_GPIO_DIR_OUTPUT, 0);
	hal_gpio_set_dir(CTIMER_PORT_PIN, HAL_GPIO_DIR_OUTPUT, 1);
	hal_gpio_set_dir(WKT_OUT_PORTPIN, HAL_GPIO_DIR_OUTPUT, 0);

	hal_adc_init_async_mode(ADC_FREQUENCY, 0, HAL_ADC_CLOCK_SOURCE_FRO, HAL_ADC_LOW_POWER_MODE_DISABLED);
	hal_adc_sequence_config(ADC_SEQUENCE, &adc_config);

	hal_usart_init(UART_NUMBER, &uart_config);

	hal_pinint_init();

	//hal_pinint_pin_interrupt_config(&pinint_config);

#ifndef	CTIMER_IN_PWM_MODE
	hal_ctimer_timer_mode_init(0); // Divisor de prescaler en 1

	hal_ctimer_timer_mode_config_match(&match_config);

	hal_ctimer_timer_mode_run();
#else
	hal_ctimer_pwm_mode_init(&pwm_config);

	hal_ctimer_pwm_mode_channel_config(HAL_CTIMER_PWM_CHANNEL_0, &pwm_channel_config);
#endif

	hal_spi_master_mode_init(SPI_INSTANCE, &spi_master_config);

	hal_systick_init(TICK_PERIOD_US, tick_callback);

	hal_wkt_init(HAL_WKT_CLOCK_SOURCE_FRO_DIV, 0, wkt_callback);
	hal_wkt_start_count(WKT_TIME_USEG);

	while(1)
	{
		if(spi_rx_complete_flag)
		{
			spi_rx_complete_flag = 0;
		}
	}

	return 0;
}

static void tick_callback(void)
{
	static uint32_t tick_counter = 0;
	static uint32_t adc_counter = 0;
	static uint32_t nrf_counter = 0;

	tick_counter = (tick_counter + 1) % blink_time_ms;
	adc_counter = (adc_counter + 1) % CONVERSION_TIME_MS;
	nrf_counter = (nrf_counter + 1) % NRF_TIME_MS;

	if(tick_counter == 0)
	{
		hal_gpio_toggle_pin(LED_PORT_PIN);
	}

	if(adc_counter == 0)
	{
		hal_adc_sequence_start(ADC_SEQUENCE);
	}

	if(nrf_counter == 0)
	{
		hal_spi_master_mode_tx_config(SPI_INSTANCE, &spi_tx_config);

		spi_tx_idx = 0;
		spi_rx_idx = 0;
		hal_spi_master_mode_tx_data(SPI_INSTANCE, &spi_tx_buff[spi_tx_idx++]);
	}
}

static void adc_callback(void)
{
	hal_adc_sequence_result_t adc_result;

	if(hal_adc_sequence_get_result(ADC_SEQUENCE, &adc_result) == HAL_ADC_SEQUENCE_RESULT_VALID)
	{
		adc_conversion = adc_result.result;
	}

	adc_conversion /= 4; // 0 ~ 1023

	blink_time_ms = adc_conversion; // 0mseg ~ 1023mseg

	if(adc_conversion > 1000)
	{
		pwm_channel_config.duty = 1000;
	}
	else
	{
		pwm_channel_config.duty = adc_conversion;
	}

	hal_ctimer_pwm_mode_channel_config(HAL_CTIMER_PWM_CHANNEL_0, &pwm_channel_config);
}

static char trama[] = "Trama de prueba para ver que onda\n";
static uint32_t trama_counter = 0;

static void rx_callback(void)
{
	uint32_t data;

	hal_usart_rx_data(UART_NUMBER, &data);

	if((char) data == ' ')
	{
		hal_usart_tx_data(UART_NUMBER, trama[trama_counter++]);
	}
}

static void tx_callback(void)
{
	if(trama[trama_counter] != '\0')
	{
		hal_usart_tx_data(UART_NUMBER, trama[trama_counter++]);
	}
	else
	{
		trama_counter = 0;
	}
}

static void pinint_callback(void)
{
	static uint8_t state = 0;

	if(state == 0)
	{
		state = 1;

		hal_ctimer_pwm_mode_period_set(2000);
	}
	else
	{
		state = 0;

		hal_ctimer_pwm_mode_period_set(1000);
	}
}

static void match_callback(void)
{
	static uint8_t counter = 0;

	counter++;
}

static void spi_tx_callback(void)
{
	if(spi_tx_idx < (sizeof(spi_tx_buff)/sizeof(spi_tx_buff[0])))
	{
		hal_spi_master_mode_tx_data(SPI_INSTANCE, &spi_tx_buff[spi_tx_idx++]);
	}
	else
	{
		// Indicar finalizacion de transmision de ser necesario
	}
}

static void spi_rx_callback(void)
{
	spi_rx_buff[spi_rx_idx++] = hal_spi_master_mode_rx_data(SPI_INSTANCE);

	spi_rx_complete_flag = 1;
}

static void wkt_callback(void)
{
	hal_gpio_toggle_pin(WKT_OUT_PORTPIN);
	hal_wkt_start_count(WKT_TIME_USEG);
}
