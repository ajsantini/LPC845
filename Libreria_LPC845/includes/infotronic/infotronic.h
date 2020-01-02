/**
 * @file infotronic.h
 * @brief Declaraciones para la placa infotronic v2
 * @author Augusto Santini
 * @date 12/2019
 * @version 1.0
 */

#ifndef INFOTRONIC_H_
#define INFOTRONIC_H_

#include <stdint.h>

#define		INFOTRONIC_NO_KEY		0xFF

#define		INFOTRONIC_MSEC(x)		(x)
#define		INFOTRONIC_DEC(x)		(x * 100)
#define		INFOTRONIC_SEC(x)		(DEC(x) * 10)
#define		INFOTRONIC_MIN(x)		(SEC(x) * 60)

/**
 * @brief Inicializacion de la placa infotronic v2
 */
void infotronic_init(void);

/**
 * @brief Obtener tecla pulsada
 * @return Tecla pulsada (0 ~ 5 o NO_KEY), una vez leida, se limpia el buffer hasta que se presione la proxima tecla
 */
uint8_t infotronic_get_key(void);

/**
 * @brief Cerrar un rele
 * @param[in] numero_rele Numero de rele a cerrar (0 ~ 3)
 */
void infotronic_relay_close(uint8_t numero_rele);

/**
 * @brief Abrir un rele
 * @param[in] numero_rele Numero de rele a abrir (0 ~ 3)
 */
void infotronic_relay_open(uint8_t numero_rele);

/**
 * @brief Invertir el estado de un rele
 * @param[in] numero_rele Numero de rele a invertir (0 ~ 3)
 */
void infotronic_relay_toggle(uint8_t numero_rele);

/**
 * @brief Apagar display
 * @param[in] display Numero de display a apagar (0 ~ 1)
 */
void infotronic_display_off(uint8_t display);

/**
 * @brief Encender display
 * @param[in] display Numero de display a encender (0 ~ 1)
 */
void infotronic_display_on(uint8_t display);

/**
 * @brief Escribir display
 * @param[in] display Numero de display sobre el cual escribir (0 ~ 1)
 * @param[in] number Numero a escribir en el display (0 ~ 999)
 */
void infotronic_display_write(uint8_t display, uint32_t number);

/**
 * @brief Encender el display point de un digito de un display
 * @param[in] display Numero de display sobre el cual escribir (0 ~ 1)
 * @param[in] digit Digito del display a prender (0 ~ 2)
 */
void infotronic_display_set_dp(uint8_t display, uint8_t digit);

/**
 * @brief Apagar el display point de un digito de un display
 * @param[in] display Numero de display sobre el cual escribir (0 ~ 1)
 * @param[in] digit Digito del display a prender (0 ~ 2)
 */
void infotronic_display_clear_dp(uint8_t display, uint8_t digit);

/**
 * @brief Tomar la lectura actual del termometro
 * @return Lectura actual del termometro en mili °C
 */
uint32_t infotronic_termometro_read(void);

/**
 * @brief Iniciar un timer
 * @param[in] event_number Numero de evento asociado
 * @param[in] msecs Tiempo (en milisegundos) del timer a vencer
 * @param[in] callback Callback a ejecutar una vez vencido el timer
 * @param[in] repeat Si es 0 es un timer one-shot, caso contrario es repetitivo
 */
void infotronic_timer_start(uint8_t event_number, uint32_t msecs, void (*callback)(void), uint8_t repeat);

/**
 * @brief Pausar un timer
 * @param[in] event_number Numero de evento asociado
 */
void infotronic_timer_pause(uint8_t event_number);

/**
 * @brief Despausar/resumir un timer
 * @param[in] event_number Numero de evento asociado
 */
void infotronic_timer_resume(uint8_t event_number);

/**
 * @brief Detener un timer
 * @param[in] event_number Numero de evento asociado
 */
void infotronic_timer_stop(uint8_t event_number);

/**
 * @brief Funcion a ejecutar en el loop principal
 *
 * Esta funcion se encargara de llamar a los callbacks de los timers que hayan vencido
 */
void infotronic_timer_loop(void);

#endif /* INFOTRONIC_H_ */
