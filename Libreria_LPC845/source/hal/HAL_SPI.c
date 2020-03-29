/**
 * @file HAL_SPI.c
 * @brief Funciones a nivel de aplicacion del periferico SPI (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

#include <HAL_SPI.h>
#include <HPL_SWM.h>
#include <HPL_SYSCON.h>

/**
 * @brief Inicializar SPI en modo master
 * @param[in] inst Instancia de SPI a inicializar
 * @param[in] config Configuracion deseada
 */
void hal_spi_master_mode_init(hal_spi_sel_en inst, const hal_spi_master_mode_config_t *config)
{
	SWM_init();

	SWM_assign_spi_MISO(inst, HAL_GPIO_PORTPIN_TO_PORT(config->miso_portpin), HAL_GPIO_PORTPIN_TO_PIN(config->miso_portpin));
	SWM_assign_spi_MOSI(inst, HAL_GPIO_PORTPIN_TO_PORT(config->mosi_portpin), HAL_GPIO_PORTPIN_TO_PIN(config->mosi_portpin));
	SWM_assign_spi_SCK(inst, HAL_GPIO_PORTPIN_TO_PORT(config->sck_portpin), HAL_GPIO_PORTPIN_TO_PIN(config->sck_portpin));

	if(config->ssel_portpin[0] != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_SSEL0(inst, HAL_GPIO_PORTPIN_TO_PORT(config->ssel_portpin[0]), HAL_GPIO_PORTPIN_TO_PIN(config->ssel_portpin[0]));
	}

	if(config->ssel_portpin[1] != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_SSEL1(inst, HAL_GPIO_PORTPIN_TO_PORT(config->ssel_portpin[1]), HAL_GPIO_PORTPIN_TO_PIN(config->ssel_portpin[1]));
	}

	if(config->ssel_portpin[2] != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_SSEL2(inst, HAL_GPIO_PORTPIN_TO_PORT(config->ssel_portpin[2]), HAL_GPIO_PORTPIN_TO_PIN(config->ssel_portpin[2]));
	}

	if(config->ssel_portpin[3] != HAL_GPIO_PORTPIN_NOT_USED)
	{
		SWM_assign_spi_SSEL3(inst, HAL_GPIO_PORTPIN_TO_PORT(config->ssel_portpin[3]), HAL_GPIO_PORTPIN_TO_PIN(config->ssel_portpin[3]));
	}

	SWM_deinit();


}
