#ifndef ENTITY_H
#define ENTITY_H

#include "Common.h"

// ══════════════════════════════════════════════════════════
//  SNAKE
// ══════════════════════════════════════════════════════════
#define MAX_SEGS   2000
#define MAX_BOTS   8
#define MAX_BOT_SEGS 300

struct Seg { float x, y; };

extern Seg  plrSeg[MAX_SEGS];
extern int  plrLen;
extern float plrAngle;
extern float plrSpeed;
extern bool plrAlive;
extern int  plrGrow;

struct Bot {
	Seg   segs[MAX_BOT_SEGS];
	int   len;
	float angle;
	float speed;
	bool  alive;
	int   grow;
	float turnTimer;
	int   colorR, colorG, colorB;
	char  name[16];
};

extern Bot bots[MAX_BOTS];
extern int botCount;

extern const char* BOT_NAMES[];
extern int BOT_CR[];
extern int BOT_CG[];
extern int BOT_CB[];

void initPlayer();
void spawnBot(int i);
void initBots();
void killPlayer();
void killBot(int bi);

#endif // ENTITY_H
