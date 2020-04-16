/**
 * @file HPL_PININT.h
 * @brief Declaraciones a nivel de abstraccion de periferico del PININT (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_PININT_H_
#define HPL_PININT_H_

#include <HRI_PININT.h>

extern volatile PININT_per_t * const PININT; //!< Periferico PININT

typedef enum
{
	PININT_INTERRUPT_MODE_EDGE = 0,
	PININT_INTERRUPT_MODE_LEVEL
}PININT_interrupt_mode_en;

typedef enum
{
	PININT_MATCH_CONTRIBUTION_CONSTANT_HIGH = 0,
	PININT_MATCH_CONTRIBUTION_STICKY_RISING_EDGE,
	PININT_MATCH_CONTRIBUTION_STICKY_FALLING_EDGE,
	PININT_MATCH_CONTRIBUTION_STICKY_RISING_OR_FALLING_EDGE,
	PININT_MATCH_CONTRIBUTION_HIGH_LEVEL,
	PININT_MATCH_CONTRIBUTION_LOW_LEVEL,
	PININT_MATCH_CONTRIBUTION_CONSTANT_0,
	PININT_MATCH_CONTRIBUTION_EVENT
}PININT_match_contribution_en;

/**
 * @brief Configurar sensibilidad a nivel/flanco
 * @param[in] channel Canal a configurar
 * @param[in] mode Modo deseado
 */
static inline void PININT_set_interrupt_mode(uint8_t channel, PININT_interrupt_mode_en mode)
{
	PININT->ISEL.PMODE &= ~(mode << channel);
	PININT->ISEL.PMODE |= (mode << channel);
}

/**
 * @brief Obtener configuracion de modo de un canal
 * @param[in] channel Canal a consultar
 * @return Modo configurado para el canal
 */
static inline PININT_interrupt_mode_en PININT_get_interrupt_mode(uint8_t channel)
{
	return (PININT->ISEL.PMODE & (1 << channel)) >> channel;
}

/**
 * @brief Habilitar detecciones por flanco ascendente
 * @param[in] channel Canal deseado
 */
static inline void PININT_enable_rising_edge(uint8_t channel)
{
	PININT->SIENR.SETENRL |= (1 << channel);
}

/**
 * @brief Inhabilitar detecciones por flanco ascendente
 * @param[in] channel Canal deseado
 */
static inline void PININT_disable_rising_edge(uint8_t channel)
{
	PININT->CIENR.CENRL |= (1 << channel);
}

/**
 * @brief Habilitar detecciones por flanco descendente
 * @param[in] channel Canal deseado
 */
static inline void PININT_enable_falling_edge(uint8_t channel)
{
	PININT->SIENF.SETENAF |= (1 << channel);
}

/**
 * @brief Inhabilitar detecciones por flanco descendente
 * @param[in] channel Canal deseado
 */
static inline void PININT_disable_falling_edge(uint8_t channel)
{
	PININT->CIENF.CENAF |= (1 << channel);
}

/**
 * @brief Habilitar detecciones por nivel
 * @param[in] channel Canal deseado
 */
static inline void PININT_enable_level_detections(uint8_t channel)
{
	PININT->IENR.ENRL |= (1 << channel);
}

/**
 * @brief Inhabilitar detecciones por nivel
 * @param[in] channel Canal deseado
 */
static inline void PININT_disable_level_detections(uint8_t channel)
{
	PININT->CIENR.CENRL |= (1 << channel);
}

/**
 * @brief Habilitar detecciones por nivel alto
 * @param[in] channel Canal deseado
 */
static inline void PININT_enable_high_level(uint8_t channel)
{
	PININT->SIENF.SETENAF |= (1 << channel);
}

/**
 * @brief Inhabilitar detecciones por flanco ascendente
 * @param[in] channel Canal deseado
 */
static inline void PININT_enable_low_level(uint8_t channel)
{
	PININT->CIENF.CENAF |= (1 << channel);
}

/**
 * @brief Obtener interrupciones activas por flanco ascendente
 * @return Mascara de bits con los canales activos
 */
static inline uint8_t PININT_get_rising_edge_active(void)
{
	return PININT->RISE.RDET;
}

/**
 * @brief Obtener interrupciones activas por flanco descendente
 * @return Mascara de bits con los canales activos
 */
static inline uint8_t PININT_get_falling_edge_active(void)
{
	return PININT->FALL.FDET;
}

/**
 * @brief Obtener interrupciones activas por nivel
 * @return Mascara de bits con los canales activos
 */
static inline uint8_t PININT_get_level_active(void)
{
	return PININT->IST.PSTAT;
}

/**
 * @brief Limpiar flag de interrupcion por flanco
 * @param[in] channel Canal de interrupcion a limpiar
 */
