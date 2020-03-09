/**
 * @file infotronic.c
 * @brief Funciones de la infotronic v2
 * @author Augusto Santini
 * @date 12/2019
 * @version 1.0
 */

#include <stdint.h>
#include <HPL_SYSCON.h>
#include <HPL_IOCON.h>
#include <HPL_GPIO.h>
#include <HPL_SYSTICK.h>
#include <HPL_ADC.h>
#include <teclado.h>
#include <relays.h>
#include <display.h>
#include <termometro.h>
#include <timer.h>
#include <LCD.h>

#define		TECLADO_BOUNCES_DEFAULT_MS		200
#define		INFOTRONIC_TICK_US				1000

static uint32_t blocking_delay_counter = 0;

static void infotronic_systick_callback(void);

/**
 * @brief Inicializacion de la placa infotronic v2
 * @param crystal_freq Frecuencia del cristal utilizado en Hz. En caso de no utilizar cristal, pasar cero (0)
 */
void infotronic_init(uint32_t crystal_freq)
{
	if(crystal_freq)
	{
		SYSCON_set_crystal_clock(crystal_freq);
		SYSCON_set_system_clock_source(SYSTEM_CLOCK_SEL_EXT_CLK);
	}

	IOCON_init();
	GPIO_init(0);
	GPIO_init(1);

	teclado_init(TECLADO_BOUNCES_DEFAULT_MS);

	relays_init();

	display_init();

	termometro_init();

	SYSTICK_init(INFOTRONIC_TICK_US, infotronic_systick_callback);
}

/**
 * @brief Delay bloqueante
 * @param[in] mseg_to_delay Tiempo en milsegundos para bloquear
 */
void infotronic_blocking_delay(uint32_t mseg_to_delay)
{
	blocking_delay_counter = mseg_to_delay;

	while(blocking_delay_counter);
}

/**
 * @brief Obtener tecla pulsada
 * @return Tecla pulsada (0 ~ 5 o NO_KEY), una vez leida, se limpia el buffer hasta que se presione la proxima tecla
 */
uint8_t infotronic_get_key(void)
{
	return get_key();
}

/**
 * @brief Energizar un rele
 * @param[in] numero_rele Numero de rele a energizar (0 ~ 3)
 */
void infotronic_relay_activate(uint8_t numero_rele)
{
	relays_activate(numero_rele);
}

/**
 * @brief Desenergizar un rele
 * @param[in] numero_rele Numero de rele a desenergizar (0 ~ 3)
 */
void infotronic_relay_deactivate(uint8_t numero_rele)
{
	relays_deactivate(numero_rele);
}

/**
 * @brief Invertir el estado de un rele
 * @param[in] numero_rele Numero de rele a invertir (0 ~ 3)
 */
void infotronic_relay_toggle(uint8_t numero_rele)
{
	relays_toggle(numero_rele);
}

/**
 * @brief Apagar display
 * @param[in] display Numero de display a apagar (0 ~ 1)
 */
void infotronic_display_off(uint8_t display)
{
	display_off(display);
}

/**
 * @brief Encender display
 * @param[in] display Numero de display a encender (0 ~ 1)
 */
void infotronic_display_on(uint8_t display)
{
	display_on(display);
}

/**
 * @brief Escribir display
 * @param[in] display Numero de display sobre el cual escribir (0 ~ 1)
 * @param[in] number Numero a escribir en el display (0 ~ 999)
 */
void infotronic_display_write(uint8_t display, uint32_t number)
{
	display_write(display, number);
}

/**
 * @brief Encender el display point de un digito de un display
 * @param[in] display Numero de display sobre el cual escribir (0 ~ 1)
 * @param[in] digit Digito del display a prender (0 ~ 2)
 */
void infotronic_display_set_dp(uint8_t display, uint8_t digit)
{
	display_set_dp(display, digit);
}

/**
 * @brief Apagar el display point de un digito de un display
 * @param[in] display Numero de display sobre el cual escribir (0 ~ 1)
 * @param[in] digit Digito del display a prender (0 ~ 2)
 */
void infotronic_display_clear_dp(uint8_t display, uint8_t digit)
{
	display_clear_dp(display, digit);
}

/**
 * @brief Tomar la lectura actual del termometro
 * @return Lectura actual del termometro en mili °C
 */
uint32_t infotronic_termometro_read(void)
{
	return termometro_read();
}

/**
 * @brief Iniciar un timer
 * @param[in] event_number Numero de evento asociado
 * @param[in] msecs Tiempo (en milisegundos) del timer a vencer
 * @param[in] callback Callback a ejecutar una vez vencido el timer. El parametro pasado es el numero de evento que genero el llamado
 * @param[in] repeat Si es 0 es un timer one-shot, caso contrario es repetitivo
 */
void infotronic_timer_start(uint8_t event_number, uint32_t msecs, void (*callback)(uint8_t), uint8_t repeat)
{
	timer_start(event_number, msecs, callback, repeat);
}

/**
 * @brief Pausar un timer
 * @param[in] event_number Numero de evento asociado
 */
void infotronic_timer_pause(uint8_t event_number)
{
	timer_pause(event_number);
}

/**
 * @brief Despausar/resumir un timer
 * @param[in] event_number Numero de evento asociado
 */
void infotronic_timer_resume(uint8_t event_number)
{
	timer_resume(event_number);
}

/**
 * @brief Detener un timer
 * @param[in] event_number Numero de evento asociado
 */
void infotronic_timer_stop(uint8_t event_number)
{
	timer_stop(event_number);
}

/**
 * @brief Funcion a ejecutar en el loop principal
 *
 * Esta funcion se encargara de llamar a los callbacks de los timers que hayan vencido
 */
void infotronic_timer_loop(void)
{
	timer_loop();
}

/**
 * @brief Escribir un mensaje en el display LCD
 * @param[in] message Puntero al mensaje a escribir, terminando con el caracter nulo
 * @param[in] renglon Renglon en cual escribir el mensaje (LCD_RENGLON_1 o LCD_RENGLON_2)
 * @param[in] start_position Posicion del renglon a partir de la cual comenzar a escribir el mensaje
 */
void infotronic_LCD_write(char * message, uint8_t renglon, uint8_t start_position)
{
	LCD_write(message, renglon, start_position);
}

/**
 * @brief Callback del systick para lo que tiene que atender la placa infotronic v2
 */
static void infotronic_systick_callback(void)
{
	timer_check();
	teclado_check();
	termometro_check();
	relays_check();
	display_check();
	LCD_check();

	if(blocking_delay_counter > 0)
	{
		blocking_delay_counter--;
	}
}
