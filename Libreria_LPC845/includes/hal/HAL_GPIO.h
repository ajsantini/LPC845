/**
 * @file HAL_GPIO.h
 * @brief Declaraciones a nivel de aplicacion del periferico GPIO (LPC845)
 * @author Augusto Santini
 * @date 3/2020
 * @version 1.0
 */

/**
 * @defgroup GPIO Entradas/Salidas de Propósito General (GPIO)
 *
 * # Introducción
 *
 * El periférico \e GPIO es el encargado de controlar tanto las entradas como la salidas <b>digitales</b>. Esto
 * implica que las salidas solamente podrán tomar valores <em>cero</em> o <em>uno</em> y que las entradas
 * únicamente interpretarán valores <em>cero</em> o <em>uno</em>.
 *
 * Cada uno de los pines del microcontrolador están descriptos mediante un número de \e puerto y un número de
 * <em>pin</em>. En este caso se tienen 2 \e puertos (0 y 1) con 32 \e pines cada uno (0 a 31). Cabe destacar
 * que si bien están definidas las condiciones para que se tengan un total de 64 pines de entrada/salida, la
 * cantidad real disponible en el microcontrolador depende del encapsulado. En el caso del encapsulado trabajado
 * en esta librería (por ahora), se tienen todos los pines del \e puerto 0, y los pines del 0 al 9 del \e puerto 1.
 * Referirse a la sección @ref acerca_del_stick para más información.
 *
 * @note Este periférico está intimamente relacionado con el periférico \e IOCON, particularmente para el manejo de
 * ciertos aspectos de hardware de los pines. Para más información, referirse a la sección @ref IOCON.
 *
 * # Funcionamiento de entradas
 *
 * Cuando un pin en particular es configurado como <em>entrada</em>, se podrá leer en el mismo, el estado de una
 * <em>señal digital externa</em>. Es importante que el usuario entienda que si la señal externa está en valores
 * que correspondan a la <em>zona prohibida</em> de los niveles de señal digital, la lectura del valor será
 * indefinida. En caso de necesitar leer valores <em>analógicos</em>, referirse al periférico @ref ADC en esta
 * misma documentación.
 *
 * @note Las entradas son configuradas como tales mediante la función @ref hal_gpio_set_dir y son
 * consultadas/leídas mediante las funciones @ref hal_gpio_read_pin y @ref hal_gpio_read_port.
 *
 * # Funcionamiento de salidas
 *
 * Cuando un pin en particular es configurado como <em>salida</em>, se podrán colocar en el mismo, <em>valores
 * digitales</em>. Es importante que el usuario entienda que no hay forma de colocar un valor \e analógico en
 * un pin mediante este periférico. Para dicha funcionalidad, referirse al periférico @ref DAC en esta misma
 * documentación.
 *
 * @note Las salidas son configuradas como tales mediante la función @ref hal_gpio_set_dir y son
 * manipuladas individualmente mediante las funciones @ref hal_gpio_set_pin, @ref hal_gpio_clear_pin y
 * @ref hal_gpio_toggle_pin.
 *
 * # Enmascaramiento de entradas/salidas
 *
 * El periférico \e GPIO dispone de la habilidad de \e enmascarar uno o más pines en particular. El hecho de
 * \e enmascarar un pin implica que:
 * 		- Si el mismo estaba configurado como salida: Se inhabilita la posibilidad de cambiar el estado actual
 * 		del pin.
 * 		- Si el mismo estaba configurado como entrada: Siempre se lee dicho pin como cero.
 * 		.
 *
 * @note Las máscaras son configuradas mediante las funciones @ref hal_gpio_set_mask_bits, @ref hal_gpio_lear_mask_bits y
 * @ref hal_gpio_toggle_mask_bits.
 *
 * # Lectura/Escritura de Puerto/Pin
 *
 * Al usuario puede serle de utilidad la posibilidad de leer/escribir el estado de un pin en particular, así
 * como también la posibilidad de leer/escribir varios pines a la vez. Si se encuentra en el segundo caso,
 * el periférico \e GPIO dispone de la habilidad de hacerlo, siempre y cuando los múltiples pines se encuentren
 * <b>en el mismo puerto</b>.
 *
 * Es importante también destacar, que las lecturas/escrituras respetarán las máscaras explicadas anteriormente,
 * por lo cual es recomendable configurar dicha funcionalidad a la hora de leer/escribir múltiples pines, y
 * asimismo es recomendable "liberar" los enmascaramientos una vez trabajados los pines necesarios.
 *
 * @note Las funciones que involucran máscaras son @ref hal_gpio_masked_set_port, @ref hal_gpio_masked_clear_port,
 * @ref hal_gpio_masked_toggle_port y @ref hal_gpio_masked_read_port.
 *
 * # Campos de aplicación típicos
 *
 * - Lectura de estado de variables externas digitales (switch, sensor digital)
 * - Escritura para accionar variables externas digitales (LED, motores)
 *
 * @{
 */

