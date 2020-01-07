/**
 * @file relays.h
 * @brief Declaraciones para las funciones del manejo de reles
 * @author Augusto Santini
 * @date 12/2019
 * @version 1.0
 */

#ifndef RELAYS_H_
#define RELAYS_H_

/**
 * @brief Inicializacion de los reles
 */
void relays_init(void);

/**
 * @brief Energizar un rele
 * @param[in] numero_rele Rele a energizar (0 ~ 3)
 */
void relays_activate(uint8_t numero_rele);

/**
 * @brief Desenergizar un rele
 * @param[in] numero_rele Rele a desenergizar (0 ~ 3)
 */
void relays_deactivate(uint8_t numero_rele);

/**
 * @brief Invertir estado de un rele
 * @param[in] numero_rele Rele a invertir (0 ~ 3)
 */
void relays_toggle(uint8_t numero_rele);

/**
 * @brief Callback para actualizar los reles periodicamente
 */
void relays_check(void);

#endif /* RELAYS_H_ */
