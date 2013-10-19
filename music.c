#include "saamusic.h"

void ambulance(uint16_t t)
{
	if (t & 64) {
		saa1099_set_freq(0, 7+(36), -55);
	}
	else {
		saa1099_set_freq(0, 5+(36), -55); 
	}
}

void heartbeat_tf(uint16_t t)
{
	char list[] = 
	{
		'e', 0, 0, 0, 0, 0, 0,
		'd', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		'e', 0, 0, 0, 0, 0, 0,
		'd', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 
	};

	saa1099_set_freq(0, 40, 0);

	switch (list[t % sizeof(list)]) {
		case 'e':
			saa1099_freq_enable(0);
			break;
		case 'd':
			saa1099_freq_disable(0);
			break;
		default:
			break;
	}
}

void glitch(uint16_t t)
{
	saa1099_set_reg(0x14, (t/64) ^ (t>>5));
	saa1099_set_freq(0, (0x75^(t/(t&0x04)))>>1, 0);
	saa1099_set_freq(1, t&(2>>t), 0);
	saa1099_set_reg(0x15, t^(t*t));
	saa1099_set_amp(1, t|(2<<t), 0);
	saa1099_set_amp(2, t<<t, t<<t);
	saa1099_set_freq(2, t ^ (t^0x75), t^(t>>5));
	saa1099_set_reg(0x16, t);
	saa1099_set_amp(3, t|(5<<(t^7)), 0);
	saa1099_set_freq(3, 1<<(t/45), (t/34));
}
uint8_t fade_in(uint8_t rate, uint8_t max, uint16_t t)
{
	if (t > 960) { // (t+1)/rate > max || t/rate > max){
		return 0xF;
	}
	else {
		return t>>6;//t/64;
	}
}
uint8_t arp(uint8_t base_note, uint8_t step0, uint8_t step1, uint16_t t)
{
	switch(t%3) {
		case 0:
			return base_note;
		case 1:
			return base_note + step0;
		case 2:
			return base_note + step1;
		default:
			return base_note;
	}
}
void melody(uint16_t t)
{
	uint8_t i;
#if 1
	if ( t <= 1920) {
		uint8_t f = t > 960 ? 0xf : t>>6;
		saa1099_set_freq(0, 36+7, 0);
		//saa1099_set_freq(0, arp(36, 4, 7, t), 0);
		saa1099_set_amp(0, f/*fade_in(64, 0xf, t)*/, f/*fade_in(64, 0xf, t)*/);
	}
	else {
		saa1099_freq_disable(0);
	}
#endif
#if 0
	if (t >= 960 && t <= 1920) {
		for (i = 1; i <= 5; ++i) {
			saa1099_freq_enable(i);
			saa1099_set_amp(i, fade_in(64, 0xf, t-960), fade_in(64, 0xf, t-960));
		}
		saa1099_set_freq(5, 24, 0);
		saa1099_set_freq(4, 33, 0);
		saa1099_set_freq(1, 36, 0);
		saa1099_set_freq(2, 40, 0);
		saa1099_set_freq(3, 43, 0);
	}
	else {
		for (i = 1; i <= 5; ++i) {
			saa1099_freq_disable(i);
		}
	}
#endif
}
