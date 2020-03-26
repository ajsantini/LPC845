/**
 * @file HPL_CTIMER.h
 * @brief Definiciones a nivel de abstraccion del periferico CTIMER (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HPL_CTIMER_H_
#define HPL_CTIMER_H_

#include <stdint.h>

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
 * @brief Inicializacion del CTIMER
 * @param prescaler Prescaler deseado
 */
void CTIMER_init(uint32_t prescaler);

/**
 * @brief Frenar el conteo
 */
void CTIMER_stop(void);

/**
 * @brief Arrancar el conteo
 */
void CTIMER_run(void);

/**
 * @brief Configuracion de los registros del CTIMER correspnodientes a los match registers
 * @param mr_number Numero de match a configurar
 * @param mr_config Configuracion de los match registers
 */
void CTIMER_config_mr(uint8_t mr_number, const CTIMER_MR_config_t *mr_config);

/**
 * @brief Actualizar un match register de inmediato (sin esperar a que reinicie el conteo)
 * @param mr_number Numero de match a actualizar
 * @param mr_value Valor de match
 */
void CTIMER_update_mr_value(uint8_t mr_number, uint32_t mr_value);

/**
 * @brief Actualizar un match register cuando se reinicie el conteo
 * @param mr_number Numero de match a actualizar
 * @param mr_value Valor de match
 */
void CTIMER_update_mr_value_on_finish(uint8_t mr_number, uint32_t mr_value);

/**
 * @brief Configuracion de los registros del CTIMER correspnodientes a los capture registers
 * @param cc_number Numero de capture a configurar
 * @param cc_config Configuracion de los capture registers
 */
void CTIMER_config_cc(uint8_t cc_number, const CTIMER_CC_config_t *cc_config);

/**
 * @brief Configuracion de los registros del CTIMER correspnodientes al control del conteo
 * @param ctcr_config Configuracion del capture control register
 */
void CTIMER_config_ctcr(const CTIMER_CTCR_config_t *ctcr_config);

/**
 * @brief Configuracion de los registros del CTIMER correspondientes al match externo
 * @param emc_number Numero de EMC a configurar
 * @param emc_config Configuracion del external match control register
 */
void CTIMER_config_emr(uint8_t emc_number, const CTIMER_EMR_config_t *emc_config);

/**
 * @brief Habilitacion de un canal de PWM
 */
void CTIMER_enable_pwm(uint8_t pwm_number);

/**
 * @brief Inhabilitacion de un canal de PWM
 */
void CTIMER_disable_pwm(uint8_t pwm_number);

#endif /* HPL_CTIMER_H_ */
