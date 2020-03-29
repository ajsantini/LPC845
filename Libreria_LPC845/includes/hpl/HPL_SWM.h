/**
 * @file HPL_SWM.h
 * @brief Definiciones a nivel de periferico del modulo SWM (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#ifndef HPL_SWM_H_
#define HPL_SWM_H_

#include <HRI_SWM.h>
#include <HPL_SYSCON.h>

extern volatile SWM_per_t * const SWM; //!< Periferico SWM

typedef enum
{
	SWM_ENABLE = 0,
	SWM_DISABLE
}SWM_enable_en;

/**
 * @brief Inicializacion de la Switch Matrix
 */
static inline void SWM_init(void)
{
	SYSCON->SYSAHBCLKCTRL0.SWM = 1;
}

/**
 * @brief Deinicializacion de la Switch Matrix
 */
static inline void SWM_deinit(void)
{
	SYSCON->SYSAHBCLKCTRL0.SWM = 0;
}

/**
 * @brief Asignar un pin del MCU a la funcion UARTn TXD
 * @param[in] uart Instancia de UART a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_uart_TXD(uint8_t uart, uint8_t port, uint8_t pin)
{
	switch(uart)
	{
	case 0: { SWM->PINASSIGN0.U0_TXD_O = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN1.U1_TXD_O = (port * 32) + pin; break; }
	case 2: { SWM->PINASSIGN2.U2_TXD_O = (port * 32) + pin; break; }
	case 3: { SWM->PINASSIGN11.UART3_TXD = (port * 32) + pin; break; }
	case 4: { SWM->PINASSIGN12.UART4_TXD = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion UARTn RXD
 * @param[in] uart Instancia de UART a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_uart_RXD(uint8_t uart, uint8_t port, uint8_t pin)
{
	switch(uart)
	{
	case 0: { SWM->PINASSIGN0.U0_RXD_I = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN1.U1_RXD_I = (port * 32) + pin; break; }
	case 2: { SWM->PINASSIGN2.U2_RXD_I = (port * 32) + pin; break; }
	case 3: { SWM->PINASSIGN12.UART3_RXD = (port * 32) + pin; break; }
	case 4: { SWM->PINASSIGN12.UART4_RXD = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion UARTn RTS
 * @param[in] uart Instancia de UART a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_uart_RTS(uint8_t uart, uint8_t port, uint8_t pin)
{
	switch(uart)
	{
	case 0: { SWM->PINASSIGN0.U0_RTS_O = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN1.U1_RTS_O = (port * 32) + pin; break; }
	case 2: { SWM->PINASSIGN3.U2_RTS_O = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion UARTn CTS
 * @param[in] uart Instancia de UART a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_uart_CTS(uint8_t uart, uint8_t port, uint8_t pin)
{
	switch(uart)
	{
	case 0: { SWM->PINASSIGN0.U0_CTS_I = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN2.U1_CTS_I = (port * 32) + pin; break; }
	case 2: { SWM->PINASSIGN3.U2_CTS_I = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion UARTn SCLK
 * @param[in] uart Instancia de UART a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_uart_SCLK(uint8_t uart, uint8_t port, uint8_t pin)
{
	switch(uart)
	{
	case 0: { SWM->PINASSIGN1.U0_SCLK_IO = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN2.U1_SCLK_IO = (port * 32) + pin; break; }
	case 2: { SWM->PINASSIGN3.U2_SCLK_IO = (port * 32) + pin; break; }
	case 3: { SWM->PINASSIGN11.UART3_TXD = (port * 32) + pin; break; }
	case 4: { SWM->PINASSIGN13.UART4_SCLK = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion SPIn SCK
 * @param[in] spi Instancia de SPI a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_spi_SCK(uint8_t spi, uint8_t port, uint8_t pin)
{
	switch(spi)
	{
	case 0: { SWM->PINASSIGN3.SPI0_SCK_IO = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN5.SPI1_SCK_IO = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion SPIn MOSI
 * @param[in] spi Instancia de SPI a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_spi_MOSI(uint8_t spi, uint8_t port, uint8_t pin)
{
	switch(spi)
	{
	case 0: { SWM->PINASSIGN4.SPI0_MOSI_IO = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN5.SPI1_MOSI_IO = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion SPIn MISO
 * @param[in] spi Instancia de SPI a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_spi_MISO(uint8_t spi, uint8_t port, uint8_t pin)
{
	switch(spi)
	{
	case 0: { SWM->PINASSIGN4.SPI0_MISO_IO = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN6.SPI1_MISO_IO = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion SPIn SSEL0
 * @param[in] spi Instancia de SPI a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_spi_SSEL0(uint8_t spi, uint8_t port, uint8_t pin)
{
	switch(spi)
	{
	case 0: { SWM->PINASSIGN4.SPI0_SSEL0_IO = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN6.SPI1_SSEL0_IO = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion SPIn SSEL1
 * @param[in] spi Instancia de SPI a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_spi_SSEL1(uint8_t spi, uint8_t port, uint8_t pin)
{
	switch(spi)
	{
	case 0: { SWM->PINASSIGN4.SPI0_SSEL1_IO = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN6.SPI1_SSEL1_IO = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion SPIn SSEL2
 * @param[in] spi Instancia de SPI a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_spi_SSEL2(uint8_t spi, uint8_t port, uint8_t pin)
{
	switch(spi)
	{
	case 0: { SWM->PINASSIGN5.SPI0_SSEL2_IO = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion SPIn SSEL3
 * @param[in] spi Instancia de SPI a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_spi_SSEL3(uint8_t spi, uint8_t port, uint8_t pin)
{
	switch(spi)
	{
	case 0: { SWM->PINASSIGN5.SPI0_SSEL3_IO = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT IN_A
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_IN_A(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN6.SCT0_GPIO_IN_A_I = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT IN_B
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_IN_B(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN7.SCT0_GPIO_IN_B_I = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT IN_C
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_IN_C(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN7.SCT0_GPIO_IN_C_I = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT IN_D
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_IN_D(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN7.SCT0_GPIO_IN_D_I = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT OUT0
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_OUT0(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN7.SCT_OUT0_O = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT OUT1
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_OUT1(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN8.SCT_OUT1_O = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT OUT2
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_OUT2(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN8.SCT_OUT2_O = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT OUT3
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_OUT3(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN8.SCT_OUT3_O = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT OUT4
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_OUT4(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN8.SCT_OUT4_O = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT OUT5
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_OUT5(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN9.SCT_OUT5_O = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion SCT OUT6
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_sct_OUT6(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN9.SCT_OUT6_O = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion IICn SDA
 *
 * En el caso del IIC0, no se le da importancia al puerto y pin
 *
 * @param[in] iic Instancia de IIC a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_iic_SDA(uint8_t iic, uint8_t port, uint8_t pin)
{
	switch(iic)
	{
	case 0: { SWM->PINENABLE0.I2C0_SDA = 0; break; }
	case 1: { SWM->PINASSIGN9.I2C1_SDA_IO = (port * 32) + pin; break; }
	case 2: { SWM->PINASSIGN10.I2C2_SDA_IO = (port * 32) + pin; break; }
	case 3: { SWM->PINASSIGN10.I2C3_SDA_IO = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion IICn SCL
 *
 * En el caso del IIC0, no se le da importancia al puerto y pin
 *
 * @param[in] iic Instancia de IIC a la cual asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_iic_SCL(uint8_t iic, uint8_t port, uint8_t pin)
{
	switch(iic)
	{
	case 0: { SWM->PINENABLE0.I2C0_SCL = 0; break; }
	case 1: { SWM->PINASSIGN9.I2C1_SCL_IO = (port * 32) + pin; break; }
	case 2: { SWM->PINASSIGN10.I2C2_SCL_IO = (port * 32) + pin; break; }
	case 3: { SWM->PINASSIGN10.I2C3_SCL_IO = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion COMP0 OUT
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_COMP0_OUT(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN11.COMP0_OUT_O = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion CLKOUT
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_CLKOUT(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN11.CLKOUT_O = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion INT BMAT
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_INT_BMAT(uint8_t port, uint8_t pin)
{
	SWM->PINASSIGN11.GPIO_INT_BMAT_O = (port * 32) + pin;
}

/**
 * @brief Asignar un pin del MCU a la funcion T0 MATn
 * @param[in] mat Numero de MATn a asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_T0_MAT(uint8_t mat, uint8_t port, uint8_t pin)
{
	switch(mat)
	{
	case 0: { SWM->PINASSIGN13.T0_MAT0 = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN13.T0_MAT1 = (port * 32) + pin; break; }
	case 2: { SWM->PINASSIGN13.T0_MAT2 = (port * 32) + pin; break; }
	case 3: { SWM->PINASSIGN14.T0_MAT3 = (port * 32) + pin; break; }
	}
}

/**
 * @brief Asignar un pin del MCU a la funcion T0 CAPn
 * @param[in] cap Numero de CAPn a asignar
 * @param[in] port Numero de puerto a asignar
 * @param[in] pin Numero de pin a asignar
 */
