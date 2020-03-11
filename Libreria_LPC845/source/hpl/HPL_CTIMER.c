/**
 * @file HPL_CTIMER.c
 * @brief Funciones a nivel de abstraccion del periferico CTIMER (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <stdio.h>

#include <HRI_CTIMER.h>
#include <HRI_SWM.h>
#include <HRI_NVIC.h>
#include <HRI_SYSCON.h>

#include <HPL_CTIMER.h>
#include <HPL_SWM.h>

#define		MATCH_AMOUNT		4
#define		CAPTURE_AMOUNT		4

volatile CTIMER_per_t * const CTIMER = (volatile CTIMER_per_t *) CTIMER_BASE; //!< Periferico CTIMER

void (*match_callbacks[MATCH_AMOUNT])(void) = //!< Callbacks para interrupciones de match
{
	NULL,
	NULL,
	NULL,
	NULL
};

void (*capture_callbacks[CAPTURE_AMOUNT])(void) = //!< Callbacks para interrupciones de capture
{
	NULL,
	NULL,
	NULL,
	NULL
};

/**
 * @brief Inicializacion del CTIMER
 * @param prescaler Prescaler deseado
 */
void CTIMER_init(uint32_t prescaler)
{
	// Habilitacion en el SYSAHBCLKCTRL register
	SYSCON->SYSAHBCLKCTRL0.CTIMER0 = 1;

	// Clear reset del CTIMER
	SYSCON->PRESETCTRL0.CTIMER0_RST_N = 1;

	// Stop el counter
	CTIMER_stop();

	CTIMER->PR.PRVAL = prescaler;
}

/**
 * @brief Frenar el conteo
 */
void CTIMER_stop(void)
{
	CTIMER->TCR.CRST = 1;
	CTIMER->TCR.CEN = 0;
}

/**
 * @brief Arrancar el conteo
 */
void CTIMER_run(void)
{
	CTIMER->TCR.CRST = 0;
	asm("nop");
	CTIMER->TCR.CEN = 1;
}

/**
 * @brief Configuracion de los registros del CTIMER correspnodientes a los match registers
 * @param mr_number Numero de match a configurar
 * @param mr_config Configuracion de los match registers
 */
void CTIMER_config_mr(uint8_t mr_number, CTIMER_MR_config_t *mr_config)
{
	match_callbacks[mr_number] = mr_config->match_callback;

	if(mr_config->interrupt_on_match)
	{
		NVIC->ISER0.ISE_CT32B0 = 1;
	}

	switch(mr_number)
	{
	case 0:
	{
		CTIMER->MCR.MR0I = mr_config->interrupt_on_match;
		CTIMER->MCR.MR0R = mr_config->reset_on_match;
		CTIMER->MCR.MR0RL = mr_config->reload_on_match;
		CTIMER->MCR.MR0S = mr_config->stop_on_match;

		CTIMER->MR[0].MATCH = mr_config->match_value;

		break;
	}

	case 1:
	{
		CTIMER->MCR.MR1I = mr_config->interrupt_on_match;
		CTIMER->MCR.MR1R = mr_config->reset_on_match;
		CTIMER->MCR.MR1RL = mr_config->reload_on_match;
		CTIMER->MCR.MR1S = mr_config->stop_on_match;

		CTIMER->MR[1].MATCH = mr_config->match_value;

		break;
	}

	case 2:
	{
		CTIMER->MCR.MR2I = mr_config->interrupt_on_match;
		CTIMER->MCR.MR2R = mr_config->reset_on_match;
		CTIMER->MCR.MR2RL = mr_config->reload_on_match;
		CTIMER->MCR.MR2S = mr_config->stop_on_match;

		CTIMER->MR[2].MATCH = mr_config->match_value;

		break;
	}

	case 3:
	{
		CTIMER->MCR.MR3I = mr_config->interrupt_on_match;
		CTIMER->MCR.MR3R = mr_config->reset_on_match;
		CTIMER->MCR.MR3RL = mr_config->reload_on_match;
		CTIMER->MCR.MR3S = mr_config->stop_on_match;

		CTIMER->MR[3].MATCH = mr_config->match_value;

		break;
	}

	default: break;
	} // End switch(mr_number)
}

