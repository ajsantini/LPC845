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
