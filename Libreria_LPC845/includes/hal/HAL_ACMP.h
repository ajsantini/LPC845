/**
 * @file HAL_ACMP.h
 * @brief Declaraciones a nivel de aplicación del periférico ADC (LPC845)
 * @author Esteban E. Chiama
 * @date 4/2020
 * @version 1.0
 */

#ifndef HAL_ACMP_H_
#define HAL_ACMP_H_

/** Selección, o no, de sincronismo de salida del comparador con clock de bus.*/
typedef enum
{
	HAL_ACMP_OUTPUT_DIRECT = 0,
	HAL_ACMP_OUTPUT_SYNC,
}hal_acmp_output_control_en;

/** */
typedef enum
{
	HAL_ACMP_HYSTERESIS_NONE = 0,
	HAL_ACMP_HYSTERESIS_5mV,
	HAL_ACMP_HYSTERESIS_10mV,
	HAL_ACMP_HYSTERESIS_20mV
}hal_acmp_hysteresis_sel_en;

/** */
typedef enum
{
	HAL_ACMP_LADDER_VREF_VDD = 0,
	HAL_ACMP_LADDER_VREF_VDDCMP_PIN
}hal_acmp_ladder_vref_sel_en;

/** */
typedef enum
{
	HAL_ACMP_EDGE_FALLING = 0,
	HAL_ACMP_EDGE_RISING,
	HAL_ACMP_EDGE_BOTH
}hal_acmp_edge_sel_en;

/** */
typedef enum
{
	HAL_ACMP_INPUT_VOLTAGE_VLADDER_OUT = 0,
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I1,
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I2,
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I3,
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I4,
	HAL_ACMP_INPUT_VOLTAGE_ACMP_I5,
	HAL_ACMP_INPUT_VOLTAGE_BANDGAP,
	HAL_ACMP_INPUT_VOLTAGE_DACOUT0
}hal_acmp_input_voltage_sel_en;

/** */
typedef struct
{
	hal_acmp_output_control_en output_control;
	hal_acmp_hysteresis_sel_en hysteresis;
	uint8_t interrupt_enable;
	hal_acmp_edge_sel_en edge_sel;
}hal_acpm_config_t;

/** */
typedef struct
{
	uint8_t enable;
	hal_acmp_ladder_vref_sel_en vref_sel;
	uint8_t step;
}hal_acmp_ladder_config_t;

/**
 * @brief xxx
 * @param[in] param xxx
 * @see xxx
 */
void hal_acmp_init(void);

/**
 * @brief xxx
 * @param[in] param xxx
 * @see xxx
 */
void hal_acmp_deinit(void);

/**
 * @brief xxx
 * @param[in] param xxx
 * @see xxx
 */
void hal_acmp_config(const hal_acpm_config_t *acmp_config);

/**
 * @brief xxx
 * @param[in] param xxx
 * @see xxx
 */
void hal_acmp_ladder_config(const hal_acmp_ladder_config_t *config);

/**
 * @brief xxx
 * @param[in] param xxx
 * @see xxx
 */
void hal_acmp_input_select(hal_acmp_input_voltage_sel_en positive_input, hal_acmp_input_voltage_sel_en negative_input);

#endif /* HAL_ACMP_H_ */