/**
 * @brief Actualizar un match register de inmediato (sin esperar a que reinicie el conteo)
 * @param mr_number Numero de match a actualizar
 * @param mr_value Valor de match
 */
void CTIMER_update_mr_value(uint8_t mr_number, uint32_t mr_value)
{
	CTIMER->MR[mr_number].MATCH = mr_value;

	switch(mr_number)
	{
	case 0:
	{
		CTIMER->MCR.MR0RL = 0;

		break;
	}

	case 1:
	{
		CTIMER->MCR.MR1RL = 0;

		break;
	}

	case 2:
	{
		CTIMER->MCR.MR2RL = 0;

		break;
	}

	case 3:
	{
		CTIMER->MCR.MR3RL = 0;

		break;
	}
	} // End switch(mr_number)
}

/**
 * @brief Actualizar un match register cuando se reinicie el conteo
 * @param mr_number Numero de match a actualizar
 * @param mr_value Valor de match
 */
void CTIMER_update_mr_value_on_finish(uint8_t mr_number, uint32_t mr_value)
{
	CTIMER->MSR[mr_number].SHADOW = mr_value;

	switch(mr_number)
	{
	case 0:
	{
		CTIMER->MCR.MR0RL = 1;

		break;
	}

	case 1:
	{
		CTIMER->MCR.MR1RL = 1;

		break;
	}

	case 2:
	{
		CTIMER->MCR.MR2RL = 1;

		break;
	}

	case 3:
	{
		CTIMER->MCR.MR3RL = 1;

		break;
	}
	} // End switch(mr_number)
}

/**
 * @brief Configuracion de los registros del CTIMER correspnodientes a los capture registers
 * @param cc_number Numero de capture a configurar
 * @param cc_config Configuracion de los capture registers
 */
void CTIMER_config_cc(uint8_t cc_number, CTIMER_CC_config_t *cc_config)
{
	capture_callbacks[cc_number] = cc_config->capture_callback;

	if(cc_config->interrupt_on_capture)
	{
		NVIC->ISER0.ISE_CT32B0 = 1;
	}

	switch(cc_number)
	{
	case 0:
	{
		CTIMER->CCR.CAP0FE = cc_config->falling_edge_capture;
		CTIMER->CCR.CAP0I = cc_config->interrupt_on_capture;
		CTIMER->CCR.CAP0RE = cc_config->rising_edge_capture;

		break;
	}

	case 1:
	{
		CTIMER->CCR.CAP1FE = cc_config->falling_edge_capture;
		CTIMER->CCR.CAP1I = cc_config->interrupt_on_capture;
		CTIMER->CCR.CAP1RE = cc_config->rising_edge_capture;

		break;
	}

	case 2:
	{
		CTIMER->CCR.CAP2FE = cc_config->falling_edge_capture;
		CTIMER->CCR.CAP2I = cc_config->interrupt_on_capture;
		CTIMER->CCR.CAP2RE = cc_config->rising_edge_capture;

		break;
	}

	case 3:
	{
		CTIMER->CCR.CAP3FE = cc_config->falling_edge_capture;
		CTIMER->CCR.CAP3I = cc_config->interrupt_on_capture;
		CTIMER->CCR.CAP3RE = cc_config->rising_edge_capture;

		break;
	}

	default: break;
	} // End switch(cc_number)
}

/**
 * @brief Configuracion de los registros del CTIMER correspnodientes al control del conteo
 * @param cc_config Configuracion de los capture registers
 */
void CTIMER_config_ctcr(CTIMER_CTCR_config_t *ctcr_config)
{
	CTIMER->CTCR.CTMODE = ctcr_config->CTMODE;
	CTIMER->CTCR.CINSEL = ctcr_config->CINSEL;
	CTIMER->CTCR.ENCC = ctcr_config->ENCC;
	CTIMER->CTCR.SELCC = ctcr_config->SELCC;
}

/**
 * @brief Configuracion de los registros del CTIMER correspondientes al match externo
 * @param emc_number Numero de EMC a configurar
 * @param emc_config Configuracion del external match control register
 */
