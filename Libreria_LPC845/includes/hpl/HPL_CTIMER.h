/**
 * @file HPL_CTIMER.h
 * @brief Definiciones a nivel de abstraccion del periferico CTIMER (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HPL_CTIMER_H_
#define HPL_CTIMER_H_

#include <HRI_CTIMER.h>

extern volatile CTIMER_per_t * const CTIMER; //!< Periferico CTIMER

typedef struct
{
	uint8_t interrupt_on_match : 1;
	uint8_t reset_on_match : 1;
	uint8_t stop_on_match : 1;
	uint8_t reload_on_match : 1;
	uint32_t match_value;
	void (*match_callback)(void);
}CTIMER_MR_config_t;

typedef struct
{
	uint8_t rising_edge_capture : 1;
	uint8_t falling_edge_capture : 1;
	uint8_t interrupt_on_capture : 1;
	void (*capture_callback)(void);
}CTIMER_CC_config_t;

typedef enum
{
	CTIMER_EMC_CONFIG_DO_NOTHING = 0,
	CTIMER_EMC_CONFIG_CLEAR,
	CTIMER_EMC_CONFIG_SET,
	CTIMER_EMC_CONFIG_TOGGLE
}CTIMER_EMC_config_en;

typedef enum
{
	CTIMER_CTMODE_CONFIG_TIMER_MODE = 0,
	CTIMER_CTMODE_CONFIG_RISING_EDGE,
	CTIMER_CTMODE_CONFIG_FALLING_EDGE,
	CTIMER_CTMODE_CONFIG_DOUBLE_EDGE
}CTIMER_CTMODE_config_en;

typedef enum
{
	CTIMER_CINSEL_CONFIG_CAP0 = 0,
	CTIMER_CINSEL_CONFIG_CAP1,
	CTIMER_CINSEL_CONFIG_CAP2,
	CTIMER_CINSEL_CONFIG_CAP3
}CTIMER_CINSEL_config_en;

typedef enum
{
	CTIMER_SELCC_CONFIG_CH0_RISING = 0,
	CTIMER_SELCC_CONFIG_CH0_FALLING,
	CTIMER_SELCC_CONFIG_CH1_RISING,
	CTIMER_SELCC_CONFIG_CH1_FALLING,
	CTIMER_SELCC_CONFIG_CH2_RISING,
	CTIMER_SELCC_CONFIG_CH2_FALLING,
	CTIMER_SELCC_CONFIG_CH3_RISING,
	CTIMER_SELCC_CONFIG_CH3_FALLING
}CTIMER_SELCC_config_en;

typedef struct
{
	CTIMER_CTMODE_config_en CTMODE;
	CTIMER_CINSEL_config_en CINSEL;
	uint8_t ENCC;
	CTIMER_SELCC_config_en SELCC;
}CTIMER_CTCR_config_t;

typedef struct
{
	CTIMER_EMC_config_en EMC;
	uint8_t mat_enable;
	uint8_t mat_port;
	uint8_t mat_pin;
}CTIMER_EMR_config_t;


typedef enum
{
	CTIMER_MATCH_SEL_0 = 0,
	CTIMER_MATCH_SEL_1,
	CTIMER_MATCH_SEL_2,
	CTIMER_MATCH_SEL_3
}CTIMER_match_sel_en;

typedef enum
{
	CTIMER_CAPTURE_SEL_0 = 0,
	CTIMER_CAPTURE_SEL_1,
	CTIMER_CAPTURE_SEL_2,
	CTIMER_CAPTURE_SEL_3
}CTIMER_capture_sel_en;

typedef enum
{
	CTIMER_EXTERNAL_MATCH_DO_NOTHING = 0,
	CTIMER_EXTERNAL_MATCH_CLEAR,
	CTIMER_EXTERNAL_MATCH_SET,
	CTIMER_EXTERNAL_MATCH_TOGGLE
}CTIMER_external_match_action_en;

typedef enum
{
	CTIMER_MODE_TIMER = 0,
	CTIMER_MODE_COUNTER_RISING_EDGE,
	CTIMER_MODE_COUNTER_FALLING_EDGE,
	CTIMER_MODE_COUNTER_DUAL_EDGE
}CTIMER_mode_en;

typedef enum
{
	CTIMER_COUNT_IN_CAP0 = 0,
	CTIMER_COUNT_IN_CAP1,
	CTIMER_COUNT_IN_CAP2
}CTIMER_count_in_en;

typedef enum
{
	CTIMER_CAPTURE_RESET_CH0_RISING_EDGE = 0,
	CTIMER_CAPTURE_RESET_CH0_FALLING_EDGE,
	CTIMER_CAPTURE_RESET_CH1_RISING_EDGE,
	CTIMER_CAPTURE_RESET_CH1_FALLING_EDGE,
	CTIMER_CAPTURE_RESET_CH2_RISING_EDGE,
	CTIMER_CAPTURE_RESET_CH2_FALLING_EDGE,
	CTIMER_CAPTURE_RESET_CH3_RISING_EDGE,
	CTIMER_CAPTURE_RESET_CH3_FALLING_EDGE
}CTIMER_capture_reset_edge_en;

typedef enum
{
	CTIMER_PWM_CHANNEL_0 = 0,
	CTIMER_PWM_CHANNEL_1,
	CTIMER_PWM_CHANNEL_2
}CTIMER_pwm_channel_en;

/**
 * @brief Obtener estado de flag de interrupcion de algun match
 * @param[in] match Numero de match a consultar
 * @return Valor del flag actual
 */
