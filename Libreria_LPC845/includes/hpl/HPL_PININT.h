/**
 * @file HPL_PININT.h
 * @brief Declaraciones a nivel de abstraccion de periferico del PININT (LPC845)
 * @author Augusto Santini
 * @date 6/2019
 * @version 1.0
 */

#ifndef HPL_PININT_H_
#define HPL_PININT_H_

#include <stdint.h>

#define	PININT_CONFIGURE_SUCCESS					0
#define	PININT_CONFIGURE_INVALID_PORT				-1
#define	PININT_CONFIGURE_INVALID_PORTPIN			-2
#define	PININT_CONFIGURE_INVALID_PIN				-3
#define	PININT_CONFIGURE_NOT_CLOCKED				-4
#define PININT_CONFIGURE_INVALID_INTERRUPT			-5

#define	PININT_REGISTER_CALLBACK_SUCCESS			0
#define	PININT_REGISTER_CALLBACK_INVALID_INTERRUPT	-1
#define PININT_REGISTER_CALLBACK_NOT_CLOCKED		-2

#define	PININT_MODE_EDGE_SENSITIVE			0
#define	PININT_MODE_LEVEL_SENSITIVE			1

typedef struct
{
	uint8_t channel;
	uint8_t mode;
	uint8_t int_on_rising_edge;
	uint8_t int_on_falling_edge;
}PININT_config_t;

/**
 * @brief Inicializa modulo de GPIO_INT
 *
 * Unicamente habilita el clock del modulo
 */
void PININT_init(void);

/**
 * @brief Inhabilita modulo de GPIO_INT
 *
 * Unicamente inhabilita el clock del modulo
 */
void PININT_deinit(void);

/**
 * @brief Configurar interrupciones de pin
 * @param[in] config Puntero a configuracion de interrupciones de pin
 */
void PININT_configure_pin_interrupt(PININT_config_t *config);

/**
 * @brief Registrar callback a llamar en interrupcion de PININTn
 * @param[in] new_callback Puntero a funcion a ejecutar
 * @param[in] interrupt Numero de PININT al cual registrar el callback
 * @return Estado del registro del callback nuevo
 */
int32_t PININT_register_callback(void (*new_callback)(void), uint32_t interrupt);

#endif /* HPL_PININT_H_ */
