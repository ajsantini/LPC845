/**
 * @file teclado.h
 * @brief Declaraciones para el periferico teclado (2x3)
 * @author Augusto Santini
 * @date 12/2019
 * @version 1.0
 */

#ifndef TECLADO_H_
#define TECLADO_H_

#define		NO_KEY							0xFF

/**
 * @brief Inicializacion del teclado
 * @param[in] bounces Tiempo en milisegundos para tomar una tecla como valida/invalida
 */
void teclado_init(uint32_t bounces);

/**
 * @brief Obtener tecla pulsada
 * @return Tecla pulsada (0 ~ 5 o NO_KEY), una vez leida, se limpia el buffer hasta que se presione la proxima tecla
 */
uint8_t get_key(void);

/**
 * @brief Callback para revisar el teclado periodicamente
 */
void teclado_check(void);

#endif /* TECLADO_H_ */
