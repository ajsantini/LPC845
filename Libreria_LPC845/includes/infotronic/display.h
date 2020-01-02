/**
 * @file display.h
 * @brief Declaraciones para las funciones del display 7 segmentos
 * @author Augusto Santini
 * @date 12/2019
 * @version 1.0
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

/**
 * @brief Inicializacion de los display
 */
void display_init(void);

/**
 * @brief Apagar display
 * @param[in] display Numero de display a apagar (0 ~ 1)
 */
void display_off(uint8_t display);

/**
 * @brief Encender display
 * @param[in] display Numero de display a encender (0 ~ 1)
 */
void display_on(uint8_t display);

/**
 * @brief Escribir display
 * @param[in] display Numero de display sobre el cual escribir (0 ~ 1)
 * @param[in] number Numero a escribir en el display (0 ~ 999)
 */
void display_write(uint8_t display, uint32_t number);

/**
 * @brief Encender el display point de un digito de un display
 * @param[in] display Numero de display sobre el cual escribir (0 ~ 1)
 * @param[in] digit Digito del display a prender (0 ~ 2)
 */
void display_set_dp(uint8_t display, uint8_t digit);

/**
 * @brief Apagar el display point de un digito de un display
 * @param[in] display Numero de display sobre el cual escribir (0 ~ 1)
 * @param[in] digit Digito del display a prender (0 ~ 2)
 */
void display_clear_dp(uint8_t display, uint8_t digit);

/**
 * @brief Callback para actualizar el display periodicamente
 */
void display_check(void);

#endif /* DISPLAY_H_ */
