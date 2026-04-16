#include "Entity.h"
#include "Globals.h"
#include "Environment.h"
#include "Audio.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

Seg  plrSeg[MAX_SEGS];
int  plrLen = 20;
float plrAngle = 0.0f;
float plrSpeed = 3.0f;
bool plrAlive = true;
int  plrGrow = 0;

Bot bots[MAX_BOTS];
int botCount = 0;

const char* BOT_NAMES[] = { "Viper", "Neon", "Gremlin", "Slither", "Zap", "Cobra", "Ghost", "Pixel" };
int BOT_CR[] = { 255, 0, 0, 255, 255, 0, 255, 255 };
int BOT_CG[] = { 68, 187, 220, 102, 170, 255, 68, 255 };
int BOT_CB[] = { 102, 255, 255, 0, 0, 68, 255, 68 };

void initPlayer() {
	plrLen = 20;
	plrAngle = 0.0f;
	plrAlive = true;
	plrGrow = 0;
	plrSpeed = (currentLevel == FOREST) ? 3.0f : (currentLevel == DESERT) ? 3.2f : 3.5f;
	float sx = WORLD_W / 2.0f, sy = WORLD_H / 2.0f;
	for (int i = 0; i < plrLen; i++) {
		plrSeg[i].x = sx - i * 8.0f;
		plrSeg[i].y = sy;
	}
	camX = sx - W / 2.0f;
	camY = sy - H / 2.0f;
}

void spawnBot(int i) {
	Bot& b = bots[i];
	float x = 200.0f + (float)(rand() % (WORLD_W - 400));
	float y = 200.0f + (float)(rand() % (WORLD_H - 400));
	float ang = (float)(rand() % 628) / 100.0f;
	int len = 14 + rand() % 12;
	for (int j = 0; j < len && j < MAX_BOT_SEGS; j++) {
		b.segs[j].x = x - j * cosf(ang) * 8.0f;
		b.segs[j].y = y - j * sinf(ang) * 8.0f;
	}
	b.len = len;
	b.angle = ang;
	float spd = (currentLevel == FOREST) ? 2.2f : (currentLevel == DESERT) ? 2.8f : 3.4f;
	b.speed = spd * (0.85f + (rand() % 30) / 100.0f);
	b.alive = true;
	b.grow = 0;
	b.turnTimer = 0;
	b.colorR = BOT_CR[i % 8];
	b.colorG = BOT_CG[i % 8];
	b.colorB = BOT_CB[i % 8];
	strncpy(b.name, BOT_NAMES[i % 8], 15);
	b.name[15] = '\0';
}

void initBots() {
	botCount = (currentLevel == FOREST) ? 4 : (currentLevel == DESERT) ? 6 : 8;
	for (int i = 0; i < botCount; i++) spawnBot(i);
}

void killPlayer() {
	if (!plrAlive) return;
	plrAlive = false;
	if (score > bestScore[currentLevel]) bestScore[currentLevel] = score;
	if (currentLevel == 1 && bestScore[1] >= 100) unlocked[2] = true;
	if (currentLevel == 2 && bestScore[2] >= 300) unlocked[3] = true;
	spawnParticles(plrSeg[0].x, plrSeg[0].y,
		(currentLevel == FOREST) ? 30 : (currentLevel == DESERT) ? 255 : 0,
		(currentLevel == FOREST) ? 220 : (currentLevel == DESERT) ? 160 : 200,
		(currentLevel == FOREST) ? 60 : (currentLevel == DESERT) ? 0 : 255, 25);
	stopMusic();
	gameOverTimer = 60.0f;
}

void killBot(int bi) {
	Bot& b = bots[bi];
	if (!b.alive) return;
	botsKilled++;
	for (int j = 0; j < b.len; j += 3)
		spawnFood(b.segs[j].x, b.segs[j].y, j < 6);
	spawnParticles(b.segs[0].x, b.segs[0].y, b.colorR, b.colorG, b.colorB, 18);
	b.alive = false;
	b.grow = 0;
	b.turnTimer = -180.0f;
}
