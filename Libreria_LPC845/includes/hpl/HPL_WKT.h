/**
 * @file HPL_WKT.h
 * @brief Declaraciones a nivel de abstraccion de periferico del WKT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HPL_WKT_H_
#define HPL_WKT_H_

#include <HRI_WKT.h>

extern volatile WKT_per_t * const WKT; //!< Periferico WKT

typedef enum
{
	WKT_CLOCK_SOURCE_DIVIDED_FRO = 0,
	WKT_CLOCK_SOURCE_LOW_POWER_CLOCK
}WKT_clock_source_sel_en;

/**
 * @brief Seleccionar la fuente de clock para el WKT
 * @param[in] clock_source Fuente de clock deseada
 */
static inline void WKT_select_clock_source(WKT_clock_source_sel_en clock_source)
{
	WKT->CTRL.CLKSEL = clock_source;
}

/**
 * @brief Obtener flag de alarma actual
 * @return Estado del flag de alarma actual
 */
static inline uint8_t WKT_get_alarm_flag(void)
{
	return WKT->CTRL.ALARMFLAG;
}

/**
 * @brief Limpiar flag de alarma
 */
static inline void WKT_clear_alarm_flag(void)
{
	WKT->CTRL.ALARMFLAG = 1;
}

/**
 * @brief Limpiar el contador del WKT
 */
static inline void WKT_clear_count(void)
{
	WKT->CTRL.CLEARCTR = 1;
}

/**
 * @brief Seleccionar fuente de clock interna para el WKT
 */
static inline void WKT_set_internal_clock_source(void)
{
	WKT->CTRL.SEL_EXTCLK = 0;
}

/**
 * @brief Seleccionar fuente de clock externa para el WKT
 */
static inline void WKT_set_external_clock_source(void)
{
	WKT->CTRL.SEL_EXTCLK = 1;
}

/**
 * @brief Obtener cuenta actual del WKT
 * @return Cuenta actual del WKT
 */
static inline uint32_t WKT_get_current_count(void)
{
	return WKT->COUNT.VALUE;
}

/**
 * @brief Fijar la cuenta del WKT
 * @param[in] count Valor de cuenta deseado
 */
static inline void WKT_write_count(uint32_t count)
{
	WKT->COUNT.VALUE = count;
}

#endif /* HPL_WKT_H_ */