static inline void PININT_clear_edge_level_irq(uint8_t channel)
{
	PININT->IST.PSTAT |= (1 << channel);
}

/**
 * @brief Invertir nivel activo de itnerrupcion por nivel
 * @param[in] channel Canal a invertir
 */
static inline void PININT_toggle_active_level(uint8_t channel)
{
	PININT->IST.PSTAT |= (1 << channel);
}

/**
 * @brief Habilita el funcionamiento del pattern match engine (inhabilita PININT)
 */
static inline void PININT_enable_pattern_match(void)
{
	PININT->PMCTRL.SEL_PMATCH = 1;
}

/**
 * @brief Inhabilita el funcionamiento del pattern match engine (habilita PININT)
 */
static inline void PININT_disable_pattern_match(void)
{
	PININT->PMCTRL.SEL_PMATCH = 0;
}

/**
 * @brief Habilita la salida de RXEV
 */
static inline void PININT_enable_RXEV(void)
{
	PININT->PMCTRL.ENA_RXEV = 1;
}

/**
 * @brief Inhabilita la salida de RXEV
 */
static inline void PININT_disable_RXEV(void)
{
	PININT->PMCTRL.ENA_RXEV = 0;
}

/**
 * @brief Obtener estado actual del pattern match
 * @return Mascara de bits indicando que miniterminos estan activos
 */
static inline uint8_t PININT_get_pattern_match_state(void)
{
	return PININT->PMCTRL.PMAT;
}

/**
 * @brief Configuracion de las fuentes del pattern match
 * @param[in] channel Entrada a configurar
 * @param[in] slice Slice a configurar
 */
static inline void PININT_config_pattern_match_source(uint8_t channel, uint8_t slice)
{
	switch(slice)
	{
	case 0: { PININT->PMSRC.SRC0 = channel; break; }
	case 1: { PININT->PMSRC.SRC1 = channel; break; }
	case 2: { PININT->PMSRC.SRC2 = channel; break; }
	case 3: { PININT->PMSRC.SRC3 = channel; break; }
	case 4: { PININT->PMSRC.SRC4 = channel; break; }
	case 5: { PININT->PMSRC.SRC5 = channel; break; }
	case 6: { PININT->PMSRC.SRC6 = channel; break; }
	case 7: { PININT->PMSRC.SRC7 = channel; break; }
	}
}

/**
 * @brief Habilitar slice como endpoint
 * @param[in] slice Slice a configurar
 */
static inline void PINITN_enable_slice_as_endpoint(uint8_t slice)
{
	switch(slice)
	{
	case 0: { PININT->PMCFG.PROD_ENDPTS0 = 1; break; }
	case 1: { PININT->PMCFG.PROD_ENDPTS1 = 1; break; }
	case 2: { PININT->PMCFG.PROD_ENDPTS2 = 1; break; }
	case 3: { PININT->PMCFG.PROD_ENDPTS3 = 1; break; }
	case 4: { PININT->PMCFG.PROD_ENDPTS4 = 1; break; }
	case 5: { PININT->PMCFG.PROD_ENDPTS5 = 1; break; }
	case 6: { PININT->PMCFG.PROD_ENDPTS6 = 1; break; }
	}
}

/**
 * @brief Inhabilitar slice como endpoint
 * @param[in] slice Slice a configurar
 */
static inline void PINITN_disable_slice_as_endpoint(uint8_t slice)
{
	switch(slice)
	{
	case 0: { PININT->PMCFG.PROD_ENDPTS0 = 0; break; }
	case 1: { PININT->PMCFG.PROD_ENDPTS1 = 0; break; }
	case 2: { PININT->PMCFG.PROD_ENDPTS2 = 0; break; }
	case 3: { PININT->PMCFG.PROD_ENDPTS3 = 0; break; }
	case 4: { PININT->PMCFG.PROD_ENDPTS4 = 0; break; }
	case 5: { PININT->PMCFG.PROD_ENDPTS5 = 0; break; }
	case 6: { PININT->PMCFG.PROD_ENDPTS6 = 0; break; }
	}
}

/**
 * @brief Configurar modo del slice
 * @param[in] slice Slice a configurar
 */
static inline void PINITN_config_slice_mode(uint8_t slice, PININT_match_contribution_en mode)
{
	switch(slice)
	{
	case 0: { PININT->PMCFG.CFG0 = mode; break; }
	case 1: { PININT->PMCFG.CFG1 = mode; break; }
	case 2: { PININT->PMCFG.CFG2 = mode; break; }
	case 3: { PININT->PMCFG.CFG3 = mode; break; }
	case 4: { PININT->PMCFG.CFG4 = mode; break; }
	case 5: { PININT->PMCFG.CFG5 = mode; break; }
	case 6: { PININT->PMCFG.CFG6 = mode; break; }
	}
}

#endif /* HPL_PININT_H_ */