/**
 * @example Ejemplo_GPIO.c
 *
 * # Configuraciones
 *
 * El programa utiliza únicamente el LED RGB y el pulsador de usuario del stick de desarrollo.
 * - LED RGB - Rojo: Puerto 1, Pin 2
 * - LED RGB - Azul: Puerto 1, Pin 1
 * - Pulsador de usuario: Puerto 0, Pin 4
 *
 * Ver @ref acerca_del_stick para más información.
 *
 * Se configura el \e Systick para generar ticks cada 1 milisegundo.
 *
 * Se configura el pin del pulsador para utilizar lógica invertida en su hardware.
 *
 * # Funcionamiento del programa
 *
 * El \e Systick se utiliza para generar un titileo en el LED RGB \e rojo siempre y cuando el pulsador de usuario
 * no esté presionado. Mientras el pulsador de usuario se encuentre presionado, el LED RGB \e azul estará fijo y
 * el \e rojo quedará apagado. El titileo nunca deja de suceder, sino que se enmascara la salida correspondiente.
 * Esto implica que si el pulsador se deja de presionar en el momento en que el mismo tiene el LED RGB \e rojo
 * encendido, no se va a ver el LED RGB \e azul encender, dado el tipo de conexión en el stick de desarrollo
 * y los valores de tensión de LED.
 */

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

#include <stdint.h>

/** Macro para obtener número de puerto a partir de un puerto/pin */
#define		HAL_GPIO_PORTPIN_TO_PORT(x)		(x / 32)

/** Macro para obtener número de pin a partir de un puerto/pin */
#define		HAL_GPIO_PORTPIN_TO_PIN(x)		(x % 32)

/** Enumeraciones de puertos disponibles */
typedef enum
{
	HAL_GPIO_PORT_0 = 0, /**< Puerto 0 */
	HAL_GPIO_PORT_1 /**< Puerto 1 */
}hal_gpio_port_en;

