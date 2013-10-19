#include <stdint.h>
#include "servos.h"
#include "lasers.h"
#include "analyze.h"
#include "demo.h"

#define TYPE_PICTURE 1
#define TYPE_FUNC 2
#define TYPE_LEDS 3

struct frame {
	frametime_t lastframe;
	uint8_t type;
	//union {
		uint8_t buf[SCREENWID];
		void (*render)(frametime_t, uint8_t*, uint8_t*);
	//};
	uint8_t lightflags;
};

static void loudness(frametime_t time, uint8_t *x, uint8_t *y) {
	(void)time;
	(void)x;
	*y = 32 + analyze_loudness();
}

#include "sincos.h"

static void circle(frametime_t time, uint8_t *x, uint8_t *y, uint8_t sz) {
	uint8_t subpos = time & 63;
	*x = 32 + ((mycos[subpos] * sz) >> 8);
	*y = 32 + ((mysin[subpos] * sz) >> 8);
	*x = 32 + ((mycos[subpos] / sz));
	*y = 32 + ((mysin[subpos] / sz));
}
static void ambulance(frametime_t time, uint8_t *x, uint8_t *y) {
	DDRF |= _BV(4)|_BV(5);
	if (time & 32) {
		PORTF |= _BV(4);
		PORTF &= ~_BV(5);
	} else {
		PORTF |= _BV(5);
		PORTF &= ~_BV(4);
	}
	if (time >= SCENE_AMBULANCE - SECONDS(1)) {
		PORTF &= ~(_BV(4)|_BV(5));
	}
	*x = 32;
	*y = 32;
}

static void parsemzk(frametime_t time, uint8_t *x, uint8_t *y) {
	*y = (((time & 7) << 3) | (time >> 3)) & 63;
}

static void circles(frametime_t time, uint8_t *x, uint8_t *y) {
	circle(time, x, y, (time >> 4) & 0xff);
	circle(time, x, y, 3 + 2 * !!(time & 8));
	*x &= 63;
	*y &= 63;
}

static struct frame frames[] = {
	{
		.lastframe = SCENE_INITBEAT,
		.type = TYPE_PICTURE,
		.buf =  {
#include "frame0.inc"
		},
		.lightflags = RED
	},
	{
		.lastframe = SCENE_GLITCH,
		.render = circles,
		.type = TYPE_FUNC,
		.lightflags = GREEN,
	},
	{
		.lastframe = SCENE_AMBULANCE,
		.render = ambulance,
		.type = TYPE_FUNC,
		.lightflags = RED,
	},
	{
		.lastframe = SCENE_MELODY,
		.render = parsemzk,
		.type = TYPE_FUNC,
		.lightflags = RED,
	},
	{
		.lastframe = SCENE_ENDBEAT,
		.type = TYPE_PICTURE,
		.buf =  {
#include "frame0.inc"
		},
		.lightflags = RED|GREEN
	},
};

static struct frame *currframe = &frames[0];

static uint8_t curridx;

void update(uint8_t idx, frametime_t frameno) {
	uint8_t x = idx, y = 32;
	switch (currframe->type) {
		case TYPE_PICTURE:
			y = currframe->buf[x];
			break;
		case TYPE_FUNC:
			currframe->render(frameno, &x, &y);
			break;
		default:
			y = x;
			break;
	}
	servos_update(x, y);
}

void screen_update(frametime_t frameno) {
	static frametime_t laststop;
	static frametime_t frameidx;
	if (curridx < SCREENWID) {
		lasers_on(currframe->lightflags);
		update(curridx, frameno);
		laststop = frameno;
	} else {
		frametime_t nextstart = laststop + BLANKTIME;
		if (nextstart >= currframe->lastframe)
			currframe++;
		if (currframe->type == TYPE_PICTURE) {
			lasers_off(RED|GREEN);
			update(0, nextstart);
		} else {
			update(curridx, frameno);
		}
	}
#if 1
	if (++curridx == SECTICKS) {
		curridx = 0;
		frameidx++;
		lasers_off(RED|GREEN);
	}
#endif
}