static inline uint8_t CTIMER_get_match_irq_flag(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { return CTIMER->IR.MR0INT; break; }
	case CTIMER_MATCH_SEL_1: { return CTIMER->IR.MR1INT; break; }
	case CTIMER_MATCH_SEL_2: { return CTIMER->IR.MR2INT; break; }
	case CTIMER_MATCH_SEL_3: { return CTIMER->IR.MR3INT; break; }
	}

	return 0;
}

/**
 * @brief Obtener estado de flag de interrupcion de algun capture
 * @param[in] match Numero de capture a consultar
 * @return Valor del flag actual
 */
static inline uint8_t CTIMER_get_capture_irq_flag(CTIMER_capture_sel_en capture)
{
	switch(capture)
	{
	case CTIMER_CAPTURE_SEL_0: { return CTIMER->IR.CR0INT; break; }
	case CTIMER_CAPTURE_SEL_1: { return CTIMER->IR.CR1INT; break; }
	case CTIMER_CAPTURE_SEL_2: { return CTIMER->IR.CR2INT; break; }
	case CTIMER_CAPTURE_SEL_3: { return CTIMER->IR.CR3INT; break; }
	}

	return 0;
}

/**
 * @brief Limpiar flag de interrupcion de match
 * @param[in] match Numero de match a limpiar
 */
static inline void CTIMER_clear_match_irq_flag(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { CTIMER->IR.MR0INT = 1; break; }
	case CTIMER_MATCH_SEL_1: { CTIMER->IR.MR1INT = 1; break; }
	case CTIMER_MATCH_SEL_2: { CTIMER->IR.MR2INT = 1; break; }
	case CTIMER_MATCH_SEL_3: { CTIMER->IR.MR3INT = 1; break; }
	}
}

/**
 * @brief Limpiar flag de interrupcion de capture
 * @param[in] match Numero de capture a limpiar
 */
static inline void CTIMER_clear_capture_irq_flag(CTIMER_capture_sel_en capture)
{
	switch(capture)
	{
	case CTIMER_CAPTURE_SEL_0: { CTIMER->IR.CR0INT = 1; break; }
	case CTIMER_CAPTURE_SEL_1: { CTIMER->IR.CR1INT = 1; break; }
	case CTIMER_CAPTURE_SEL_2: { CTIMER->IR.CR2INT = 1; break; }
	case CTIMER_CAPTURE_SEL_3: { CTIMER->IR.CR3INT = 1; break; }
	}
}

