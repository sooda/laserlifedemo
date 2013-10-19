#include <stdint.h>
#include "servos.h"
#include "lasers.h"
#include "analyze.h"
#include "demo.h"

#define TYPE_PICTURE 1
#define TYPE_FUNC 2

struct frame {
	frametime_t lastframe;
	uint8_t type;
	union {
		uint8_t buf[SECTICKS];
		void (*render)(frametime_t, uint8_t*, uint8_t*);
	};
	uint8_t lightflags;
};

void loudness(frametime_t time, uint8_t *x, uint8_t *y) {
	(void)time;
	(void)x;
	*y = 32 + analyze_loudness();
}

void circle(frametime_t time, uint8_t *x, uint8_t *y) {
	uint8_t subpos = time & 63;
	uint8_t stage = (time & 0xff) >> 6;
	switch (stage) {
		case 0: *x /= 2; *y = 32 - *x; break;
		case 1: *x /= 2; *y = 32 - *x; break;
		case 2: *x /= 2; *y = 32 - 21; break;
		case 3: *x = *y = 0; break;
		default: return;
	}
}

static struct frame frames[] = {
	{
		.lastframe = SECONDS(0),
		.type = TYPE_PICTURE,
		.buf =  {
#include "frame0.inc"
		},
		.lightflags = RED
	},
	{
		.lastframe = SECONDS(3),
		.type = TYPE_FUNC,
		.render = loudness,
		.lightflags = RED,
	},
	{
		.lastframe = -1,
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
		lasers_off(RED|GREEN);
		frametime_t nextstart = laststop + BLANKTIME;
		if (nextstart >= currframe->lastframe)
			currframe++;
		update(0, nextstart);
	}
#if 1
	if (++curridx == SECTICKS) {
		curridx = 0;
		frameidx++;
	}
#endif
}
