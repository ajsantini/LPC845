/**
 * Descripcion del programa, simplemente utilizando el stick.
 *
 * El programa envia una trama constante ("Hola\n") por el puerto serie propio del debugger.
 *
 * En mi PC, estando en Ubuntu, el mismo aparece como el dispositivo "ttyACM0", en Windows probablemente
 * habria que chequear que "COMn" aparece.
 * Si el programa recibe cualquier caracter luego de haber enviado la trama constante, enciende el
 * LED verde del stick durante un segundo, mientras que si no recibe nada durante 1 segundo entero
 * luego de haber terminado de enviar la trama constante, encendera el LED rojo durante 2 segundos
 * y medio. Durante el proceso de envio de la trama constante, se enciende el LED azul.
 *
 * El proceso se repite indefinidamente.
 */

#include <HPL_UART.h>
#include <HPL_SYSCON.h>
#include <HPL_SYSTICK.h>
#include <HPL_GPIO.h>

#define		TICK_TIME_US		1000
#define		LED_AMOUNT			3

#define		LED_OFF				1
#define		LED_ON				0

#define		UART_VCOM			0

#define		TIMEOUT_TIME_MS		1000
#define		RED_BLINK_TIME_MS	2500
#define		GREEN_BLINK_TIME_MS	1000

static const uint8_t led_ports[LED_AMOUNT] =
{
	1, 1, 1
};

static const uint8_t led_pins[LED_AMOUNT] =
{
	0, 1, 2
};

typedef enum
{
	LED_GREEN = 0,
	LED_BLUE,
	LED_RED
}led_selection_en;

typedef enum
{
	STATE_IDLE = 0,
	STATE_SENDING_MSG,
	STATE_WAITING_ANS,
	STATE_ANS_RECEIVED_OK,
	STATE_TIMEOUT
}states_en;

static void init_led_gpio(void);

static void rx_callback(void);
static void tx_callback(void);

static void tick_callback(void);

static inline void set_led(led_selection_en led);
static inline void clear_led(led_selection_en led);

static uint32_t timeout_counter_ms = 0;

static const uint8_t msg_to_send[6] = "Hola\n";
static uint8_t flag_msg_sending = 0;
static uint8_t flag_msg_received = 0;
static uint8_t msg_idx_counter = 0;

int main(void)
{
	const UART_config_t uart_config =
	{
		.data_length = UART_DATALEN_8BIT,
		.parity = UART_PARITY_NO_PARITY,
		.stop_bits = UART_STOPLEN_1BIT,
		.oversampling = UART_OVERSAMPLING_X16,
		.clock_selection = SYSCON_PERIPHERAL_CLOCK_SEL_FRO,
		.baudrate = 9600,
		.tx_port = 0,
		.tx_pin = 25,
		.rx_port = 0,
		.rx_pin = 24,
		.rx_ready_callback = rx_callback,
		.tx_ready_callback = tx_callback
	};

	states_en state = STATE_SENDING_MSG;

	SYSCON_set_fro_direct();
	SYSCON_set_system_clock_source(SYSCON_SYSTEM_CLOCK_SEL_FRO);

	init_led_gpio();

	UART_init(UART_VCOM, &uart_config);

	SYSTICK_init(TICK_TIME_US, tick_callback);

	while(1)
	{
		switch(state)
		{
		case STATE_IDLE:
		{
			set_led(LED_BLUE);
			UART_tx_byte(UART_VCOM, msg_to_send[msg_idx_counter++]);
			flag_msg_sending = 1;
			state = STATE_SENDING_MSG;

			break;
		}

		case STATE_SENDING_MSG:
		{
			if(flag_msg_sending == 0)
			{
				clear_led(LED_BLUE);
				timeout_counter_ms = TIMEOUT_TIME_MS;
				flag_msg_received = 0;
				state = STATE_WAITING_ANS;
			}

			break;
		}

		case STATE_WAITING_ANS:
		{
			if(timeout_counter_ms == 0)
			{
				timeout_counter_ms = RED_BLINK_TIME_MS;
				set_led(LED_RED);
				state = STATE_TIMEOUT;
			}
			else if(flag_msg_received == 1)
			{
				flag_msg_received = 0;
				timeout_counter_ms = GREEN_BLINK_TIME_MS;
				set_led(LED_GREEN);
				state = STATE_ANS_RECEIVED_OK;
			}

			break;
		}

		case STATE_TIMEOUT:
		{
			if(timeout_counter_ms == 0)
			{
				clear_led(LED_RED);
				state = STATE_IDLE;
			}

			break;
		}

		case STATE_ANS_RECEIVED_OK:
		{
			if(timeout_counter_ms == 0)
			{
				clear_led(LED_GREEN);
				state = STATE_IDLE;
			}

			break;
		}
		}
	}

	return 0;
}

static void init_led_gpio(void)
{
	uint8_t counter;

	for(counter = 0; counter < LED_AMOUNT; counter++)
	{
		GPIO_init(led_ports[counter]);
		GPIO_set_dir(led_ports[counter], led_pins[counter], GPIO_DIR_OUTPUT, LED_OFF);
	}
}

static void rx_callback(void)
{
	flag_msg_received = 1;
}

static void tx_callback(void)
{
	if(msg_to_send[msg_idx_counter] != '\0')
	{
		UART_tx_byte(UART_VCOM, msg_to_send[msg_idx_counter++]);
	}
	else
	{
		msg_idx_counter = 0;
		flag_msg_sending = 0;
	}
}

static void tick_callback(void)
{
	if(timeout_counter_ms > 0)
	{
		timeout_counter_ms--;
	}
}

static inline void set_led(led_selection_en led)
{
	GPIO_clear_pin(led_ports[led], led_pins[led]);
}

static inline void clear_led(led_selection_en led)
{
	GPIO_set_pin(led_ports[led], led_pins[led]);
}
