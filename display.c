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

#include "sincos.h"

void circle(frametime_t time, uint8_t *x, uint8_t *y) {
	uint8_t subpos = time & 63;
	*x = 32 + mycos[subpos] / 16;
	*y = 32 + mysin[subpos] / 16;
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
		.lastframe = SECONDS(30),
		.type = TYPE_FUNC,
		.render = circle,
		.lightflags = RED,
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
	}
#endif
}
