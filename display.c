#include <stdint.h>
#include "servos.h"
#include "lasers.h"
#include "analyze.h"
#include "demo.h"

#define TYPE_PICTURE 1
#define TYPE_MUSIC 2

struct frame {
	frametime_t lastframe;
	uint8_t type;
	uint8_t buf[SECTICKS];
};

static struct frame frames[] = {
	{
		.lastframe = SECONDS(3),
		.type = TYPE_PICTURE,
		.buf =  {
#include "frame0.inc"
		}
	},
	{
		.lastframe = -1,
		.type = TYPE_MUSIC,
		.buf = {}
	},
#if 0
	{
		.lastframe = -1,
		.type = TYPE_PICTURE,
		.buf = 
#include "frame1.inc"
	},
#endif
};

static struct frame *currframe = &frames[0];

static uint8_t curridx;

void screen_update(frametime_t frameno) {
	if (curridx < SCREENWID) {
		uint8_t sz;
		switch (currframe->type) {
			case TYPE_PICTURE:
				sz = currframe->buf[curridx];
				break;
			case TYPE_MUSIC:
				sz = 32 + analyze_loudness();
				break;
			default:
				sz = curridx;
				break;
		}
		servos_update(curridx, sz);
		lasers_on(RED);
	} else {
		servos_update(0, 32);
		lasers_off(RED|GREEN);
	}
#if 1
	if (++curridx == SECTICKS) {
		curridx = 0;
		if (frameno == currframe->lastframe)
			currframe++;
	}
#endif
}
