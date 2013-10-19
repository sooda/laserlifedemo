#include "saamusic.h"
#include <avr/pgmspace.h>

const PROGMEM uint8_t octave_lookup[] =
{
    B_NOTE, C_NOTE, CS_NOTE, D_NOTE, DS_NOTE,E_NOTE, 
    F_NOTE, FS_NOTE, G_NOTE, GS_NOTE, A_NOTE, AS_NOTE
};

static uint8_t regs[0x1f];

void saa1099_cache_reg(uint8_t reg, uint8_t val)
{
    /*saa1099_set_reg(reg, val);*/
    regs[reg] = val;
}

void saa1099_reg_init()
{
    uint8_t i;
    /* init the registers */
    for (i = 0; i < 0x1f; ++i) {
        saa1099_cache_reg(i, 0);
        regs[i] = 0; // wat?
    }
}

void saa1099music_init()
{
    saa1099_init();
    saa1099_reg_init();
}

static uint8_t split_assign(uint8_t orig, uint8_t val, uint8_t half)
{
    uint8_t reg_mask;
    reg_mask = 0xf<<(4*half);
    return (orig & reg_mask) | (val<<(4*((~half)&1)));
}


static void split_reg_assign(uint8_t reg, uint8_t val, uint8_t half)
{
    regs[reg] = split_assign(regs[reg], val, ~half);
}

void saa1099_sync(void)
{
    uint8_t i;
    for (i = 0; i < 0x1f; ++i) {
        saa1099_set_reg(i, regs[i]);
    }
}

void saa1099_freq_reset(void)
{
    regs[CTL] |= 1;
    saa1099_sync();
    regs[CTL] &= ~(1);
    saa1099_sync();
}

void saa1099_sound_enable(void)
{
    regs[CTL] = 1;
}

void saa1099_sound_disable(void)
{
    regs[CTL] = 0;
}

void saa1099_set_amp(uint8_t channel, uint8_t valr, uint8_t vall)
{
    regs[channel] = (valr<<4) | (vall & 0xf);
}

void saa1099_freq_enable(uint8_t channel)
{
    regs[FE] |= (1<<channel);
}

void saa1099_freq_disable(uint8_t channel)
{
    regs[FE] &= ~(1<<channel);
}

void saa1099_noise_enable(uint8_t channel)
{
    regs[NE] |= (1<<channel);
}

void saa1099_noise_disable(uint8_t channel)
{
    regs[NE] &= ~(1<<channel);
}

void saa1099_set_freq(uint8_t channel, uint8_t note, int16_t fine)
{
    /* TODO fix finetune and octave crossing */
    uint8_t oct_reg;
    regs[F0 + channel] = pgm_read_byte(octave_lookup + (note + 1) % 12) + fine;
    oct_reg = O10 + (channel / 2);
    split_reg_assign(oct_reg, (note + 1) / 12, channel & 1);
}

void saa1099_set_noise_rate(uint8_t generator, uint8_t rate)
{
    split_reg_assign(NN, rate, generator);
}