static inline void SWM_assign_T0_CAP(uint8_t cap, uint8_t port, uint8_t pin)
{
	switch(cap)
	{
	case 0: { SWM->PINASSIGN14.T0_CAP0 = (port * 32) + pin; break; }
	case 1: { SWM->PINASSIGN14.T0_CAP1 = (port * 32) + pin; break; }
	case 2: { SWM->PINASSIGN14.T0_CAP2 = (port * 32) + pin; break; }
	}
}

/**
 * @brief Habilitar/inhabilitar la funcion ACMPn
 * @param[in] acmp Numero de ACMP a asignar
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_ACMP(uint8_t acmp, SWM_enable_en en_dis)
{
	switch(acmp)
	{
	case 1: { SWM->PINENABLE0.ACMP_I1 = en_dis; break; }
	case 2: { SWM->PINENABLE0.ACMP_I2 = en_dis; break; }
	case 3: { SWM->PINENABLE0.ACMP_I3 = en_dis; break; }
	case 4: { SWM->PINENABLE0.ACMP_I4 = en_dis; break; }
	case 5: { SWM->PINENABLE0.ACMP_I5 = en_dis; break; }
	}
}

/**
 * @brief Habilitar/inhabilitar la funcion SWCLK
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_SWCLK(SWM_enable_en en_dis)
{
	SWM->PINENABLE0.SWCLK = en_dis;
}

/**
 * @brief Habilitar/inhabilitar la funcion SWDIO
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_SWDIO(SWM_enable_en en_dis)
{
	SWM->PINENABLE0.SWDIO = en_dis;
}

/**
 * @brief Habilitar/inhabilitar la funcion XTALIN
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_XTALIN(SWM_enable_en en_dis)
{
	SWM->PINENABLE0.XTALIN = en_dis;
}

/**
 * @brief Habilitar/inhabilitar la funcion XTALOUT
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_XTALOUT(SWM_enable_en en_dis)
{
	SWM->PINENABLE0.XTALOUT = en_dis;
}

/**
 * @brief Habilitar/inhabilitar la funcion RESETN
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_RESETN(SWM_enable_en en_dis)
{
	SWM->PINENABLE0.RESETN = en_dis;
}

/**
 * @brief Habilitar/inhabilitar la funcion CLKIN
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_CLKIN(SWM_enable_en en_dis)
{
	SWM->PINENABLE0.CLKIN = en_dis;
}

/**
 * @brief Habilitar/inhabilitar la funcion VDDCMP
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_VDDCMP(SWM_enable_en en_dis)
{
	SWM->PINENABLE0.VDDCMP = en_dis;
}

/**
 * @brief Habilitar/inhabilitar la funcion ADC
 * @param[in] adc Numero de ADC a asignar
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_ADC(uint8_t adc, SWM_enable_en en_dis)
{
	switch(adc)
	{
	case 0: { SWM->PINENABLE0.ADC_0 = en_dis; break; }
	case 1: { SWM->PINENABLE0.ADC_1 = en_dis; break; }
	case 2: { SWM->PINENABLE0.ADC_2 = en_dis; break; }
	case 3: { SWM->PINENABLE0.ADC_3 = en_dis; break; }
	case 4: { SWM->PINENABLE0.ADC_4 = en_dis; break; }
	case 5: { SWM->PINENABLE0.ADC_5 = en_dis; break; }
	case 6: { SWM->PINENABLE0.ADC_6 = en_dis; break; }
	case 7: { SWM->PINENABLE0.ADC_7 = en_dis; break; }
	case 8: { SWM->PINENABLE0.ADC_8 = en_dis; break; }
	case 9: { SWM->PINENABLE0.ADC_9 = en_dis; break; }
	case 10: { SWM->PINENABLE0.ADC_10 = en_dis; break; }
	case 11: { SWM->PINENABLE0.ADC_11 = en_dis; break; }
	}
}

/**
 * @brief Habilitar/inhabilitar la funcion DAC
 * @param[in] dac Numero de DAC a asignar
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_DAC(uint8_t dac, SWM_enable_en en_dis)
{
	switch(dac)
	{
	case 0: { SWM->PINENABLE0.DACOUT0 = en_dis; break; }
	case 1: { SWM->PINENABLE0.DACOUT1 = en_dis; break; }
	}
}

/**
 * @brief Habilitar/inhabilitar la funcion CAPTX
 * @param[in] captx Numero de CAPTX a asignar
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_CAPTX(uint8_t captx, SWM_enable_en en_dis)
{
	switch(captx)
	{
	case 0: { SWM->PINENABLE0.CAPT_X0 = en_dis; break; }
	case 1: { SWM->PINENABLE0.CAPT_X1 = en_dis; break; }
	case 2: { SWM->PINENABLE0.CAPT_X2 = en_dis; break; }
	case 3: { SWM->PINENABLE0.CAPT_X3 = en_dis; break; }
	case 4: { SWM->PINENABLE1.CAPT_X4 = en_dis; break; }
	case 5: { SWM->PINENABLE1.CAPT_X5 = en_dis; break; }
	case 6: { SWM->PINENABLE1.CAPT_X6 = en_dis; break; }
	case 7: { SWM->PINENABLE1.CAPT_X7 = en_dis; break; }
	case 8: { SWM->PINENABLE1.CAPT_X8 = en_dis; break; }
	}
}

/**
 * @brief Habilitar/inhabilitar la funcion CAPYL
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_CAPYL(SWM_enable_en en_dis)
{
	SWM->PINENABLE1.CAPT_YL = en_dis;
}

/**
 * @brief Habilitar/inhabilitar la funcion CAPYH
 * @param[in] en_dis Seleccion de habilitacion o inhabilitacion
 */
static inline void SWM_enable_CAPYH(SWM_enable_en en_dis)
{
	SWM->PINENABLE1.CAPT_YH = en_dis;
}

#endif /* HPL_SWM_H_ */
