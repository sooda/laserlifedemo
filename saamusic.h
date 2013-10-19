#ifndef SAAMUSIC_H
#define SAAMUSIC_H

#include "saa1099.h"

/* chip register address definitions */
/* see the chip data sheet for bit definitions */

/* amp regs */
#define A0 0x00
#define A1 0x01
#define A2 0x02
#define A3 0x03
#define A4 0x04
#define A5 0x05

/* freq regs */
#define F0 0x08
#define F1 0x09
#define F2 0x0a
#define F3 0x0b
#define F4 0x0c
#define F5 0x0d

/* octave regs */
#define O10 0x10
#define O32 0x11
#define O54 0x12

/* freq enable */
#define FE 0x14

/* noise regs */
#define NE 0x15
#define NN 0x16

/* envelope generator regs */
#define E0 0x18
#define E1 0x19

/* controll */
#define CTL 0x1c

/* note values */
#define B_NOTE 4
#define C_NOTE 35
#define CS_NOTE 62
#define D_NOTE 86
#define DS_NOTE 110
#define E_NOTE 132
#define F_NOTE 154
#define FS_NOTE 174
#define G_NOTE 192
#define GS_NOTE 210
#define A_NOTE 228
#define AS_NOTE 244

void saa1099music_init();
void saa1099_sync(void);
void saa1099_freq_reset(void);
void saa1099_sound_enable(void);
void saa1099_sound_disable(void);
void saa1099_set_amp(uint8_t channel, uint8_t valr, uint8_t vall);
void saa1099_freq_enable(uint8_t channel);
void saa1099_freq_disable(uint8_t channel);
void saa1099_noise_enable(uint8_t channel);
void saa1099_noise_disable(uint8_t channel);
void saa1099_set_freq(uint8_t channel, uint8_t note, int16_t fine);
void saa1099_set_noise_rate(uint8_t generator, uint8_t rate);
void saa1099_reg_init();

#endif
