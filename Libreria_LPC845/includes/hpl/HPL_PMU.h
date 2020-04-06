/**
 * @file HPL_PMU.h
 * @brief Declaraciones a nivel de abstraccion de periferico del PMU (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#ifndef HPL_PMU_H_
#define HPL_PMU_H_

#include <HRI_PMU.h>

extern volatile SCR_reg_t * const SCR; //!< Registro SCR

extern volatile PMU_per_t * const PMU; //!< Periferico PMU

typedef enum
{
	PMU_SLEEP_MODE_SLEEP = 0,
	PMU_SLEEP_MODE_DEEP_SLEEP
}PMU_sleep_mode_en;

typedef enum
{
	PMU_POWER_MODE_DEFAULT = 0,
	PMU_POWER_MODE_DEEP_SLEEP,
	PMU_POWER_MODE_POWER_DOWN,
	PMU_POWER_MODE_DEEP_POWER_DOWN
}PMU_power_mode_en;

typedef enum
{
	PMU_GENERAL_PURPOUSE_REGISTER_0 = 0,
	PMU_GENERAL_PURPOUSE_REGISTER_1,
	PMU_GENERAL_PURPOUSE_REGISTER_2,
	PMU_GENERAL_PURPOUSE_REGISTER_3
}PMU_general_purpouse_regiter_en;

/**
 * @brief Activar funcion sleep on exit
 */
static inline void PMU_set_sleep_on_exit(void)
{
	SCR->SLEEPONEXIT = 1;
}

/**
 * @brief Desactivar funcion sleep on exit
 */
static inline void PMU_clear_sleep_on_exit(void)
{
	SCR->SLEEPONEXIT = 0;
}

/**
 * @brief Configurar profundidad del sleep
 * @param[in] sleep_mode Profundidad del sleep deseada
 */
static inline void PMU_config_sleep_mode(PMU_sleep_mode_en sleep_mode)
{
	SCR->SLEEPDEEP = sleep_mode;
}

/*
 * @brief Habilitar event on pending bit
 */
static inline void PMU_set_send_event_on_pending_bit(void)
{
	SCR->SEVONPEND = 1;
}

/*
 * @brief Inhabilitar event on pending bit
 */
static inline void PMU_clear_send_event_on_pending_bit(void)
{
	SCR->SEVONPEND = 0;
}

/**
 * @brief Configurar modo de energia en WFI
 * @param[in] power_mode Modo de energia deseado
 */
static inline void PMU_config_power_mode(PMU_power_mode_en power_mode)
{
	PMU->PCON.PM = power_mode;
}

/**
 * @brief Inhabilitar deep power-down mode en WFI
 *
 * Inhabilita el deep power-down mode, aunque este configurado en ese modo, hasta el proximo reset
 */
static inline void PMU_set_prevent_deep_power(void)
{
	PMU->PCON.NODPD = 1;
}

/*
 * @brief Obtener el estado del flag SLEEP
 * @return Estado actual del flag
 */
static inline uint8_t PMU_get_sleep_flag(void)
{
	return PMU->PCON.SLEEPFLAG;
}

/*
 * @brief Limpiar el flag SLEEP
 */
static inline void PMU_clear_sleep_flag(void)
{
	PMU->PCON.SLEEPFLAG = 1;
}

/*
 * @brief Obtener el estado del flag deep power-down
 * @return Estado actual del flag
 */
static inline uint8_t PMU_get_deep_power_down_flag(void)
{
	return PMU->PCON.DPDFLAG;
}

/*
 * @brief Limpiar el flag deep power-down
 */
static inline void PMU_clear_deep_power_down_flag(void)
{
	PMU->PCON.DPDFLAG = 1;
}

/**
 * @brief Escibir un dato en un registro de proposito general
 * @param[in] reg Numero de registro a escribir
 * @param[in] data Dato a escribir en el registro de proposito general
 */
