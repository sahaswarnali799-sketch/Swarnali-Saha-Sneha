#ifndef RENDERER_H
#define RENDERER_H

#include "Common.h"
#include "Entity.h"

void wFilledCircle(float wx_, float wy_, float r);
void wFilledRect(float wx_, float wy_, float ww, float wh);
void wLine(float x1, float y1, float x2, float y2);

void drawForestBg();
void drawDesertBg();
void drawSpaceBg();

void drawFood();
void drawObstacles();
void drawOneSnake(Seg* segs, int len, int headR, int headG, int headB,
	int bodyR, int bodyG, int bodyB, bool isPlayer);
void drawAllSnakes();
void drawBotNames();
void drawParticles();
void drawMinimap();
void drawHUD();

void drawSplash();
void drawMainMenu();
void drawCredits();
void drawLevelSelect();
void drawGameOver();
void initStars();

#endif // RENDERER_H