/**
 * @brief Habilitar el contador
 */
static inline void CTIMER_enable_counter(void)
{
	CTIMER->TCR.CEN = 1;
}

/**
 * @brief Inhabilitar el contador
 */
static inline void CTIMER_disable_counter(void)
{
	CTIMER->TCR.CEN = 0;
}

/**
 * @brief Accionar el reset del contador
 */
static inline void CTIMER_assert_counter_reset(void)
{
	CTIMER->TCR.CRST = 1;
}

/**
 * @brief Limpiar el reset del contador
 */
static inline void CTIMER_clear_counter_reset(void)
{
	CTIMER->TCR.CRST = 0;
}

/**
 * @brief Escribir conteo
 * @param[in] value Valor deseado
 */
static inline void CTIMER_write_counter(uint32_t value)
{
	CTIMER->TC.TCVAL = value;
}

/**
 * @brief Leer valor de conteo actual
 * @return Valor actual de conteo
 */
static inline uint32_t CTIMER_read_counter(void)
{
	return CTIMER->TC.TCVAL;
}

/**
 * @brief Escribir valor de prescaler
 * @param[in] value Valor de prescaler deseado
 */
static inline void CTIMER_write_prescaler(uint32_t value)
{
	CTIMER->PC.PCVAL = value;
}

/**
 * @brief Leer el valor del prescaler
 * @param[in] Valor de prescaler actual
 */
static inline uint32_t CTIMER_read_prescaler(void)
{
	return CTIMER->PC.PCVAL;
}

/**
 * @brief Habilitar interrupcion en match
 * @param[in] match Numero de match a configurar
 */
static inline void CTIMER_enable_interrupt_on_match(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { CTIMER->MCR.MR0I = 1; break; }
	case CTIMER_MATCH_SEL_1: { CTIMER->MCR.MR1I = 1; break; }
	case CTIMER_MATCH_SEL_2: { CTIMER->MCR.MR2I = 1; break; }
	case CTIMER_MATCH_SEL_3: { CTIMER->MCR.MR3I = 1; break; }
	}
}

/**
 * @brief Inhabilitar interrupcion en match
 * @param[in] match Numero de match a configurar
 */
static inline void CTIMER_disable_interrupt_on_match(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { CTIMER->MCR.MR0I = 0; break; }
	case CTIMER_MATCH_SEL_1: { CTIMER->MCR.MR1I = 0; break; }
	case CTIMER_MATCH_SEL_2: { CTIMER->MCR.MR2I = 0; break; }
	case CTIMER_MATCH_SEL_3: { CTIMER->MCR.MR3I = 0; break; }
	}
}

/**
 * @brief Habilitar reset en match
 * @param[in] match Numero de match a configurar
 */
static inline void CTIMER_enable_reset_on_match(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { CTIMER->MCR.MR0R = 1; break; }
	case CTIMER_MATCH_SEL_1: { CTIMER->MCR.MR1R = 1; break; }
	case CTIMER_MATCH_SEL_2: { CTIMER->MCR.MR2R = 1; break; }
	case CTIMER_MATCH_SEL_3: { CTIMER->MCR.MR3R = 1; break; }
	}
}

/**
 * @brief Inhabilitar reset en match
 * @param[in] match Numero de match a configurar
 */
static inline void CTIMER_disable_reset_on_match(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { CTIMER->MCR.MR0R = 0; break; }
	case CTIMER_MATCH_SEL_1: { CTIMER->MCR.MR1R = 0; break; }
	case CTIMER_MATCH_SEL_2: { CTIMER->MCR.MR2R = 0; break; }
	case CTIMER_MATCH_SEL_3: { CTIMER->MCR.MR3R = 0; break; }
	}
}

