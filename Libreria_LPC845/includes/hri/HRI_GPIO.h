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
	GPIO_B_reg_t B[54];
	const uint8_t RESERVED_1[0xFCA];
	GPIO_W_reg_t W[54];
	const uint8_t RESERVED_2[0xF28];
	GPIO_DIR_reg_t DIR[2];
	const uint8_t RESERVED_3[0x78];
	GPIO_MASK_reg_t MASK[2];
	const uint8_t RESERVED_4[0x78];
	GPIO_PIN_reg_t PIN[2];
	const uint8_t RESERVED_5[0x78];
	GPIO_MPIN_reg_t MPIN[2];
	const uint8_t RESERVED_6[0x75];
	GPIO_SET_reg_t SET[2];
	const uint8_t RESERVED_7[0x78];
	GPIO_CLR_reg_t CLR[2];
	const uint8_t RESERVED_8[0x78];
	GPIO_NOT_reg_t NOT[2];
	const uint8_t RESERVED_9[0x78];
	GPIO_DIRSET_reg_t DIRSET[2];
	const uint8_t RESERVED_10[0x78];
	GPIO_DIRCLR_reg_t DIRCLR[2];
	const uint8_t RESERVED_11[0x78];
	GPIO_DIRNOT_reg_t DIRNOT[2];
}GPIO_per_t;

#endif /* HRI_GPIO_H_ */
