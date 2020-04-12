/**
 * @file HAL_WKT.c
 * @brief Funciones a nivel de aplicacion del periferico WKT (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <stddef.h>
#include <HAL_WKT.h>
#include <HAL_SYSCON.h>
#include <HPL_WKT.h>
#include <HPL_NVIC.h>
#include <HPL_SYSCON.h>
#include <HPL_PMU.h>

#define		HAL_WKT_DIVIDE_VALUE		(16)
#define		HAL_WKT_LOW_POWER_OSC_FREQ	(10e3)

static hal_wkt_clock_source_en current_clock_source = HAL_WKT_CLOCK_SOURCE_FRO_DIV;
static uint32_t current_ext_clock = 0;

static void dummy_irq(void);

static void (*hal_wkt_irq_callback)(void) = dummy_irq;

/**
 * @brief Inicializar el WKT
 * @param[in] clock_sel Seleccion de clock deseada para el WKT
 * @param[in] ext_clock_value Valor de clock externo (si la seleccion es interna, no importa este parametro)
 * @param[in] callback Callback a ejecutar en la interrupcion del WKT
 */
void hal_wkt_init(hal_wkt_clock_source_en clock_sel, uint32_t ext_clock_value, void (*callback)(void))
{
	SYSCON_enable_clock(SYSCON_ENABLE_CLOCK_SEL_WKT);

	SYSCON_clear_reset(SYSCON_RESET_SEL_WKT);

	hal_wkt_select_clock_source(clock_sel, ext_clock_value);
	current_ext_clock = ext_clock_value;

	hal_wkt_register_callback(callback);
}

/*
 * @brief Configurar fuente de clock para el WKT
 * @param[in] clock_sel Seleccion de clock deseada para el WKT
 * @param[in] ext_clock_value Valor de clock externo (si la seleccion es interna, no importa este parametro)
 */
void hal_wkt_select_clock_source(hal_wkt_clock_source_en clock_sel, uint32_t ext_clock_value)
{
	switch(clock_sel)
	{
	case HAL_WKT_CLOCK_SOURCE_FRO_DIV:
	{
		PMU_disable_wake_up_clock_pin();
		WKT_set_internal_clock_source();
		WKT_select_clock_source(WKT_CLOCK_SOURCE_DIVIDED_FRO);

		break;
	}

	case HAL_WKT_CLOCK_SOURCE_LOW_POWER_OSC:
	{
		PMU_disable_wake_up_clock_pin();
		WKT_set_internal_clock_source();
		WKT_select_clock_source(WKT_CLOCK_SOURCE_LOW_POWER_CLOCK);

		break;
	}

	case HAL_WKT_CLOCK_SOURCE_EXTERNAL:
	{
		PMU_enable_wake_up_clock_pin();
		current_ext_clock = ext_clock_value;
		WKT_set_external_clock_source();

		break;
	}
	default: break;
	}
}

/**
 * @brief Registrar un callback para la interrupcion del WKT
 * @param[in] new_callback Nuevo callback para la interrupcion del WKT
 */
void hal_wkt_register_callback(void (*new_callback)(void))
{
	if(new_callback == NULL)
	{
		hal_wkt_irq_callback = dummy_irq;
		NVIC_disable_interrupt(NVIC_IRQ_SEL_WKT);
	}
	else
	{
		hal_wkt_irq_callback = new_callback;
		NVIC_clear_pending_interrupt(NVIC_IRQ_SEL_WKT);
		NVIC_enable_interrupt(NVIC_IRQ_SEL_WKT);
	}
}

/*
 * @brief Iniciar el conteo con el WKT en base a un tiempo
 * @param[in] time_useg Tiempo en microsegundos deseado (se redondeara al valor primer posible hacia arriba)
 */
void hal_wkt_start_count(uint32_t time_useg)
{
	float clock_base_value;
	uint32_t calculated_count;

	switch(current_clock_source)
	{
	case HAL_WKT_CLOCK_SOURCE_FRO_DIV: { clock_base_value = (float) (hal_syscon_fro_clock_get() / HAL_WKT_DIVIDE_VALUE); break; }
	case HAL_WKT_CLOCK_SOURCE_LOW_POWER_OSC: { clock_base_value = (float) (HAL_WKT_LOW_POWER_OSC_FREQ); break; }
	case HAL_WKT_CLOCK_SOURCE_EXTERNAL: { clock_base_value = (float) (current_ext_clock); break; }
	}

	calculated_count = (uint32_t) ((clock_base_value * time_useg) / 1e6);

	WKT_write_count(calculated_count);
}

/*
 * @brief Iniciar el conteo con el WKT en base a un valor
 *
 * El usuario es responsable de colocar un valor que tenga sentido en base al clock utilizado.
 *
 * @param[in] value Valor deseado a poner en el conteo (util para una actualizacion mas rapida)
 */
void hal_wkt_start_count_with_value(uint32_t value)
{
	WKT_write_count(value);
}

/*
 * @brief Funcion dummy para inicializar punteros a funcion de interrupcion
 */
static void dummy_irq(void)
{
	return;
}

/**
 * @brief Interrupcion de WKT
 */
void WKT_IRQHandler(void)
{
	hal_wkt_irq_callback();

	WKT_clear_alarm_flag();
}