void CTIMER_config_emr(uint8_t emr_number, CTIMER_EMR_config_t *emr_config)
{
	switch(emr_number)
	{
	case 0:
	{
		CTIMER->EMR.EMC0 = emr_config->EMC;

		if(emr_config->mat_enable)
		{
			SWM_init();

			SWM->PINASSIGN13.T0_MAT0 = (emr_config->mat_port * 32) + emr_config->mat_pin;

			SWM_deinit();
		}

		break;
	}

	case 1:
	{
		CTIMER->EMR.EMC1 = emr_config->EMC;

		if(emr_config->mat_enable)
		{
			SWM_init();

			SWM->PINASSIGN13.T0_MAT1 = (emr_config->mat_port * 32) + emr_config->mat_pin;

			SWM_deinit();
		}

		break;
	}

	case 2:
	{
		CTIMER->EMR.EMC2 = emr_config->EMC;

		if(emr_config->mat_enable)
		{
			SWM_init();

			SWM->PINASSIGN13.T0_MAT2 = (emr_config->mat_port * 32) + emr_config->mat_pin;

			SWM_deinit();
		}

		break;
	}

	case 3:
	{
		CTIMER->EMR.EMC3 = emr_config->EMC;

		if(emr_config->mat_enable)
		{
			SWM_init();

			SWM->PINASSIGN14.T0_MAT3 = (emr_config->mat_port * 32) + emr_config->mat_pin;

			SWM_deinit();
		}

		break;
	}

	default: break;
	} // End switch(emc_number)
}

/**
 * @brief Habilitacion de un canal de PWM
 */
void CTIMER_enable_pwm(uint8_t pwm_number)
{
	switch(pwm_number)
	{
	case 0:
	{
		CTIMER->PWMC.PWMEN0 = 1;

		break;
	}

	case 1:
	{
		CTIMER->PWMC.PWMEN1 = 1;

		break;
	}

	case 2:
	{
		CTIMER->PWMC.PWMEN2 = 1;

		break;
	}

	case 3:
	{
		CTIMER->PWMC.PWMEN3 = 1;

		break;
	}
	} // End switch(pwm_number)
}

/**
 * @brief Inhabilitacion de un canal de PWM
 */
void CTIMER_disable_pwm(uint8_t pwm_number)
{
	switch(pwm_number)
	{
	case 0:
	{
		CTIMER->PWMC.PWMEN0 = 0;

		break;
	}

	case 1:
	{
		CTIMER->PWMC.PWMEN1 = 0;

		break;
	}

	case 2:
	{
		CTIMER->PWMC.PWMEN2 = 0;

		break;
	}

	case 3:
	{
		CTIMER->PWMC.PWMEN3 = 0;

		break;
	}
	} // End switch(pwm_number)
}

/**
 * @brief Interrupcion de CTIMER
 */
void CTIMER0_IRQHandler(void)
{
	if(CTIMER->IR.MR0INT)
	{
		match_callbacks[0]();
		CTIMER->IR.MR0INT = 1;
	}

	if(CTIMER->IR.MR1INT)
	{
		match_callbacks[1]();
		CTIMER->IR.MR1INT = 1;
	}

	if(CTIMER->IR.MR2INT)
	{
		match_callbacks[2]();
		CTIMER->IR.MR2INT = 1;
	}

	if(CTIMER->IR.MR3INT)
	{
		match_callbacks[3]();
		CTIMER->IR.MR3INT = 1;
	}

	if(CTIMER->IR.CR0INT)
	{
		capture_callbacks[0]();
		CTIMER->IR.CR0INT = 1;
	}

	if(CTIMER->IR.CR1INT)
	{
		capture_callbacks[1]();
		CTIMER->IR.CR1INT = 1;
	}

	if(CTIMER->IR.CR2INT)
	{
		capture_callbacks[2]();
		CTIMER->IR.CR2INT = 1;
	}

	if(CTIMER->IR.CR3INT)
	{
		capture_callbacks[3]();
		CTIMER->IR.CR3INT = 1;
	}
}
