/**
 * @file HAL_CTIMER.h
 * @brief Declaraciones a nivel de aplicacion del periferico CTIMER (LPC845)
 * @author Augusto Santini
 * @author Esteban E. Chiama
 * @date 3/2020
 * @version 1.0
 */

#ifndef HAL_CTIMER_H_
#define HAL_CTIMER_H_

#include <stdint.h>
#include "HAL_GPIO.h"

#if defined (__cplusplus)
extern "C" {
#endif

/**
 * @defgroup CTIMER Standard Counter/Timer (CTIMER)
 *
 * # Introducción
 *
 * El periférico *CTIMER* está implementado con toda su funcionalidad de Timer y PWM.
 * Cuenta con una resolución de 32 bits y es configurable su frecuencia de conteo.
 *
 * Esta librería diferencia dos distintos modos de operación del periférico *CTIMER*:
 * - Modo Timer
 * - Modo PWM
 *
 * Se ofrecen distintas funciones de inicialización y configuración para cada uno de estos modos.
 *
 * @note Es importante que, una vez inicializado un modo de funcionamiento, el usuario no intente utilizar el otro sin antes
 * hacer un reset del periférico.
 *
 * # Funcionamiento básico y frecuencia
 *
 * De manera simplificada, el *CTIMER* lleva un contador interno que aumenta en cada ciclo de clock del periférico.
 * Al llegar este valor a los valores de *MATCH* establecido por el usuario, el periférico responde de la manera
 * en que se lo haya configurado: con una interrupción, con un reset, manejando un pin externo del microcontrolador, entre otras.
 *
 * La frecuencia de aumento del contador del *CTIMER* es la frecuencia del periférico. Se calcula de la siguiente manera:
 *
 * \f{eqnarray*}{
 * 		f_{CT} = \frac{SYSTEM\_CLK}{DIV}
 * \f}
 *
 * Donde:
 * - \f$f_{CT}\f$: frecuencia del contador/timer.
 * - \f$SYSTEM\_CLK\f$: frecuencia del *system clock*.
 * - \f$DIV\f$: divisor configurable por el usuario.
 *
 * El divisor \f$DIV\f$ se establece en las funciones de inicialización del periférico:
 *
 * - En la función de inicialización del modo timer hal_ctimer_timer_mode_init() con su único parámetro *clock_div*.
 * - En la función de inicialización del modo PWM, en el parámetro perteneciente a su estructura de configuración
 * se tiene hal_ctimer_pwm_config_t::clock_div.
 *
 * # Modo Timer
 *
 * En modo **Timer** se cuenta con 4 *canales* independientes, sobre los cuales esta librería
 * ofrece la posibilidad de configurar:
 *
 * - Valores de *MATCH*, para los cuales la librería ofrece una interfaz al usuario para que los configure en términos
 * de *tiempo transcurrido en useg* o dicho de otra forma *tiempos de MATCH*.
 *
 * - Para cada canal, el periférico ofrece *señales de salida de MATCH*. Estas pueden cambiar en los momentos en que
 * se cumplen los *tiempos de MATCH* establecidos por el usuario, según se desee.
 *
 * - Las posibles acciones a tomar en dichos momentos sobre las *señales de salida de MATCH* son 4
 * y son las detalladas y seleccionables según la enumeración hal_ctimer_match_action_en().
 *
 * - La posibilidad de asociar las *señales de salida de MATCH* a pines externos del microcontrolador.
 *
 * La inicialización del *CTIMER* en esta modalidad se realiza por medio de la función hal_ctimer_timer_mode_init().
 *
 * La configuración de cada canal específico se establece llamando a la función hal_ctimer_timer_mode_match_config() con su
 * correspondiente estructura hal_ctimer_match_config_t con los parámetros deseados.
 *
 * # Modo PWM
 *
 * En modo PWM la interfaz ofrecida permite manejar hasta 3 salidas de PWM sobre pines externos del microcontrolador.
 *
 * La configuración básica e inicialización de este modo se realiza en el llamado a la función hal_ctimer_pwm_mode_init()
 * por medio de la estructura hal_ctimer_pwm_config_t.
 * Además de establecerse el divisor que determina la frecuencia de conteo del periférico, la configuración básica del PWM consiste de:
 *
 * - Establecer el **período** del PWM.
 * - Si se quiere generar una **interrupción** al cumplirse el mismo o no.
 * - Determinar la **función a ejecutar** en caso de que se quiera dicha interrupción.
 *
 * @note En caso de luego de la inicialización querer modificar el **período** de PWM establecido, se puede hacer mediante
 * uso de la función hal_ctimer_pwm_mode_period_set().
 *
 * Luego, para configurar cada uno de los 3 utilizables canales de PWM, se debe llamar a la función hal_ctimer_pwm_mode_channel_config()
 * con los deseados parámetros de configuración reflejados en la estructura hal_ctimer_pwm_channel_config_t.
 *
 * La configuración de cada canal de PWM consiste de:
 *
 * - Especificación del **duty** deseado para el canal.
 * - Generación o no de una **interrupción** al cumplirse el duty especificado.
 * - **Función a ejecutar** en caso de haber establecido la interrupción.
 * - **Pin** externo del microcontrolador sobre el cual se quiere la señal de este canal de PWM.
 *
 * @note Para hacer cambios en el duty establecido para un canal, el usuario debe simplemente volver a llamar a la
 * función hal_ctimer_pwm_mode_channel_config() con los parámetros deseados.
 *
 * # Campos de aplicación típicos
 *
 * - Medición de tiempos.
 * - Aviso de *eventos*, por ejemplo un *timeout* en una comunicación.
 * - Muestreo y digitalización de señales analógicas en intervalos fijos de tiempo.
 *
 * # Consideraciones sobre los callbacks
 *
 * Las funciones especificadas en hal_ctimer_match_config_t::callback, hal_ctimer_pwm_config_t::callback y
 * hal_ctimer_pwm_channel_config_t::callback son ejecutadas en un contexto de **interrupción**,
 * por lo que debe tratarse de que sean lo más breve posibles.
 *
 * @{
 */

/** Acción a realizar sobre la *salida de match* al alcanzar el valor de su correspondiente canal de match. */
typedef enum
{
	HAL_CTIMER_MATCH_DO_NOTHING = 0, /**< La *salida de match* no cambiará.*/
	HAL_CTIMER_MATCH_CLEAR_PIN,	/**< La *salida de match* será llevada a 0.*/
	HAL_CTIMER_MATCH_SET_PIN, /**< La *salida de match* será llevada a 1.*/
	HAL_CTIMER_MATCH_TOGGLE_PIN, /**< La *salida de match* será invertida.*/
}hal_ctimer_match_action_en;

/** Estructura de configuración de un canal de match para el modo timer.*/
typedef struct
{
	uint8_t interrupt_on_match;
	/**< Si este parámetro no es 0, estará habilitada la generación de interrupciones al alcanzar el *CTIMER*
	el valor de *MATCH* correspondiente al canal de esta configuración.
	Se ejecutará la función especificada en	hal_ctimer_match_config_t::callback.*/
	uint8_t reset_on_match;
	/**< Si este parámetro no es 0, el *CTIMER* reiniciará el conteo al alcanzar
	el valor de *MATCH* correspondiente al canal de esta configuración.*/
	uint8_t stop_on_match;
	/**< Si este parámetro no es 0, el *CTIMER* se detendrá al alcanzar el valor de *MATCH*
	correspondiente al canal de esta configuración.*/
	uint8_t reload_on_match;
	/**<
	- Si este parámetro no es 0, el *CTIMER* cargará nuevos valores de *MATCH* para este canal
	en la próxima ocasión en que el contador del *CTIMER* se reinicie (ya sea manualmente o por
	alcanzar un valor de *MATCH* que haya sido configurado para disparar un reset).
	- Si este parámetro es 0, el *CTIMER* cargará nuevos valores de *MATCH* para este canal de manera inmediata. */
	uint32_t match_value_useg;
	/**< Valor, en microsegundos, en que se desea que el *CTIMER* alcance el valor de *MATCH* del canal configurado.*/
	hal_ctimer_match_action_en match_action;
	/**< Acción a realizar sobre la *salida de match* del canal configurado al alcanzar el *CTIMER*
	su valor de *MATCH* correspondiente. */
	uint8_t enable_external_pin;
	/**< Si este parámetro no es 0, la *salida de match* del canal configurado será reflejada en
	un pin externo del microcontrolador (determinado por el parámetro hal_ctimer_match_config_t::match_pin). */
	hal_gpio_portpin_en match_pin;
	/**< Selección de pin externo del microcontrolador sobre el cual reflejar la *salida de match* del canal configurado,
	cuando hal_ctimer_match_config_t::enable_external_pin lo habilite.*/
	void (*callback)(void);
	/**< Cuando hal_ctimer_match_config_t::interrupt_on_match lo habilite, ésta será la función a ejecutar cuando
	el *CTIMER* genere una interrupción al haber alcanzado el valor de *MATCH* correspondiente al canal de esta configuración.*/
}hal_ctimer_match_config_t;

/** Selección del canal de match del *CTIMER*.*/
typedef enum
{
	HAL_CTIMER_MATCH_0 = 0, /**< Canal de match 0.*/
	HAL_CTIMER_MATCH_1, /**< Canal de match 1.*/
	HAL_CTIMER_MATCH_2, /**< Canal de match 2.*/
	HAL_CTIMER_MATCH_3 /**< Canal de match 3.*/
}hal_ctimer_match_sel_en;

/** Estructura de configuración de un canal para el modo PWM.*/
typedef struct
{
	uint8_t interrupt_on_action;
	/**< Si este parámetro no es 0, el *CTIMER* generará una interrupción
	 * cada vez que se cumpla el *duty* establecido para este canal de PWM.*/
	uint32_t duty;
	/**< Duty en decimas de porciento (1 equivale a 0.1%) */
	hal_gpio_portpin_en channel_pin;
	/**< Especifica el pin externo del microcontrolador sobre el cual reflejar la *salida de match* correspondiente a este canal de PWM.*/
	void (*callback)(void);
	/**< Si hal_ctimer_pwm_channel_config_t::interrupt_on_action lo habilita, ésta será la función que se ejecute
	 * al generarse una interrupción debida a este canal de PWM.*/
}hal_ctimer_pwm_channel_config_t;

/** Estructura de configuración del modo PWM.*/
typedef struct
{
	uint32_t clock_div;
	/**< Divisor que actuará sobre el *system clock*, determina la frecuencia de clock del *CTIMER*. */
	uint32_t pwm_period_useg;
	/**< Período del PWM en microsegundos. */
	uint8_t interrupt_on_period;
	/**< Si este parámetro no es 0, el *CTIMER* generará una interrupción cada vez que se cumpla el período establecido del PWM.*/
	void (*callback)(void);
	/**< Si hal_ctimer_pwm_config_t::interrupt_on_period lo habilita, ésta será la función que se ejecute al generarse la interrupción.*/
}hal_ctimer_pwm_config_t;

/** Selección del canal de PWM */
typedef enum
{
	HAL_CTIMER_PWM_CHANNEL_0 = 0, /**< Canal de PWM 0.*/
	HAL_CTIMER_PWM_CHANNEL_1, /**< Canal de PWM 1.*/
	HAL_CTIMER_PWM_CHANNEL_2 /**< Canal de PWM 2.*/
}hal_ctimer_pwm_channel_sel_en;

/**
 * @brief Inicialización del periférico *CTIMER* en modo timer.
 *
 * La frecuencia de clock del periférico *CTIMER* será aquella del *system_clock* dividida por *clock_div*.
 * En caso de que *clock_div* sea 0, la frecuencia del *CTIMER* será igual a la del *system_clock*.
 *
 * @note La inicialización no dispara el contador.
 * @note El periférico debiera inicializarse para modo timer o para modo PWM, no ambos a la vez.
 *
 * @param[in] clock_div Divisor que se aplicará al *system clock*.
 * @see hal_ctimer_timer_mode_match_config
 * @see hal_ctimer_timer_mode_start
 * @see hal_ctimer_pwm_mode_init
 */
void hal_ctimer_timer_mode_init(uint32_t clock_div);

/**
 * @brief Configurar un canal de match
 *
 * El canal de match especificado por el parámetro *match_sel* será configurado según la
 * estructura *match_config*.
 *
 * @note Antes de llamar a esta función, el periférico debiera de haber sido inicializado con
 * la función hal_ctimer_timer_mode_init().
 * @note Esta función no dispara ni detiene el contador.
 *
 * @param[in] match_sel Canal de match a configurar.
 * @param[in] match_config Estructura con los parámetros de configuración deseados.
 * @see hal_ctimer_match_config_t
 * @see hal_ctimer_timer_mode_init
 */
void hal_ctimer_timer_mode_match_config(hal_ctimer_match_sel_en match_sel, const hal_ctimer_match_config_t *match_config);

/**
 * @brief Habilitar el conteo del *CTIMER*.
 *
 * Previamente, el periférico *CTIMER* debe haber sido inicializado con la función hal_ctimer_timer_mode_init().
 *
 * Además, el usuario probablemente debiera de haber establecido previamente su configuración deseada con la
 * función hal_ctimer_timer_mode_match_config().
 *
 * @see hal_ctimer_timer_mode_init
 * @see hal_ctimer_timer_mode_match_config
 * @see hal_ctimer_timer_mode_stop
 */
void hal_ctimer_timer_mode_start(void);

/**
 * @brief Inhabilita el conteo del *CTIMER*.
 * @note Esta función no reinicia el contador del *CTIMER*, ni altera alguna otra configuración.
 * @see hal_ctimer_timer_mode_start
 * @see hal_ctimer_timer_mode_reset
 */
void hal_ctimer_timer_mode_stop(void);

/**
 * @brief Reinicia el conteo del *CTIMER*.
 * @see hal_ctimer_timer_mode_start
 * @see hal_ctimer_timer_mode_stop
 */
void hal_ctimer_timer_mode_reset(void);

/**
 * @brief Cambia el tiempo de *MATCH* del canal de match especificado.
 *
 * Si el canal de match especificado está configurado para realizar hal_ctimer_match_config_t::reload_on_match,
 * la actualización del valor de match se hará efectiva cuando el contador del *CTIMER*
 * se reinicie (ya sea manualmente o por alcanzar un valor de *MATCH* que haya
 * sido configurado para disparar un reset).
 *
 * Caso contrario, la actualización del valor de match es inmediata.
 *
 * @param[in] match_sel Canal de *match* a configurar.
 * @param[in] match_value_useg Nuevo valor de match, en useg, deseado.
 * @see hal_ctimer_match_config_t
 * @see hal_ctimer_timer_mode_match_config
 */
void hal_ctimer_timer_mode_match_change_value(hal_ctimer_match_sel_en match, uint32_t match_value_useg);

/**
 * @brief Lee el estado de la *salida de match* de un canal especificado.
 * @param[in] match Canal de match a consultar.
 * @return Estado actual de la *salida de match* consultada.
 * @see hal_ctimer_match_set_output
 * @see hal_ctimer_match_clear_output
 */
uint8_t hal_ctimer_match_read_output(hal_ctimer_match_sel_en match);

/**
 * @brief Establece en alto la *salida de match* de un canal especificado.
 * @param[in] match Canal de match seleccionado.
 * @see hal_ctimer_match_read_output
 * @see hal_ctimer_match_clear_output
 */
void hal_ctimer_match_set_output(hal_ctimer_match_sel_en match);

/**
 * @brief Establece en bajo la *salida de match* de un canal especificado.
 * @param[in] match Canal de match seleccionado.
 * @see hal_ctimer_match_read_output
 * @see hal_ctimer_match_set_output
 */
void hal_ctimer_match_clear_output(hal_ctimer_match_sel_en match);

/**
 * @brief Inicialización del periférico *CTIMER* en modo PWM.
 *
 * @note La inicialización en modo PWM dispara el contador.
 * @note El periférico debiera inicializarse para modo timer o para modo PWM, no ambos a la vez.
 *
 * @param[in] config Estructura con los parámetros de configuración deseados.
 * @see hal_ctimer_pwm_config_t
 * @see hal_ctimer_pwm_mode_period_set
 * @see hal_ctimer_pwm_mode_channel_config
 */
void hal_ctimer_pwm_mode_init(const hal_ctimer_pwm_config_t *config);

/**
 * @brief Actualizar el período en modo PWM
 * El nuevo período se hará efectivo al finalizar el conteo del período actual.
 * @param[in] period_useg Nuevo periodo deseado en microsegundos
 * @see hal_ctimer_pwm_mode_init
 * @see hal_ctimer_pwm_mode_channel_config
 */
void hal_ctimer_pwm_mode_period_set(uint32_t period_useg);

/**
 * @brief Actualizar configuracion de algun canal de PWM
 * @param[in] channel_sel Seleccion de canal a configurar
 * @param[in] channel_config Configuracion del canal de PWM
 */
void hal_ctimer_pwm_mode_channel_config(hal_ctimer_pwm_channel_sel_en channel_sel, const hal_ctimer_pwm_channel_config_t *channel_config);

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HAL_CTIMER_H_ */

/**
 * @}
 */