static inline void PMU_write_general_purpouse_register(PMU_general_purpouse_regiter_en reg, uint32_t data)
{
	PMU->GPREG[reg].GPDATA = data;
}

/**
 * @brief Leer un dato en un registro de proposito general
 * @param[in] reg Numero de registro a escribir
 * @return Dato en el registro de proposito general
 */
static inline uint32_t PMU_read_general_purpouse_register(PMU_general_purpouse_regiter_en reg)
{
	return PMU->GPREG[reg].GPDATA;
}

/**
 * @brief Habilitar histeresis en el pin wake up
 */
static inline void PMU_enable_wake_up_pin_hysteresis(void)
{
	PMU->DPDCTRL.WAKEUPHYS = 1;
}

/**
 * @brief Inhabilitar histeresis en el pin wake up
 */
static inline void PMU_disable_wake_up_pin_hysteresis(void)
{
	PMU->DPDCTRL.WAKEUPHYS = 0;
}

/**
 * @brief Habilitar wake up pin
 */
static inline void PMU_enable_wake_up_pin(void)
{
	PMU->DPDCTRL.WAKEPAD_DISABLE = 0;
}

/**
 * @brief Inhabilitar wake up pin
 */
static inline void PMU_disable_wake_up_pin(void)
{
	PMU->DPDCTRL.WAKEPAD_DISABLE = 1;
}

/**
 * @brief Habilitar low-power oscillator
 */
static inline void PMU_enable_low_power_oscillator(void)
{
	PMU->DPDCTRL.LPOSCEN = 1;
}

/**
 * @brief Inhabilitar low-power oscillator
 */
static inline void PMU_disable_low_power_oscillator(void)
{
	PMU->DPDCTRL.LPOSCEN = 0;
}

/**
 * @brief Habilitar low-power oscillator en deep power-down mode
 */
static inline void PMU_enable_low_power_oscillator_in_dpdmode(void)
{
	PMU->DPDCTRL.LPOSCDPDEN = 1;
}

/**
 * @brief Inhabilitar low-power oscillator en deep power-down mode
 */
static inline void PMU_disable_low_power_oscillator_in_dpdmode(void)
{
	PMU->DPDCTRL.LPOSCDPDEN = 0;
}

/*
 * @brief Habilitar histeresis en el pin de clock externo de wake up
 */
static inline void PMU_enable_wake_up_clock_hysteresis(void)
{
	PMU->DPDCTRL.WAKEUPCLKHYS = 1;
}

/*
 * @brief Inhabilitar histeresis en el pin de clock externo de wake up
 */
static inline void PMU_disable_wake_up_clock_hysteresis(void)
{
	PMU->DPDCTRL.WAKEUPCLKHYS = 0;
}

/**
 * @brief Habilitar el pin del clock externo de wake up
 */
static inline void PMU_enable_wake_up_clock_pin(void)
{
	PMU->DPDCTRL.WAKECLKPAD_DISABLE = 0;
}

/**
 * @brief Inhabilitar el pin del clock externo de wake up
 */
static inline void PMU_disable_wake_up_clock_pin(void)
{
	PMU->DPDCTRL.WAKECLKPAD_DISABLE = 1;
}

/**
 * @brief Habilitar histeresis en el pin de reset
 */
static inline void PMU_enable_reset_hysteresis(void)
{
	PMU->DPDCTRL.RESETHYS = 1;
}

/**
 * @brief Inhabilitar histeresis en el pin de reset
 */
static inline void PMU_disable_reset_hysteresis(void)
{
	PMU->DPDCTRL.RESETHYS = 0;
}

/**
 * @brief Habilitar funcion de reset en el pin
 */
static inline void PMU_enable_reset(void)
{
	PMU->DPDCTRL.RESET_DISABLE = 0;
}

/**
 * @brief Inhabilitar funcion de reset en el pin
 */
static inline void PMU_disable_reset(void)
{
	PMU->DPDCTRL.RESET_DISABLE = 1;
}

#endif /* HPL_PMU_H_ */
