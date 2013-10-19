#include "servos.h"
#include "demo.h"

#define TYPE_PICTURE 1
#define TYPE_MUSIC 2

struct frame {
	frametime_t endframe;
	uint8_t type;
	uint8_t buf[SECTICKS];
};

static struct frame frames[] = {
	{
		.endframe = -1,
		.type = TYPE_PICTURE,
		.buf = 
#include "frame0.inc"
	},
	{
		.endframe = -1,
		.type = TYPE_MUSIC,
		.buf = {}
	},
	{
		.endframe = -1,
		.type = TYPE_PICTURE,
		.buf = 
#include "frame1.inc"
	},
};

static struct frame *currframe = &frames[0];

static frametime_t lastupdate;
static uint8_t curridx;

static void getheight(void) {
}

void screen_update(frametime_t frameno) {
	if (currwid < SCREENWID) {
		uint8_t sz;
		switch (curridx->type) {
			case TYPE_PICTURE:
				sz = currframe->buf[curridx];
			case TYPE_MUSIC:
				sz = music_loudness();
			default:
				sz = 0;
		}
		servos_update(sz, x);
		lasers_on(RED);
	} else {
		servos_update(0, 0);
		lasers_off(RED|GREEN);
	}
	if (++curridx == SCREENWID) {
		curridx = 0;
		if (frameno == currframe->endframe)
			currframe++;
	}
}
