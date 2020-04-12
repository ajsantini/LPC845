/**
 * @file HRI_PMU.c
 * @brief Declaración del periférico PMU (LPC845)
 * @author Augusto Santini
 * @date 4/2020
 * @version 1.0
 */

#include <HRI_PMU.h>

volatile SCR_reg_t * const SCR = (volatile SCR_reg_t *) SCR_REG_BASE; //!< Registro SCR

volatile PMU_per_t * const PMU = (volatile PMU_per_t *) PMU_BASE; //!< Periferico PMU

