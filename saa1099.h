#ifndef SAA1099_H
#define SAA1099_H

#include <stdint.h>

/* io pin defs */
#define CTRLDDR DDRD
#define CTRLPORT PORTD
#define CLK_PIN PD0 /* OC0B is on PORTD too */
#define A0_PIN PD1 /* low: ctl; high: adr */
#define CS_PIN PD2 /* active low */
#define WR_PIN PD3 /* active low */
/* and data bus on PORTC */
#define DATAPORT PORTC
#define DATADDR DDRC

void saa1099_init(); /* init the io and chip regs */
void saa1099_set_reg(uint8_t reg, uint8_t val);
void saa1099_set_ctl(uint8_t byte);
void saa1099_set_adr(uint8_t byte);
void saa1099_clear_regs();
void saa1099_set_freq(uint8_t channel, uint8_t note, int16_t fine);
void saa1099_set_amp(uint8_t channel, uint8_t valr, uint8_t vall);
#endif
