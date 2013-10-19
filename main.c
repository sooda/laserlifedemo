#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "servos.h"
#include "display.h"
#include "lasers.h"
#include "demo.h"

void player_init() {
	// CTC, 256 prescaler, 50Hz
	TCCR3A = 0; // no OCn's, WGM11..10=0
	TCCR3B = _BV(WGM32) | _BV(CS32); // CTC, TOP=OCRnA, 256 prescale
	OCR3A = 1250; // 16e6/(256*50), 50Hz
	TIMSK3 = _BV(OCIE3A); // interrupt on compare match
}

static frametime_t frame;

ISR(TIMER3_COMPA_vect) {
	screen_update(frame++);
	PORTD ^= _BV(6);
}

int main() {
	clock_prescale_set(clock_div_1);
	DDRD |= _BV(6);
	lasers_init();
	player_init();
	servos_init();
	sei();
	for (;;)
		;
}
