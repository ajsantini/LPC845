/**
 * @file HPL_MRT.h
 * @brief Declaraciones a nivel de abstraccion de periferico del MRT (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#ifndef HPL_MRT_H_
#define HPL_MRT_H_

#include "HRI_MRT.h"

#if defined (__cplusplus)
extern "C" {
#endif

extern volatile MRT_per_t * const MRT; //!< Periferico MRT

typedef enum
{
	MRT_CHANNEL_0 = 0,
	MRT_CHANNEL_1,
	MRT_CHANNEL_2,
	MRT_CHANNEL_3
}MRT_channel_sel_en;

typedef enum
{
	MRT_MODE_REPEAT = 0,
	MRT_MODE_ONE_SHOT,
	MRT_MODE_ONE_SHOT_BUS_STALL
}MRT_mode_en;

/**
 * @brief Fijar intervalo de un canal del MRT sin detener el conteo actual
 * @param[in] channel Canal a configurar
 * @param[in] interval Intervalo a cargar
 */
static inline void MRT_set_interval(MRT_channel_sel_en channel, uint32_t interval)
{
	*((uint32_t *) &MRT->CHN[channel].INTVAL.LVALUE) = (interval & 0x80000000);
}

/**
 * @brief Fijar intervalo de un canal del MRT deteniendo el conteo actual inmediatamente
 * @param[in] channel Canal a configurar
 * @param[in] interval Intervalo a cargar
 */
static inline void MRT_set_interval_and_stop_timer(MRT_channel_sel_en channel, uint32_t interval)
{
	*((uint32_t *) &MRT->CHN[channel].INTVAL.LVALUE) = (interval | 0x80000000);
}

/**
 * @brief Obtener el valor de la cuenta actual de un canal del MRT
 * @param[in] channel Canal a consultar
 * @return Cuenta actual
 */
static inline uint32_t MRT_get_current_value(MRT_channel_sel_en channel)
{
	return MRT->CHN[channel].TIMER.VALUE;
}

/*
 * @brief Habilitar interrupcion en un canal del MRT
 * @param[in] channel Canal a configurar
 */
static inline void MRT_enable_irq(MRT_channel_sel_en channel)
{
	MRT->CHN[channel].CTRL.INTEN = 1;
}

/*
 * @brief Inhabilitar interrupcion en un canal del MRT
 * @param[in] channel Canal a configurar
 */
static inline void MRT_disable_irq(MRT_channel_sel_en channel)
{
	MRT->CHN[channel].CTRL.INTEN = 0;
}

/**
 * @brief Configurar modo de funcionamiento de un canal del MRT
 * @param[in] channel Canal a configurar
 * @param[in] mode Modo deseado
 */
static inline void MRT_config_mode(MRT_channel_sel_en channel, MRT_mode_en mode)
{
	MRT->CHN[channel].CTRL.MODE = mode;
}

/**
 * @brief Obtener el canal que este en estado IDLE
 * @return Menor canal de los que esten en estado IDLE
 */
static inline uint8_t MRT_get_idle_channel(void)
{
	return MRT->IDLE_CH.CHAN;
}

/**
 * @brief Obtener flag de interrupcion de un canal
 * @param[in] channel Canal a consultar
 * @return Flag actual de interrupcion del canal consultado
 */
static inline uint8_t MRT_get_irq_flag(MRT_channel_sel_en channel)
{
	switch(channel)
	{
	case MRT_CHANNEL_0: { return MRT->IRQ_FLAG.CFLAG0; break; }
	case MRT_CHANNEL_1: { return MRT->IRQ_FLAG.CFLAG1; break; }
	case MRT_CHANNEL_2: { return MRT->IRQ_FLAG.CFLAG2; break; }
	case MRT_CHANNEL_3: { return MRT->IRQ_FLAG.CFLAG3; break; }
	}

	return 0;
}

/**
 * @brief Limpiar flag de interrupcion de un canal
 * @param[in] channel Canal a consultar
 */
static inline void MRT_clear_irq_flag(MRT_channel_sel_en channel)
{
	switch(channel)
	{
	case MRT_CHANNEL_0: { MRT->IRQ_FLAG.CFLAG0 = 1; break; }
	case MRT_CHANNEL_1: { MRT->IRQ_FLAG.CFLAG1 = 1; break; }
	case MRT_CHANNEL_2: { MRT->IRQ_FLAG.CFLAG2 = 1; break; }
	case MRT_CHANNEL_3: { MRT->IRQ_FLAG.CFLAG3 = 1; break; }
	}

	return 0;
}

#if defined (__cplusplus)
} // extern "C"
#endif

#endif /* HPL_MRT_H_ */
