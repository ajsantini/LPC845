/**
 * Programa para probar las distintas funcionalidades de la libreria Infotronic v2.
 *
 * Salidas digitales: Cada un segundo encenderan con el patron de un contador binario.
 * Termistor: Se medira la temperatura cada medio segundo.
 * Display 7 segmentos: Se informara la temperatura medida en °C en el display verde (Display 1).
 * LCD: Se informara la ultima tecla presionada en el teclado matricial 2x3.
 */

#include <cr_section_macros.h>
#include <infotronic/infotronic.h>

#define		CRYSTAL_FREQ			12e6

#define		EVENT_NUMBER_5DEC		0
#define		EVENT_NUMBER_1SEG		1

static void timer_5dec_callback(uint8_t event_number);
static void timer_1seg_callback(uint8_t event_number);

int main(void)
{
	char mensaje_LCD[17] = "Ultima tecla: -";

	infotronic_init(CRYSTAL_FREQ);

	infotronic_timer_start(EVENT_NUMBER_5DEC, INFOTRONIC_DEC(5), timer_5dec_callback, 1);
	infotronic_timer_start(EVENT_NUMBER_1SEG, INFOTRONIC_SEC(1), timer_1seg_callback, 1);

	infotronic_display_on(0);
	infotronic_display_on(1);

	infotronic_LCD_write("Infotronic--V2.0", INFOTRONIC_RENGLON_1, 0);
	infotronic_LCD_write(mensaje_LCD, INFOTRONIC_RENGLON_2, 0);

    while(1)
    {
    	uint8_t tecla_presionada = infotronic_get_key();

    	infotronic_timer_loop();

    	if(tecla_presionada != INFOTRONIC_NO_KEY)
    	{
    		mensaje_LCD[15] = tecla_presionada + '0';
    	}
    }

    return 0;
}

static void timer_5dec_callback(uint8_t event_number)
{
	int32_t temperatura_medida = infotronic_termometro_read();

	infotronic_display_write(1, temperatura_medida / 1000);
}

static void timer_1seg_callback(uint8_t event_number)
{
	static uint8_t contador = 0;

	uint8_t i;

	for(i = 0; i < 4; i++)
	{
		if((contador & (1 << i)) != 0)
		{
			infotronic_relay_activate(i);
		}
		else
		{
			infotronic_relay_deactivate(i);
		}
	}

	contador = (contador + 1) % 16;
}
