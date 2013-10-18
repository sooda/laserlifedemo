#include <avr/io.h>

#define OCR_x OCRTODO1
#define OCR_y OCRTODO2

void servos_init(void) {
	// CTC mode, prescale 256
	// clear oc0a/oc0b on compare match
	TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01);
	TCCR0B = _BV(CS02);
	// 64*256/16e6 = 1.024ms
	// 128*256/16e6 = 2.048ms
	// servo pos thus 0..63
}

void servos_update(uint8_t x, uint8_t y) {
	OCR_x = 64 + x;
	OCR_y = 64 + y;
}


