#include <avr/io.h>
#include <util/delay.h>
#include "saa1099.h"

static inline void saa1099_clk_tmr_init()
{
    TCCR0A = (1<<COM0B0) | (1<<WGM01); /* output compare match toggle A */
    TCCR0B = (1<<CS00); /* no prescaler */

    OCR0B = 0; /* in the end 16MHz gets divided by two because it's toggle*/
    CTRLDDR |= (1<<CLK_PIN); /* clock is output */
}

static inline void saa1099_io_init()
{
    CTRLDDR |= (1<<WR_PIN) | (1<<CS_PIN) | (1<<A0_PIN);
	DATADDR = 0xff;
    CTRLPORT |= (1<<CS_PIN) | (1<<WR_PIN);
}

static inline void saa1099_bus_set(uint8_t byte)
{
	DATAPORT = byte;
}

inline void saa1099_set_adr(uint8_t byte)
{
    CTRLPORT |= (1<<A0_PIN);
    CTRLPORT &= ~(1<<CS_PIN);
    saa1099_bus_set(byte & 0x1f);
    CTRLPORT &= ~(1<<WR_PIN);
    CTRLPORT |= (1<<WR_PIN);
    CTRLPORT |= (1<<CS_PIN);
}

inline void saa1099_set_ctl(uint8_t byte)
{

    CTRLPORT &= ~(1<<A0_PIN);
    CTRLPORT &= ~(1<<CS_PIN);
    saa1099_bus_set(byte);
    CTRLPORT &= ~(1<<WR_PIN);
	/* not using dtack, so compensate manually */
	_delay_us(3);
    CTRLPORT |= (1<<WR_PIN);
    CTRLPORT |= (1<<CS_PIN);
}

inline void saa1099_set_reg(uint8_t reg, uint8_t val)
{
    saa1099_set_adr(reg);
    saa1099_set_ctl(val);
}

inline void saa1099_clear_regs()
{
    uint8_t i;
    for (i = 0; i < 0x1f; ++i) {
        saa1099_set_reg(i, 0);
    }
}

inline void saa1099_init()
{
    saa1099_io_init();
    saa1099_clk_tmr_init();
}
