/**
 * @file HAL_SYSTICK.h
 * @brief Declaraciones a nivel de aplicacion del periferico SYSICK (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @defgroup SYSTICK Tick del Sistema (SYSTICK)
 *
 * # Introducción
 *
 * El periférico *SYSTICK* es un timer básico cuya funcionalidad principal es mantener una base de tiempos, en
 * general fija, para múltiples tareas que necesiten cierta periodicidad o una base de tiempo. La frecuencia con
 * la que ocurren dichos ticks es configurable y valores típicos pueden ser: 1 milisegundo, 2.5 milisegundos,
 * 10 milisegundos entre otros.
 *
 * # Fuente de clock
 *
 * El periférico admite dos ocpiones de fuente de clock para el mismo:
 * 		- Clock principal del sistema
 * 		- Clock principal del sistema dividido 2
 * 		.
 *
 * # Configuración de tiempo de tick
 *
 * Para configurar el tiempo de tick del periférico, el mismo cuenta con un contador descendente de *24 bits*
 * el cual cuando desborda, activa un flag, pudiendo el mismo generar una interrupción o no. La librería implementa
 * una función @ref hal_systick_init a la cual simplemente se le asigna el tiempo de tick deseado en microsegundos
 * y automáticamente configura el periférico para generar ticks con dicha base de tiempo, pudiendo ejecutar un
 * callback en caso de ser necesario.
 *
 * # Callback de tick
 *
 * Usualmente el periférico se configura para generar interrupciones y ejecutar un callback asociado en dicho
 * momento. Es importante recordar que dicho callback se ejecutará en el contexto de una interrupción, por lo
 * que el callback a ejecutar tendrá que tener todas las consideraciones pertinentes. En la librería, el
 * callback puede ser configurado al llamar a la función @ref hal_systick_init o luego de haber inicializado
 * el periférico, si es necesario cambiar el callback, o inhabilitarlo, se puede llamar a la función
 * @ref hal_systick_update_callback.
 *
 * # Inhibición de tick
 *
 * Para ciertos procesos críticos, es deseable inhibir las interrupciones del *SYSTICK*. Para este propósito
 * se disponen de las funciones @ref hal_systick_inhibit_set y @ref hal_systick_inhibit_clear.
 *
 * @{
 */

#ifndef HAL_SYSTICK_H_
#define HAL_SYSTICK_H_

#include <stdint.h>

#if defined (__cplusplus)
extern "C" {
#endif

/** Tipo de dato para callbacks del *SYSTICK* */
typedef void (*hal_systick_callback_t)(void);

/**
 * @brief Inicializacion del SYSTICK
 * @param[in] tick_us Tiempo en microsegundos deseado para el tick
 * @param[in] callback Funcion a llamar en cada tick
 */
void hal_systick_init(uint32_t tick_us, void (*callback)(void));

/**
 * @brief Actualizar callback del SYSTICK
 * @param[in] callback Nuevo callback a ejecutar en cada tick
 *
 * Si se pasa como parámetro *NULL*, se inhabilitarán las interrupciones
 */
void hal_systick_update_callback(hal_systick_callback_t callback);

/**
 * @brief Inhabilitar interrupciones de *SYSTICK*
 */
void hal_systick_inhibit_set(void);

/**
 * @brief Habilitar interrupciones de *SYSTICK*
 */
void hal_systick_inhibit_clear(void);

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HAL_SYSTICK_H_ */

/**
 * @}
 */
