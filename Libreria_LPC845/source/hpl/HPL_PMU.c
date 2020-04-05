/**
 * @file HPL_PMU.c
 * @brief Funciones a nivel de abstraccion de periferico para el PMU (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_PMU.h>

volatile SCR_reg_t * const SCR = (volatile SCR_reg_t *) SCR_REG_BASE; //!< Registro SCR

volatile PMU_per_t * const PMU = (volatile PMU_per_t *) PMU_BASE; //!< Periferico PMU