/** Enumeraciones de puerto/pin disponibles */
typedef enum
{
	HAL_GPIO_PORTPIN_0_0 = 0, /**< Puerto 0, pin 0 */
	HAL_GPIO_PORTPIN_0_1, /**< Puerto 0, pin 1 */
	HAL_GPIO_PORTPIN_0_2, /**< Puerto 0, pin 2 */
	HAL_GPIO_PORTPIN_0_3, /**< Puerto 0, pin 3 */
	HAL_GPIO_PORTPIN_0_4, /**< Puerto 0, pin 4 */
	HAL_GPIO_PORTPIN_0_5, /**< Puerto 0, pin 5 */
	HAL_GPIO_PORTPIN_0_6, /**< Puerto 0, pin 6 */
	HAL_GPIO_PORTPIN_0_7, /**< Puerto 0, pin 7 */
	HAL_GPIO_PORTPIN_0_8, /**< Puerto 0, pin 8 */
	HAL_GPIO_PORTPIN_0_9, /**< Puerto 0, pin 9 */
	HAL_GPIO_PORTPIN_0_10, /**< Puerto 0, pin 10 */
	HAL_GPIO_PORTPIN_0_11, /**< Puerto 0, pin 11 */
	HAL_GPIO_PORTPIN_0_12, /**< Puerto 0, pin 12 */
	HAL_GPIO_PORTPIN_0_13, /**< Puerto 0, pin 13 */
	HAL_GPIO_PORTPIN_0_14, /**< Puerto 0, pin 14 */
	HAL_GPIO_PORTPIN_0_15, /**< Puerto 0, pin 15 */
	HAL_GPIO_PORTPIN_0_16, /**< Puerto 0, pin 16 */
	HAL_GPIO_PORTPIN_0_17, /**< Puerto 0, pin 17 */
	HAL_GPIO_PORTPIN_0_18, /**< Puerto 0, pin 18 */
	HAL_GPIO_PORTPIN_0_19, /**< Puerto 0, pin 19 */
	HAL_GPIO_PORTPIN_0_20, /**< Puerto 0, pin 20 */
	HAL_GPIO_PORTPIN_0_21, /**< Puerto 0, pin 21 */
	HAL_GPIO_PORTPIN_0_22, /**< Puerto 0, pin 22 */
	HAL_GPIO_PORTPIN_0_23, /**< Puerto 0, pin 23 */
	HAL_GPIO_PORTPIN_0_24, /**< Puerto 0, pin 24 */
	HAL_GPIO_PORTPIN_0_25, /**< Puerto 0, pin 25 */
	HAL_GPIO_PORTPIN_0_26, /**< Puerto 0, pin 26 */
	HAL_GPIO_PORTPIN_0_27, /**< Puerto 0, pin 27 */
	HAL_GPIO_PORTPIN_0_28, /**< Puerto 0, pin 28 */
	HAL_GPIO_PORTPIN_0_29, /**< Puerto 0, pin 29 */
	HAL_GPIO_PORTPIN_0_30, /**< Puerto 0, pin 30 */
	HAL_GPIO_PORTPIN_0_31, /**< Puerto 0, pin 31 */
	HAL_GPIO_PORTPIN_1_0, /**< Puerto 1, pin 0 */
	HAL_GPIO_PORTPIN_1_1, /**< Puerto 1, pin 1 */
	HAL_GPIO_PORTPIN_1_2, /**< Puerto 1, pin 2 */
	HAL_GPIO_PORTPIN_1_3, /**< Puerto 1, pin 3 */
	HAL_GPIO_PORTPIN_1_4, /**< Puerto 1, pin 4 */
	HAL_GPIO_PORTPIN_1_5, /**< Puerto 1, pin 5 */
	HAL_GPIO_PORTPIN_1_6, /**< Puerto 1, pin 6 */
	HAL_GPIO_PORTPIN_1_7, /**< Puerto 1, pin 7 */
	HAL_GPIO_PORTPIN_1_8, /**< Puerto 1, pin 8 */
	HAL_GPIO_PORTPIN_1_9, /**< Puerto 1, pin 9 */
	HAL_GPIO_PORTPIN_NOT_USED /**< Puerto/pin no utilizado */
}hal_gpio_portpin_en;

/** Enumeraciones de direcciones posibles para un puerto/pin */
typedef enum
{
	HAL_GPIO_DIR_INPUT = 0, /**< Puerto/pin como entrada */
	HAL_GPIO_DIR_OUTPUT /**< Puerto/pin como salida */
}hal_gpio_dir_en;

/**
 * @brief Inicializar un puerto
 * @param[in] port Puerto a inicializar
 */
void hal_gpio_init(hal_gpio_port_en port);

/**
 * @brief Fijar dirección de una GPIO
 * @param[in] portpin Número de puerto/pin a configurar
 * @param[in] dir Dirección deseada
 * @param[in] initial_state Estado inicial (aplica únicamente para salidas)
 *
 * @note Es importante recordar que el @ref IOCON controla aspectos de hardware del puerto/pin, por ejemplo la
 * inversión en la lógica del mismo, y que esta función no configura ninguno de esos aspectos. En caso de ser
 * necesario configurar dichas características, ver @ref hal_iocon_config_io.
 *
 * @see hal_gpio_portpin_en
 * @see hal_gpio_dir_en
 */
void hal_gpio_set_dir(hal_gpio_portpin_en portpin, hal_gpio_dir_en dir, uint8_t initial_state);

