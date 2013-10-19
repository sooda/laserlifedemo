#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "servos.h"
#include "display.h"
#include "lasers.h"
#include "saamusic.h"
#include "music.h"
#include "demo.h"

static void player_init() {
	// CTC, 256 prescaler, 50Hz
	TCCR3A = 0; // no OCn's, WGM11..10=0
	TCCR3B = _BV(WGM32) | _BV(CS32); // CTC, TOP=OCRnA, 256 prescale
	OCR3A = 1250; // 16e6/(256*50), 50Hz
	OCR3A = 976; // ~64 hz
	TIMSK3 = _BV(OCIE3A); // interrupt on compare match
}

static frametime_t frame;

static void initsound() {
	saa1099_reg_init();
	saa1099_set_amp(0, 0xf, 0xf);
	saa1099_freq_enable(0);
	saa1099_sound_enable();
}
ISR(TIMER3_COMPA_vect) {
	PORTD ^= _BV(6);
	screen_update(frame);
#if 1
	if (frame <= SCENE_INITBEAT) {
		heartbeat_tf(frame);
	} else if (frame <= SCENE_GLITCH) {
		glitch(frame - SCENE_INITBEAT);
	} else if (frame <= SCENE_AMBULANCE) {
		ambulance(frame);
	} else if (frame <= SCENE_MELODY) {
		melody(frame - SCENE_AMBULANCE);
	} else if (frame <= SCENE_ENDBEAT) {
		heartbeat_tf(frame);
	}
	if (frame == SCENE_INITBEAT
			|| frame == SCENE_GLITCH
			|| frame == SCENE_AMBULANCE
			|| frame == SCENE_MELODY
			|| frame == SCENE_ENDBEAT)
		initsound();
#else
	melody(frame);
#endif
	saa1099_sync();
	frame++;
}


int main() {
	clock_prescale_set(clock_div_1);
	DDRD |= _BV(6);
	lasers_init();
	player_init();
	servos_init();
	saa1099music_init();
	initsound();

	sei();
	for (;;)
		;
}
