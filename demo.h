#ifndef DEMO_H
#define DEMO_H

/*
 * Platform: teensy++ 2.0 (AT90USB1286) @ 16 MHz
 * SAA1099 clock: 8MHz from 8bit timer X
 * Servo pulses from 16bit timer 1
 * System 50Hz: 16bit timer 3
 */

typedef uint16_t frametime_t;

#define SECTICKS 64
#define BLANKTIME 10
#define SCREENWID (SECTICKS-BLANKTIME)

#define SECONDS(s) ((s)*SECTICKS)

#endif