/**
 * @brief Habilitar stop en match
 * @param[in] match Numero de match a configurar
 */
static inline void CTIMER_enable_stop_on_match(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { CTIMER->MCR.MR0S = 1; break; }
	case CTIMER_MATCH_SEL_1: { CTIMER->MCR.MR1S = 1; break; }
	case CTIMER_MATCH_SEL_2: { CTIMER->MCR.MR2S = 1; break; }
	case CTIMER_MATCH_SEL_3: { CTIMER->MCR.MR3S = 1; break; }
	}
}

/**
 * @brief Inhabilitar stop en match
 * @param[in] match Numero de match a configurar
 */
static inline void CTIMER_disable_stop_on_match(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { CTIMER->MCR.MR0S = 0; break; }
	case CTIMER_MATCH_SEL_1: { CTIMER->MCR.MR1S = 0; break; }
	case CTIMER_MATCH_SEL_2: { CTIMER->MCR.MR2S = 0; break; }
	case CTIMER_MATCH_SEL_3: { CTIMER->MCR.MR3S = 0; break; }
	}
}

/**
 * @brief Habilitar reload en match
 * @param[in] match Numero de match a configurar
 */
static inline void CTIMER_enable_reload_on_match(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { CTIMER->MCR.MR0RL = 1; break; }
	case CTIMER_MATCH_SEL_1: { CTIMER->MCR.MR1RL = 1; break; }
	case CTIMER_MATCH_SEL_2: { CTIMER->MCR.MR2RL = 1; break; }
	case CTIMER_MATCH_SEL_3: { CTIMER->MCR.MR3RL = 1; break; }
	}
}

/**
 * @brief Inhabilitar reload en match
 * @param[in] match Numero de match a configurar
 */
static inline void CTIMER_disable_reload_on_match(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { CTIMER->MCR.MR0RL = 0; break; }
	case CTIMER_MATCH_SEL_1: { CTIMER->MCR.MR1RL = 0; break; }
	case CTIMER_MATCH_SEL_2: { CTIMER->MCR.MR2RL = 0; break; }
	case CTIMER_MATCH_SEL_3: { CTIMER->MCR.MR3RL = 0; break; }
	}
}

/**
 * @brief Escribir un registro de match
 * @param[in] match Numero de match a configurar
 * @param[in] value Valor de match deseado
 */
static inline void CTIMER_write_match_value(CTIMER_match_sel_en match, uint32_t value)
{
	CTIMER->MR[match].MATCH = value;
}

/**
 * @brief Leer un registro de match
 * @param[in] match Numero de match a configurar
 * @return Valor de match actual
 */
static inline uint32_t CTIMER_read_match_value(CTIMER_match_sel_en match)
{
	return CTIMER->MR[match].MATCH;
}

/**
 * @brief Habilitar captura en flanco ascendente
 * @param[in] capture Numero de capture a configurar
 */
static inline void CTIMER_enable_rising_edge_capture(CTIMER_capture_sel_en capture)
{
	switch(capture)
	{
	case CTIMER_CAPTURE_SEL_0: { CTIMER->CCR.CAP0RE = 1; break; }
	case CTIMER_CAPTURE_SEL_1: { CTIMER->CCR.CAP1RE = 1; break; }
	case CTIMER_CAPTURE_SEL_2: { CTIMER->CCR.CAP2RE = 1; break; }
	case CTIMER_CAPTURE_SEL_3: { CTIMER->CCR.CAP3RE = 1; break; }
	}
}

/**
 * @brief Inhabilitar captura en flanco ascendente
 * @param[in] capture Numero de capture a configurar
 */
