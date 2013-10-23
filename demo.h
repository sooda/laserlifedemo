#ifndef DEMO_H
#define DEMO_H

typedef uint16_t frametime_t;

#define SECTICKS 64
#define BLANKTIME 10
#define SCREENWID (SECTICKS-BLANKTIME)

#define SECONDS(s) ((s)*SECTICKS)

#define SCENE_INITBEAT SECONDS(8)
#define SCENE_GLITCH SECONDS(8 + 10)
#define SCENE_AMBULANCE SECONDS(8 + 10 + 8)
#define SCENE_MELODY SECONDS(8 + 10 + 8 + 30)
#define SCENE_ENDBEAT SECONDS(8 + 10 + 8 + 30 + 10)

#endif
