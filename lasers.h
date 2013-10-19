#ifndef LASERS_H
#define LASERS_H

#include <avr/io.h>

void lasers_init(void);
void lasers_on(uint8_t flags);
void lasers_off(uint8_t flags);

#define RED 1
#define GREEN 2

#endif