static inline void CTIMER_disable_rising_edge_capture(CTIMER_capture_sel_en capture)
{
	switch(capture)
	{
	case CTIMER_CAPTURE_SEL_0: { CTIMER->CCR.CAP0RE = 0; break; }
	case CTIMER_CAPTURE_SEL_1: { CTIMER->CCR.CAP1RE = 0; break; }
	case CTIMER_CAPTURE_SEL_2: { CTIMER->CCR.CAP2RE = 0; break; }
	case CTIMER_CAPTURE_SEL_3: { CTIMER->CCR.CAP3RE = 0; break; }
	}
}

/**
 * @brief Habilitar captura en flanco descendente
 * @param[in] capture Numero de capture a configurar
 */
static inline void CTIMER_enable_falling_edge_capture(CTIMER_capture_sel_en capture)
{
	switch(capture)
	{
	case CTIMER_CAPTURE_SEL_0: { CTIMER->CCR.CAP0FE = 1; break; }
	case CTIMER_CAPTURE_SEL_1: { CTIMER->CCR.CAP1FE = 1; break; }
	case CTIMER_CAPTURE_SEL_2: { CTIMER->CCR.CAP2FE = 1; break; }
	case CTIMER_CAPTURE_SEL_3: { CTIMER->CCR.CAP3FE = 1; break; }
	}
}

/**
 * @brief Inhabilitar captura en flanco descendente
 * @param[in] capture Numero de capture a configurar
 */
static inline void CTIMER_disable_falling_edge_capture(CTIMER_capture_sel_en capture)
{
	switch(capture)
	{
	case CTIMER_CAPTURE_SEL_0: { CTIMER->CCR.CAP0FE = 0; break; }
	case CTIMER_CAPTURE_SEL_1: { CTIMER->CCR.CAP1FE = 0; break; }
	case CTIMER_CAPTURE_SEL_2: { CTIMER->CCR.CAP2FE = 0; break; }
	case CTIMER_CAPTURE_SEL_3: { CTIMER->CCR.CAP3FE = 0; break; }
	}
}

/**
 * @brief Habilitar interrupcion en captura
 * @param[in] capture Numero de capture a configurar
 */
static inline void CTIMER_enable_interrupt_on_capture(CTIMER_capture_sel_en capture)
{
	switch(capture)
	{
	case CTIMER_CAPTURE_SEL_0: { CTIMER->CCR.CAP0I = 1; break; }
	case CTIMER_CAPTURE_SEL_1: { CTIMER->CCR.CAP1I = 1; break; }
	case CTIMER_CAPTURE_SEL_2: { CTIMER->CCR.CAP2I = 1; break; }
	case CTIMER_CAPTURE_SEL_3: { CTIMER->CCR.CAP3I = 1; break; }
	}
}

/**
 * @brief Inhabilitar interrupcion en captura
 * @param[in] capture Numero de capture a configurar
 */
static inline void CTIMER_disable_interrupt_on_capture(CTIMER_capture_sel_en capture)
{
	switch(capture)
	{
	case CTIMER_CAPTURE_SEL_0: { CTIMER->CCR.CAP0I = 0; break; }
	case CTIMER_CAPTURE_SEL_1: { CTIMER->CCR.CAP1I = 0; break; }
	case CTIMER_CAPTURE_SEL_2: { CTIMER->CCR.CAP2I = 0; break; }
	case CTIMER_CAPTURE_SEL_3: { CTIMER->CCR.CAP3I = 0; break; }
	}
}

/**
 * @brief Leer registro de captura
 * @param[in] capture Numero de captura a leer
 * @return Valor actual de la captura
 */
static inline uint32_t CTIMER_read_capture_value(CTIMER_capture_sel_en capture)
{
	return CTIMER->CR[capture].CAP;
}

/**
 * @brief Leer estado de match externo
 * @param[in] match Numero de match externo a consultar
 * @return Estado del match actual
 */
