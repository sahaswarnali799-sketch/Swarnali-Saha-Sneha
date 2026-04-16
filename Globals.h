#ifndef GLOBALS_H
#define GLOBALS_H

#include "Common.h"

extern State state;
extern Level currentLevel;

extern float camX, camY;

extern int  score;
extern int  bestScore[4];
extern bool unlocked[4];
extern int  foodsEaten;
extern int  botsKilled;

extern int  menuSel;
extern int  levelSel;
extern bool showCreds;

extern int   splashTimer;
extern float nebulaAngle;
extern float snakeWiggle;
extern float starX[150], starY[150], starSpd[150];
extern bool  starsInit;

extern unsigned int levelSelectTex;
extern bool         levelSelectTexLoaded;

extern float levelUpTimer;
extern char  levelUpText[64];
extern char  levelUpSub[64];
extern float gameOverTimer;

extern int   mouseX, mouseY;
extern bool  mouseActive;

#endif // GLOBALS_H
