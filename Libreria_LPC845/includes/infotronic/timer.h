/**
 * @file timer.h
 * @brief Declaraciones para los timers por software
 * @author Augusto Santini
 * @date 12/2019
 * @version 1.0
 */

#ifndef TIMER_H_
#define TIMER_H_

#define		MSEC(x)		(x)
#define		DEC(x)		(x * 100)
#define		SEC(x)		(DEC(x) * 10)
#define		MIN(x)		(SEC(x) * 60)

/**
 * @brief Iniciar un timer
 * @param[in] event_number Numero de evento asociado
 * @param[in] msecs Tiempo (en milisegundos) del timer a vencer
 * @param[in] callback Callback a ejecutar una vez vencido el timer. El parametro pasado al callback es el numero de evento que genero el llamado.
 * @param[in] repeat Si es 0 es un timer one-shot, caso contrario es repetitivo
 */
void timer_start(uint8_t event_number, uint32_t msecs, void (*callback)(uint8_t), uint8_t repeat);

/**
 * @brief Pausar un timer
 * @param[in] event_number Numero de evento asociado
 */
void timer_pause(uint8_t event_number);

/**
 * @brief Despausar/resumir un timer
 * @param[in] event_number Numero de evento asociado
 */
void timer_resume(uint8_t event_number);

/**
 * @brief Detener un timer
 * @param[in] event_number Numero de evento asociado
 */
void timer_stop(uint8_t event_number);

/**
 * @brief Funcion a ejecutar en el loop principal
 *
 * Esta funcion se encargara de llamar a los callbacks de los timers que hayan vencido
 */
void timer_loop(void);

/**
 * @brief Callback para revisar la maquinaria de timers (a ejecutar cada 1mseg)
 */
void timer_check(void);

#endif /* TIMER_H_ */