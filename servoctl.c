#include <avr/io.h>
#include <avr/interrupt.h>

#define OCR_x OCR1A
#define OCR_y OCR1B
#define OCR_top ICR1

void servos_init(void) {
#if 0
	// CTC mode, prescale 256
	// clear oc0a/oc0b on compare match
	TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM01);
	TCCR0B = _BV(CS02);
	// 64*256/16e6 = 1.024ms
	// 128*256/16e6 = 2.048ms
	// servo pos thus 0..63
#endif
	// CTC mode, prescale 256 TODO prescale 64, multiply 0..255 ocr by 2 in update()
	// clear oc0a/oc0b on compare match
	// TOP at ICR3
	TCCR1A = 0;//WHY U NO WORK (level always at 0V) _BV(COM1A1) | _BV(COM1B1); // WGM11..10=0
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12); // CTC, TOP at ICR1
	TIMSK1 = _BV(OCIE1A) | _BV(OCIE1B) | _BV(TOIE1);
	DDRB |= _BV(5) | _BV(6); // OC1A, OC1B
	OCR_x = 0xff;
	OCR_y = 0xff;
	OCR_top = 0xffff;
}

ISR(TIMER1_OVF_vect) {
	// nothing to do?
}
ISR(TIMER1_COMPA_vect) {
	PORTB &= ~_BV(5);
}
ISR(TIMER1_COMPB_vect) {
	PORTB &= ~_BV(6);
}

// y, x
// y 0 bottom, x 0 right
void servos_update(uint8_t x, uint8_t y) {
	x = 63 - x; // servo inverted
	PORTB |= _BV(5) | _BV(6);
	OCR_y = 64 + x;
	OCR_x = 64 + y;
	TCNT1 = 0;
}


