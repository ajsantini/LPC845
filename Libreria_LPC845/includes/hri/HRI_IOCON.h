/**
 * @file HRI_GPIO.h
 * @brief Definiciones a nivel de registros del modulo IOCON (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HRI_IOCON_H_
#define HRI_IOCON_H_

#include <stdint.h>

#define	IOCON_BASE	0x40044000

/*
 * Nota sobre la siguiente estructura:
 *
 * No todos los pines admiten configuracion de ADC o I2C.
 * Esto sera tomado en cuenta a la hora de configurar cada pin.
 *
 */
typedef struct
{
	uint32_t : 3;
	uint32_t MODE : 2;
	uint32_t HYS : 1;
	uint32_t INV : 1;
	uint32_t I2CMODE : 2;
	uint32_t : 1;
	uint32_t OD : 1;
	uint32_t S_MODE : 2;
	uint32_t CLK_DIV : 3;
	uint32_t DACMODE : 1;
	uint32_t : 15;
}IOCON_PIO_reg_t;

typedef struct
{
	IOCON_PIO_reg_t PIO0_17;
	IOCON_PIO_reg_t PIO0_13;
	IOCON_PIO_reg_t PIO0_12;
	IOCON_PIO_reg_t PIO0_5;
	IOCON_PIO_reg_t PIO0_4;
	IOCON_PIO_reg_t PIO0_3;
	IOCON_PIO_reg_t PIO0_2;
	IOCON_PIO_reg_t PIO0_11;
	IOCON_PIO_reg_t PIO0_10;
	IOCON_PIO_reg_t PIO0_16;
	IOCON_PIO_reg_t PIO0_15;
	IOCON_PIO_reg_t PIO0_1;
	const uint32_t _RESERVED_1;
	IOCON_PIO_reg_t PIO0_9;
	IOCON_PIO_reg_t PIO0_8;
	IOCON_PIO_reg_t PIO0_7;
	IOCON_PIO_reg_t PIO0_6;
	IOCON_PIO_reg_t PIO0_0;
	IOCON_PIO_reg_t PIO0_14;
	const uint32_t _RESERVED_2;
	IOCON_PIO_reg_t PIO0_28;
	IOCON_PIO_reg_t PIO0_27;
	IOCON_PIO_reg_t PIO0_26;
	IOCON_PIO_reg_t PIO0_25;
	IOCON_PIO_reg_t PIO0_24;
	IOCON_PIO_reg_t PIO0_23;
	IOCON_PIO_reg_t PIO0_22;
	IOCON_PIO_reg_t PIO0_21;
	IOCON_PIO_reg_t PIO0_20;
	IOCON_PIO_reg_t PIO0_19;
	IOCON_PIO_reg_t PIO0_18;
	IOCON_PIO_reg_t PIO1_8;
	IOCON_PIO_reg_t PIO1_9;
	IOCON_PIO_reg_t PIO1_12;
	IOCON_PIO_reg_t PIO1_13;
	IOCON_PIO_reg_t PIO0_31;
	IOCON_PIO_reg_t PIO1_0;
	IOCON_PIO_reg_t PIO1_1;
	IOCON_PIO_reg_t PIO1_2;
	IOCON_PIO_reg_t PIO1_14;
	IOCON_PIO_reg_t PIO1_15;
	IOCON_PIO_reg_t PIO1_3;
	IOCON_PIO_reg_t PIO1_4;
	IOCON_PIO_reg_t PIO1_5;
	IOCON_PIO_reg_t PIO1_16;
	IOCON_PIO_reg_t PIO1_17;
	IOCON_PIO_reg_t PIO1_6;
	IOCON_PIO_reg_t PIO1_18;
	IOCON_PIO_reg_t PIO1_19;
	IOCON_PIO_reg_t PIO1_7;
	IOCON_PIO_reg_t PIO0_29;
	IOCON_PIO_reg_t PIO0_30;
	IOCON_PIO_reg_t PIO1_20;
	IOCON_PIO_reg_t PIO1_21;
	IOCON_PIO_reg_t PIO1_11;
	IOCON_PIO_reg_t PIO1_10;
}IOCON_per_t;

extern volatile IOCON_per_t * const IOCON; //!< Periferico IOCON

#endif /* HRI_IOCON_H_ */