/**
 * @brief Fijar estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see hal_gpio_portpin_en
 */
void hal_gpio_set_pin(hal_gpio_portpin_en portpin);

/**
 * @brief Fijar estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_set Máscara de bits a fijar
 *
 * @see hal_gpio_portpin_en
 */
void hal_gpio_set_port(hal_gpio_port_en port, uint32_t bits_to_set);

/**
 * @brief Fijar estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_set Máscara de bits a fijar
 *
 * @see hal_gpio_portpin_en
 */
void hal_gpio_masked_set_port(hal_gpio_port_en port, uint32_t bits_to_set);

/**
 * @brief Limpiar estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see hal_gpio_portpin_en
 */
void hal_gpio_clear_pin(hal_gpio_portpin_en portpin);

/**
 * @brief Limpiar estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_clear Máscara de bits a limpiar
 *
 * @see hal_gpio_portpin_en
 */
void hal_gpio_clear_port(hal_gpio_port_en port, uint32_t bits_to_clear);

/**
 * @brief Limpiar estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_clear Máscara de bits a limpiar
 *
 * @see hal_gpio_portpin_en
 */
void hal_gpio_masked_clear_port(hal_gpio_port_en port, uint32_t bits_to_clear);

/**
 * @brief Invertir estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 *
 * @see hal_gpio_portpin_en
 */
void hal_gpio_toggle_pin(hal_gpio_portpin_en portpin);

/**
 * @brief Invertir estado de pines de un puerto (sin importar máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_toggle Máscara de bits a invertir
 *
 * @see hal_gpio_portpin_en
 */
void hal_gpio_toggle_port(hal_gpio_port_en port, uint32_t bits_to_toggle);

/**
 * @brief Invertir estado de pines de un puerto (teniendo en cuenta máscara)
 * @param[in] port Número de puerto a accionar
 * @param[in] bits_to_toggle Máscara de bits a invertir
 *
 * @see hal_gpio_portpin_en
 */
void hal_gpio_masked_toggle_port(hal_gpio_port_en port, uint32_t bits_to_toggle);

/**
 * @brief Leer el estado de una GPIO (sin importar máscara)
 * @param[in] portpin Número de puerto/pin a accionar
 * @return Estado actual del puerto/pin omitiendo máscara
 *
 * @see hal_gpio_portpin_en
 */
uint8_t hal_gpio_read_pin(hal_gpio_portpin_en portpin);

/**
 * @brief Leer estado de un puerto (sin importar máscara)
 * @param[in] port Puerto a consultar
 * @return Estado del puerto sin enmascarar
 *
 * @see hal_gpio_port_en
 */
uint32_t hal_gpio_read_port(hal_gpio_port_en port);

/**
 * @brief Leer estado de un puerto (teniendo en cuenta máscara)
 * @param[in] port Puerto a consultar
 * @return Estado del puerto contemplando la máscara asociada
 *
 * @see hal_gpio_port_en
 */
uint32_t hal_gpio_masked_read_port(hal_gpio_port_en port);

/**
 * @brief Fijar enmascaramiento de pines en un puerto
 * @param[in] port Puerto a fijar enmascaramiento
 * @param[in] mask Máscara de bits a fijar
 *
 * @see hal_gpio_port_en
 */
void hal_gpio_set_mask_bits(hal_gpio_port_en port, uint32_t mask);

/**
 * @brief Limpiar enmascaramiento de pines en un puerto
 * @param[in] port Puerto a limpiar enmascaramiento
 * @param[in] mask Máscara de bits a limpiar
 *
 * @see hal_gpio_port_en
 */
void hal_gpio_clear_mask_bits(hal_gpio_port_en port, uint32_t mask);

/**
 * @brief Invertir enmascaramiento de pines en un puerto
 * @param[in] port Puerto a invertir enmascaramiento
 * @param[in] mask Máscara de bits a invertir
 *
 * @see hal_gpio_port_en
 */
void hal_gpio_toggle_mask_bits(hal_gpio_port_en port, uint32_t mask);

#endif /* HAL_GPIO_H_ */

/**
 * @}
 */
