#include <avr/io.h>
#include "lasers.h"

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