static inline uint8_t CTIMER_read_match_status(CTIMER_match_sel_en match)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { return CTIMER->EMR.EM0; break; }
	case CTIMER_MATCH_SEL_1: { return CTIMER->EMR.EM1; break; }
	case CTIMER_MATCH_SEL_2: { return CTIMER->EMR.EM2; break; }
	case CTIMER_MATCH_SEL_3: { return CTIMER->EMR.EM3; break; }
	}

	return 0;
}

/**
 * @brief Configurar accion en match externo
 * @param[in] match Numero de match a configurar
 * @param[in] action Accion a realizar en match externo
 */
static inline void CTIMER_config_external_match(CTIMER_match_sel_en match, CTIMER_external_match_action_en action)
{
	switch(match)
	{
	case CTIMER_MATCH_SEL_0: { CTIMER->EMR.EMC0 = action; break; }
	case CTIMER_MATCH_SEL_1: { CTIMER->EMR.EMC1 = action; break; }
	case CTIMER_MATCH_SEL_2: { CTIMER->EMR.EMC2 = action; break; }
	case CTIMER_MATCH_SEL_3: { CTIMER->EMR.EMC3 = action; break; }
	}
}

/**
 * @brief Configurar modo de funcionamiento
 * @param[in] mode Modo de funcionamiento
 */
static inline void CTIMER_config_counter_timer_mode(CTIMER_mode_en mode)
{
	CTIMER->CTCR.CTMODE = mode;
}

/**
 * @brief Configurar entrada de conteo (modo counter rising/falling/dual edge)
 * @param[in] count_in Entrada de conteo deseada
 */
static inline void CTIMER_config_counter_input(CTIMER_count_in_en count_in)
{
	CTIMER->CTCR.CINSEL = count_in;
}

/**
 * @brief Habilitar reset de conteo y prescaler en captura
 */
static inline void CTIMER_enable_count_reset_on_capture(void)
{
	CTIMER->CTCR.ENCC = 1;
}

/**
 * @brief Inhabilitar reset de conteo y prescaler en captura
 */
static inline void CTIMER_disable_count_reset_on_capture(void)
{
	CTIMER->CTCR.ENCC = 0;
}

/**
 * @brief Configurar captura que genera reset de conteo y prescaler (si esta habilitado)
 * @param[in] capture_sel Captura que genera el reset de conteo y prescaler
 */
static inline void CTIMER_config_capture_reset(CTIMER_capture_reset_edge_en capture_sel)
{
	CTIMER->CTCR.SELCC = capture_sel;
}

/**
 * @brief Habilitar canal de PWM
 * @param[in] pwm Canal deseado
 */
static inline void CTIMER_enable_pwm(CTIMER_pwm_channel_en pwm)
{
	switch(pwm)
	{
	case CTIMER_PWM_CHANNEL_0: { CTIMER->PWMC.PWMEN0 = 1; break; }
	case CTIMER_PWM_CHANNEL_1: { CTIMER->PWMC.PWMEN1 = 1; break; }
	case CTIMER_PWM_CHANNEL_2: { CTIMER->PWMC.PWMEN2 = 1; break; }
	}
}

/**
 * @brief Inhabilitar canal de PWM
 * @param[in] pwm Canal deseado
 */
static inline void CTIMER_disable_pwm(CTIMER_pwm_channel_en pwm)
{
	switch(pwm)
	{
	case CTIMER_PWM_CHANNEL_0: { CTIMER->PWMC.PWMEN0 = 0; break; }
	case CTIMER_PWM_CHANNEL_1: { CTIMER->PWMC.PWMEN1 = 0; break; }
	case CTIMER_PWM_CHANNEL_2: { CTIMER->PWMC.PWMEN2 = 0; break; }
	}
}

/**
 * @brief Escribir registros fantasma de match
 * @param[in] match Match a escribir
 * @param[in] value Valor deseado
 */
static inline void CTIMER_write_shadow_register(CTIMER_match_sel_en match, uint32_t value)
{
	CTIMER->MSR[match].SHADOW = value;
}

#endif /* HPL_CTIMER_H_ */
