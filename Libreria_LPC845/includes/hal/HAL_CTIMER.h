/**
 * @file HAL_CTIMER.h
 * @brief Declaraciones a nivel de aplicacion del periferico CTIMER (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HAL_CTIMER_H_
#define HAL_CTIMER_H_

typedef enum
{
	HAL_CTIMER_MATCH_DO_NOGHING = 0,
	HAL_CTIMER_MATCH_CLEAR_PIN,
	HAL_CTIMER_MATCH_SET_PIN,
	HAL_CTIMER_MATCH_TOGGLE_PIN,
}hal_ctimer_match_action_en;

typedef struct
{
	uint8_t interrupt_on_match;
	uint8_t reset_on_match;
	uint8_t stop_on_match;
	uint32_t match_value_useg;
	uint8_t match_action_enable;
	hal_ctimer_match_action_en match_action;
	void (*callback)(void);
}hal_ctimer_match_config_t;

typedef struct
{
	uint32_t clock_div;
	hal_ctimer_match_config_t match_config[4];
}hal_ctimer_timer_config_t;

/**
 * @brief Inicializacion del periferico en modo timer
 * @param[in] config Configuracion deseada
 */
void hal_ctimer_init_timer_mode(const hal_ctimer_timer_config_t *config);

#endif /* HAL_CTIMER_H_ */
