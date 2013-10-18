#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "servos.h"

void player_init() {
	// CTC, 256 prescaler, 50Hz
	TCCR3A = 0; // no OCn's, WGM11..10=0
	TCCR3B = _BV(WGM32) | _BV(CS32); // CTC, TOP=OCRnA, 256 prescale
	OCR3A = 1250; // 16e6/(256*50), 50Hz
	TIMSK3 = _BV(OCIE3A); // interrupt on compare match
}

uint8_t x;
uint8_t a,b=63;

ISR(TIMER3_COMPA_vect) {
	if (++x == 50) {
		x = 0;
		a = 63 - a;
		b = 63 - a;
	}
	servos_update(a, b);
	PORTD ^= _BV(6);
}

int main() {
	clock_prescale_set(clock_div_1);
	DDRD |= _BV(6);
	player_init();
	servos_init();
	sei();
	for (;;) {
		_delay_ms(500);
		//PORTD ^= _BV(6);
	}
}
