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
