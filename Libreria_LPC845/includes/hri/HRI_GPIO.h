/**
 * @file HRI_GPIO.h
 * @brief Definiciones a nivel de registros del modulo GPIO (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HRI_GPIO_H_
#define HRI_GPIO_H_

#include <stdint.h>

#define	GPIO_BASE	0xA0000000

typedef struct
{
	uint8_t PBYTE : 1;
	uint8_t : 7;
}GPIO_B_reg_t;

typedef struct
{
	uint32_t PWORD;
}GPIO_W_reg_t;

typedef struct
{
	uint32_t DIRP;
}GPIO_DIR_reg_t;

typedef struct
{
	uint32_t MASKP;
}GPIO_MASK_reg_t;

typedef struct
{
	uint32_t PORT;
}GPIO_PIN_reg_t;

typedef struct
{
	uint32_t MPORTP;
}GPIO_MPIN_reg_t;

typedef struct
{
	uint32_t SETP;
}GPIO_SET_reg_t;

typedef struct
{
	uint32_t CLRP;
}GPIO_CLR_reg_t;

typedef struct
{
	uint32_t NOTP;
}GPIO_NOT_reg_t;

typedef struct
{
	uint32_t DIRSETP;
}GPIO_DIRSET_reg_t;

typedef struct
{
	uint32_t DIRCLRP;
}GPIO_DIRCLR_reg_t;

typedef struct
{
	uint32_t DIRNOTP;
}GPIO_DIRNOT_reg_t;

typedef struct
{
	GPIO_B_reg_t B0;
	GPIO_B_reg_t B1;
	GPIO_B_reg_t B2;
	GPIO_B_reg_t B3;
	GPIO_B_reg_t B4;
	GPIO_B_reg_t B5;
	GPIO_B_reg_t B6;
	GPIO_B_reg_t B7;
	GPIO_B_reg_t B8;
	GPIO_B_reg_t B9;
	GPIO_B_reg_t B10;
	GPIO_B_reg_t B11;
	GPIO_B_reg_t B12;
	GPIO_B_reg_t B13;
	GPIO_B_reg_t B14;
	GPIO_B_reg_t B15;
	GPIO_B_reg_t B16;
	GPIO_B_reg_t B17;
	GPIO_B_reg_t B18;
	GPIO_B_reg_t B19;
	GPIO_B_reg_t B20;
	GPIO_B_reg_t B21;
	GPIO_B_reg_t B22;
	GPIO_B_reg_t B23;
	GPIO_B_reg_t B24;
	GPIO_B_reg_t B25;
	GPIO_B_reg_t B26;
	GPIO_B_reg_t B27;
	GPIO_B_reg_t B28;
	GPIO_B_reg_t B29;
	GPIO_B_reg_t B30;
	GPIO_B_reg_t B31;
	GPIO_B_reg_t B32;
	GPIO_B_reg_t B33;
	GPIO_B_reg_t B34;
	GPIO_B_reg_t B35;
	GPIO_B_reg_t B36;
	GPIO_B_reg_t B37;
	GPIO_B_reg_t B38;
	GPIO_B_reg_t B39;
	GPIO_B_reg_t B40;
	GPIO_B_reg_t B41;
	GPIO_B_reg_t B42;
	GPIO_B_reg_t B43;
	GPIO_B_reg_t B44;
	GPIO_B_reg_t B45;
	GPIO_B_reg_t B46;
	GPIO_B_reg_t B47;
	GPIO_B_reg_t B48;
	GPIO_B_reg_t B49;
	GPIO_B_reg_t B50;
	GPIO_B_reg_t B51;
	GPIO_B_reg_t B52;
	GPIO_B_reg_t B53;
	const uint8_t RESERVED_1[0xFCA];
	GPIO_W_reg_t W0;
	GPIO_W_reg_t W1;
	GPIO_W_reg_t W2;
	GPIO_W_reg_t W3;
	GPIO_W_reg_t W4;
	GPIO_W_reg_t W5;
	GPIO_W_reg_t W6;
	GPIO_W_reg_t W7;
	GPIO_W_reg_t W8;
	GPIO_W_reg_t W9;
	GPIO_W_reg_t W10;
	GPIO_W_reg_t W11;
	GPIO_W_reg_t W12;
	GPIO_W_reg_t W13;
	GPIO_W_reg_t W14;
	GPIO_W_reg_t W15;
	GPIO_W_reg_t W16;
	GPIO_W_reg_t W17;
	GPIO_W_reg_t W18;
	GPIO_W_reg_t W19;
	GPIO_W_reg_t W20;
	GPIO_W_reg_t W21;
	GPIO_W_reg_t W22;
	GPIO_W_reg_t W23;
	GPIO_W_reg_t W24;
	GPIO_W_reg_t W25;
	GPIO_W_reg_t W26;
	GPIO_W_reg_t W27;
	GPIO_W_reg_t W28;
	GPIO_W_reg_t W29;
	GPIO_W_reg_t W30;
	GPIO_W_reg_t W31;
	GPIO_W_reg_t W32;
	GPIO_W_reg_t W33;
	GPIO_W_reg_t W34;
	GPIO_W_reg_t W35;
	GPIO_W_reg_t W36;
	GPIO_W_reg_t W37;
	GPIO_W_reg_t W38;
	GPIO_W_reg_t W39;
	GPIO_W_reg_t W40;
	GPIO_W_reg_t W41;
	GPIO_W_reg_t W42;
	GPIO_W_reg_t W43;
	GPIO_W_reg_t W44;
	GPIO_W_reg_t W45;
	GPIO_W_reg_t W46;
	GPIO_W_reg_t W47;
	GPIO_W_reg_t W48;
	GPIO_W_reg_t W49;
	GPIO_W_reg_t W50;
	GPIO_W_reg_t W51;
	GPIO_W_reg_t W52;
	GPIO_W_reg_t W53;
	const uint8_t RESERVED_2[0xF28];
	GPIO_DIR_reg_t DIR0;
	GPIO_DIR_reg_t DIR1;
	const uint8_t RESERVED_3[0x78];
	GPIO_MASK_reg_t MASK0;
	GPIO_MASK_reg_t MASK1;
	const uint8_t RESERVED_4[0x78];
	GPIO_PIN_reg_t PIN0;
	GPIO_PIN_reg_t PIN1;
	const uint8_t RESERVED_5[0x78];
	GPIO_MPIN_reg_t MPIN0;
	const uint8_t RESERVED_6[0x79];
	GPIO_SET_reg_t SET0;
	GPIO_SET_reg_t SET1;
	const uint8_t RESERVED_7[0x78];
	GPIO_CLR_reg_t CLR0;
	GPIO_CLR_reg_t CLR1;
	const uint8_t RESERVED_8[0x78];
	GPIO_NOT_reg_t NOT0;
	GPIO_NOT_reg_t NOT1;
	const uint8_t RESERVED_9[0x78];
	GPIO_DIRSET_reg_t DIRSET0;
	GPIO_DIRSET_reg_t DIRSET1;
	const uint8_t RESERVED_10[0x78];
	GPIO_DIRCLR_reg_t DIRCLR0;
	GPIO_DIRCLR_reg_t DIRCLR1;
	const uint8_t RESERVED_11[0x78];
	GPIO_DIRNOT_reg_t DIRNOT0;
	GPIO_DIRNOT_reg_t DIRNOT1;
}GPIO_per_t;

volatile GPIO_per_t * const GPIO = (GPIO_per_t *) GPIO_BASE; //!< Periferico GPIO

#endif /* HRI_GPIO_H_ */
