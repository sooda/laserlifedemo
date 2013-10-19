#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "servos.h"
#include "demo.h"

void player_init() {
	// CTC, 256 prescaler, 50Hz
	TCCR3A = 0; // no OCn's, WGM11..10=0
	TCCR3B = _BV(WGM32) | _BV(CS32); // CTC, TOP=OCRnA, 256 prescale
	OCR3A = 1250; // 16e6/(256*50), 50Hz
	TIMSK3 = _BV(OCIE3A); // interrupt on compare match
}

uint8_t x;
uint8_t a,b=63;

#define RED 1
#define GREEN 2

void lasers_init(void) {
	DDRF |= _BV(6) | _BV(7);
}

void lasers_on(uint8_t flags) {
	if (flags & RED)
		PORTF |= _BV(6);
	if (flags & GREEN)
		PORTF |= _BV(7);
}

void lasers_off(uint8_t flags) {
	if (flags & RED)
		PORTF &= ~_BV(6);
	if (flags & GREEN)
		PORTF &= ~_BV(7);
}

#define ENDPOS 43

ISR(TIMER3_COMPA_vect) {
	x++;
	if (x < ENDPOS) {
		a++;
		b--;
	}
	if (x == 1) {
		lasers_on(RED|GREEN);
	} else if (x == ENDPOS) {
		a = 0;
		b = 63;
		lasers_off(RED|GREEN);
	} else if (x == SECTICKS) {
		x = 0;
	}
	servos_update(32 - 4 + 8 * !!(a & 4), b);
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
	for (;;) {
		_delay_ms(500);
		//PORTD ^= _BV(6);
	}
}
