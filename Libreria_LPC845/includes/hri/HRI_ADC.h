/**
 * @file HRI_ADC.h
 * @brief Declaraciones a nivel de registros del ADC (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HRI_ADC_H_
#define HRI_ADC_H_

#include <stdint.h>

#define	ADC_BASE	0x4001C000 //!< Direccion base del ADC

//! Registro de control del ADC
typedef struct
{
	uint32_t CLKDIV : 8; //!< Divisor del clock
	uint32_t ASYNCMODE : 1; //!< 0 -> Modo sincronico ; 1 -> Modo asincronico
	uint32_t : 1; //!< Reservado
	uint32_t LPWRMODE : 1; //!< Modo de bajo consumo
	uint32_t : 19;  //!< Reservado
	uint32_t CALMODE : 1; //!< Modo calibracion
	uint32_t : 1; //!< Reservado
}ADC_CTRL_reg_t;

//! Registro de control de secuencia A y B del ADC
typedef struct
{
	uint32_t CHANNELS : 12; //!< Canales habilitados para la conversion en la secuencia
	uint32_t TRIGGER : 3; //!< Que pin dispara la secuencia
	uint32_t : 3; //!< Reservado
	uint32_t TRIGPOL : 1; //!< Polaridad del pin que dispara la secuencia
	uint32_t SYNCBYPASS : 1; //!< Disparo asincronico/sincronico
	uint32_t : 6; //!< Reservado
	uint32_t START : 1; //!< Empezar secuencia mediante software
	uint32_t BURST : 1; //!< Habilitacion de modo rafaga
	uint32_t SINGLESTEP : 1; //!< Un start activa un paso o la secuencia entera
	uint32_t LOWPRIO : 1; //!< Prioridad. Solo lo tiene la secuencia A
	uint32_t MODE : 1; //!< Interrupcion por cada conversion o al final de la secuencia
	uint32_t SEQ_ENA : 1; //!< Habilitacion de secuencia
}ADC_SEQ_CTRL_reg_t;

typedef struct
{
	uint32_t : 4;
	uint32_t RESULT : 12;
	uint32_t THCMPRANGE : 2;
	uint32_t THCMPCROSS : 2;
	uint32_t : 6;
	uint32_t CHANNEL : 4;
	uint32_t OVERRUN : 1;
	uint32_t DATAVALID : 1;
}ADC_SEQ_GDAT_reg_t;

typedef struct
{
	uint32_t : 4;
	uint32_t RESULT : 12;
	uint32_t THCMPRANGE : 2;
	uint32_t THCMPCROSS : 2;
	uint32_t : 6;
	uint32_t CHANNEL : 4;
	uint32_t OVERRUN : 1;
	uint32_t DATAVALID : 1;
}ADC_DAT_reg_t;

typedef struct
{
	uint32_t : 4;
	uint32_t THRLOW : 12;
	uint32_t : 16;
}ADC_THR_LOW_reg_t;

typedef struct
{
	uint32_t : 4;
	uint32_t THRHIGH : 12;
	uint32_t : 16;
}ADC_THR_HIGH_reg_t;

typedef struct
{
	uint32_t CH0_THRSEL : 1;
	uint32_t CH1_THRSEL : 1;
	uint32_t CH2_THRSEL : 1;
	uint32_t CH3_THRSEL : 1;
	uint32_t CH4_THRSEL : 1;
	uint32_t CH5_THRSEL : 1;
	uint32_t CH6_THRSEL : 1;
	uint32_t CH7_THRSEL : 1;
	uint32_t CH8_THRSEL : 1;
	uint32_t CH9_THRSEL : 1;
	uint32_t CH10_THRSEL : 1;
	uint32_t CH11_THRSEL : 1;
	uint32_t : 20;
}ADC_CHAN_THRSEL_reg_t;

typedef struct
{
	uint32_t SEQA_INTEN : 1;
	uint32_t SEQB_INTEN : 1;
	uint32_t OVR_INTEN : 1;
	uint32_t ADCMPINTEN0 : 2;
	uint32_t ADCMPINTEN1 : 2;
	uint32_t ADCMPINTEN2 : 2;
	uint32_t ADCMPINTEN3 : 2;
	uint32_t ADCMPINTEN4 : 2;
	uint32_t ADCMPINTEN5 : 2;
	uint32_t ADCMPINTEN6 : 2;
	uint32_t ADCMPINTEN7 : 2;
	uint32_t ADCMPINTEN8 : 2;
	uint32_t ADCMPINTEN9 : 2;
	uint32_t ADCMPINTEN10 : 2;
	uint32_t ADCMPINTEN11 : 2;
	uint32_t : 5;
}ADC_INTEN_reg_t;

typedef struct
{
	uint32_t THCMP0 : 1;
	uint32_t THCMP1 : 1;
	uint32_t THCMP2 : 1;
	uint32_t THCMP3 : 1;
	uint32_t THCMP4 : 1;
	uint32_t THCMP5 : 1;
	uint32_t THCMP6 : 1;
	uint32_t THCMP7 : 1;
	uint32_t THCMP8 : 1;
	uint32_t THCMP9 : 1;
	uint32_t THCMP10 : 1;
	uint32_t THCMP11 : 1;
	uint32_t OVERRUN0 : 1;
	uint32_t OVERRUN1 : 1;
	uint32_t OVERRUN2 : 1;
	uint32_t OVERRUN3 : 1;
	uint32_t OVERRUN4 : 1;
	uint32_t OVERRUN5 : 1;
	uint32_t OVERRUN6 : 1;
	uint32_t OVERRUN7 : 1;
	uint32_t OVERRUN8 : 1;
	uint32_t OVERRUN9 : 1;
	uint32_t OVERRUN10 : 1;
	uint32_t OVERRUN11 : 1;
	uint32_t : 2;
	uint32_t SEQA_INT : 1;
	uint32_t SEQB_INT : 1;
	uint32_t THCMP_INT : 1;
	uint32_t OVR_INT : 1;
}ADC_FLAGS_reg_t;

typedef struct
{
	uint32_t : 5;
	uint32_t VRANGE : 1;
	uint32_t : 26;
}ADC_TRM_reg_t;

typedef struct
{
	ADC_CTRL_reg_t CTRL;
	const uint32_t RESERVED_1;
	ADC_SEQ_CTRL_reg_t SEQ_CTRL[2];
	ADC_SEQ_GDAT_reg_t SEQ_GDAT[2];
	const uint32_t RESERVED_2[2];
	const ADC_DAT_reg_t DAT[12];
	ADC_THR_LOW_reg_t THR_LOW[2];
	ADC_THR_HIGH_reg_t THR_HIGH[2];
	ADC_CHAN_THRSEL_reg_t CHAN_THRSEL;
	ADC_INTEN_reg_t INTEN;
	ADC_FLAGS_reg_t FLAGS;
	ADC_TRM_reg_t TRM;
}ADC_per_t;

extern volatile ADC_per_t * const ADC; //!< Periferico ADC

#endif /* HRI_ADC_H_ */
