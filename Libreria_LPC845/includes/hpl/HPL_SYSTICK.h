/**
 * @file HPL_SYSTICK.h
 * @brief Declaraciones a nivel de abstraccion de periferico del SYSTICK (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_SYSTICK_H_
#define HPL_SYSTICK_H_

#include "HRI_SYSTICK.h"

#if defined (__cplusplus)
extern "C" {
#endif

extern volatile SYSTICK_reg_t * const SYSTICK; //!< Periferico SYSTICK

typedef enum
{
	SYSTICK_CLOCK_SOURCE_MAIN_CLOCK_2 = 0,
	SYSTICK_CLOCK_SOURCE_MAIN_CLOCK
}SYSTICK_clock_source_en;

/*
 * @brief Habilitar el conteo
 */
static inline void SYSTICK_enable_count(void)
{
	SYSTICK->CSR.ENABLE = 1;
}

/*
 * @brief Inhabilitar el conteo
 */
static inline void SYSTICK_disable_count(void)
{
	SYSTICK->CSR.ENABLE = 0;
}

/*
 * @brief Habilitar interrupcion de tick
 */
static inline void SYSTICK_enable_interrupt(void)
{
	SYSTICK->CSR.TICKINT = 1;
}

/*
 * @brief Inhabilitar interrupcion de tick
 */
static inline void SYSTICK_disable_interrupt(void)
{
	SYSTICK->CSR.TICKINT = 0;
}

/**
 * @brief Seleccion de fuente de clock
 * @param[in] clock_source Fuente deseada
 */
static inline void SYSTICK_select_clock_source(SYSTICK_clock_source_en clock_source)
{
	SYSTICK->CSR.CLKSOURCE = clock_source;
}

/**
 * @brief Obtener flag de conteo terminado
 * @return Si el SYSTICK habia terminado la cuenta antes de leer el registro, devuelve 1
 */
static inline uint8_t SYSTICK_get_count_flag(void)
{
	return SYSTICK->CSR.COUNTFLAG;
}

/**
 * @brief Fijar el valor de reload
 */
static inline void SYSTICK_set_reload(uint32_t reload)
{
	/*
	 * Error de implementacion, la hoja de datos aclara que no hay que escribir unos en la parte
	 * superior del registro, por lo que en el reset puede arrancar con valores raros y llevar a
	 * un mal funcionamiento del periferico.
	 */
	*((uint32_t *)(&SYSTICK->RVR)) = 0;
	SYSTICK->RVR.RELOAD = reload;
}

/**
 * @brief Limpiar el conteo actual
 */
static inline void SYSTICK_set_clear_current_value(void)
{
	SYSTICK->CVR.CURRENT = 1;
}

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HPL_SYSTICK_H_ */
