/**
 * @file termometro.h
 * @brief Declaraciones para el periferico termometro implementado con R fija y NTC
 * @author Augusto Santini
 * @date 12/2019
 * @version 1.0
 */

#ifndef TERMOMETRO_H_
#define TERMOMETRO_H_

/**
 * @brief Inicializacion del termometro
 */
void termometro_init(void);

/**
 * @brief Callback para revisar el termometro periodicamente
 */
void termometro_check(void);

/**
 * @brief Tomar la lectura actual del termometro
 * @return Lectura actual del termometro en mili °C
 */
uint32_t termometro_read(void);

#endif /* TERMOMETRO_H_ */
