#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "servos.h"
#include "lasers.h"
#include "demo.h"

void player_init() {
	// CTC, 256 prescaler, 50Hz
	TCCR3A = 0; // no OCn's, WGM11..10=0
	TCCR3B = _BV(WGM32) | _BV(CS32); // CTC, TOP=OCRnA, 256 prescale
	OCR3A = 1250; // 16e6/(256*50), 50Hz
	TIMSK3 = _BV(OCIE3A); // interrupt on compare match
}

uint8_t x;
uint8_t a, b;

ISR(TIMER3_COMPA_vect) {
	x++;
	if (x < SCREENWID) {
		a++;
		b++;
		lasers_on(RED|GREEN);
		servos_update(a, 32 - 4 + 8 * !!(b & 4));
	} else {
		servos_update(0, 32);
		a = 0;
		b = 0;
		lasers_off(RED|GREEN);
	}
	if (x == SECTICKS) {
		x = 0;
	}
	PORTD ^= _BV(6);
}

int main() {
	clock_prescale_set(clock_div_1);
	DDRD |= _BV(6);
	lasers_init();
	lasers_on(3);
	player_init();
	servos_init();
	sei();
	for (;;)
		;
}
