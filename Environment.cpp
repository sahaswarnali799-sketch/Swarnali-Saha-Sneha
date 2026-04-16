#include "Environment.h"
#include "Globals.h"
#include "Entity.h"
#include <cstdlib>
#include <cmath>
#include <cstdio>

Food foods[MAX_FOOD];
int  foodCount = 0;

Particle parts[MAX_PARTS];
int partCount = 0;

Obstacle obstacles[MAX_OBSTACLES];
int obstacleCount = 0;
int obstacleSpawnTimer = 0;
float obstacleHitFlash = 0.0f;
char  obstacleHitMsg[32] = "";
float obstacleHitMsgTimer = 0.0f;
float obstacleHitMsgX = 0, obstacleHitMsgY = 0;

// ══════════════════════════════════════════════════════════
//  FOOD
// ══════════════════════════════════════════════════════════
void levelFoodColor(int& r, int& g, int& b) {
	int palette[3][6][3] = {
		{ { 255, 68, 102 }, { 255, 136, 0 }, { 255, 238, 0 }, { 68, 255, 136 }, { 136, 255, 68 }, { 255, 170, 68 } },
		{ { 255, 102, 0 }, { 255, 204, 0 }, { 255, 153, 34 }, { 255, 68, 0 }, { 255, 238, 136 }, { 221, 136, 0 } },
		{ { 170, 68, 255 }, { 68, 136, 255 }, { 255, 68, 255 }, { 68, 255, 255 }, { 136, 68, 255 }, { 255, 136, 255 } }
	};
	int lv = (int)currentLevel - 1;
	int idx = rand() % 6;
	r = palette[lv][idx][0];
	g = palette[lv][idx][1];
	b = palette[lv][idx][2];
}

void spawnFood(float sx, float sy, bool big) {
	if (foodCount >= MAX_FOOD) return;
	Food& f = foods[foodCount++];
	if (sx < 0) {
		f.x = 50.0f + (float)(rand() % (WORLD_W - 100));
		f.y = 50.0f + (float)(rand() % (WORLD_H - 100));
	}
	else {
		f.x = sx + (float)((rand() % 80) - 40);
		f.y = sy + (float)((rand() % 80) - 40);
		f.x = fmaxf(10, fminf((float)(WORLD_W - 10), f.x));
		f.y = fmaxf(10, fminf((float)(WORLD_H - 10), f.y));
	}
	f.r = big ? 10.0f : 4.0f + (rand() % 5);
	f.value = big ? 3 : 1;
	f.big = big;
	f.pulse = (float)(rand() % 628) / 100.0f;
	levelFoodColor(f.cr, f.cg, f.cb);
}

void initFood() {
	foodCount = 0;
	int count = (currentLevel == FOREST) ? 40 : (currentLevel == DESERT) ? 55 : 70;
	for (int i = 0; i < count; i++) spawnFood();
}

// ══════════════════════════════════════════════════════════
//  PARTICLES
// ══════════════════════════════════════════════════════════
void spawnParticles(float x, float y, int cr, int cg, int cb, int count) {
	for (int i = 0; i < count; i++) {
		if (partCount >= MAX_PARTS) break;
		float angle = (float)(rand() % 628) / 100.0f;
		float spd = 1.0f + (rand() % 40) / 10.0f;
		Particle& p = parts[partCount++];
		p.x = x; p.y = y;
		p.vx = cosf(angle) * spd;
		p.vy = sinf(angle) * spd;
		p.life = 1.0f;
		p.r = cr; p.g = cg; p.b = cb;
	}
}

void updateParticles() {
	for (int i = partCount - 1; i >= 0; i--) {
		parts[i].x += parts[i].vx;
		parts[i].y += parts[i].vy;
		parts[i].vy -= 0.06f;
		parts[i].life -= 0.04f;
		if (parts[i].life <= 0) {
			parts[i] = parts[--partCount];
		}
	}
}

// ══════════════════════════════════════════════════════════
//  OBSTACLES
// ══════════════════════════════════════════════════════════
void getObstacleColor(int& r, int& g, int& b) {
	switch (currentLevel) {
	case FOREST: r = 220; g = 30;  b = 30;  break; // Red
	case DESERT: r = 0;   g = 30;  b = 130; break; // Navy blue
	case SPACE:  r = 140; g = 20;  b = 200; break; // Purple
	default:     r = 220; g = 30;  b = 30;  break;
	}
}

int getObstaclePenalty() {
	return (currentLevel == FOREST) ? 10 : (currentLevel == DESERT) ? 20 : 30;
}

void spawnObstacle() {
	if (obstacleCount >= MAX_OBSTACLES) return;
	Obstacle& o = obstacles[obstacleCount++];
	o.x = 80.0f + (float)(rand() % (WORLD_W - 160));
	o.y = 80.0f + (float)(rand() % (WORLD_H - 160));
	o.halfSize = 14.0f;
	getObstacleColor(o.cr, o.cg, o.cb);
	o.penalty = getObstaclePenalty();
	o.life = 300.0f + (float)(rand() % 200); // ~5-8 seconds at 60fps
	o.flashTimer = 0.0f;
	o.active = true;
}

void initObstacles() {
	obstacleCount = 0;
	obstacleSpawnTimer = 0;
	obstacleHitFlash = 0.0f;
	obstacleHitMsgTimer = 0.0f;
	for (int i = 0; i < 2; i++) spawnObstacle();
}

void updateObstacles() {
	for (int i = obstacleCount - 1; i >= 0; i--) {
		obstacles[i].flashTimer += 1.0f;
		obstacles[i].life -= 1.0f;
		if (obstacles[i].life <= 0) {
			obstacles[i] = obstacles[--obstacleCount];
		}
	}

	int spawnInterval = (currentLevel == FOREST) ? 180 : (currentLevel == DESERT) ? 120 : 90;
	int maxActive = (currentLevel == FOREST) ? 4 : (currentLevel == DESERT) ? 6 : 8;
	obstacleSpawnTimer++;
	if (obstacleSpawnTimer >= spawnInterval && obstacleCount < maxActive) {
		spawnObstacle();
		obstacleSpawnTimer = 0;
	}

	if (obstacleHitMsgTimer > 0) obstacleHitMsgTimer -= 1.0f;
	if (obstacleHitFlash > 0) obstacleHitFlash -= 0.05f;
}

void checkObstacleCollision() {
	if (!plrAlive) return;
	for (int i = obstacleCount - 1; i >= 0; i--) {
		Obstacle& o = obstacles[i];
		if (!o.active) continue;
		float dx = plrSeg[0].x - o.x;
		float dy = plrSeg[0].y - o.y;
		if (dx < 0) dx = -dx;
		if (dy < 0) dy = -dy;
		if (dx < o.halfSize + 6.0f && dy < o.halfSize + 6.0f) {
			score -= o.penalty;
			if (score < 0) score = 0;
			spawnParticles(o.x, o.y, o.cr, o.cg, o.cb, 15);
			obstacleHitFlash = 1.0f;
			sprintf(obstacleHitMsg, "-%d!", o.penalty);
			obstacleHitMsgTimer = 30.0f;
			obstacleHitMsgX = o.x;
			obstacleHitMsgY = o.y;
			if (plrLen > 12) plrLen -= 3;
			obstacles[i] = obstacles[--obstacleCount];
		}
	}
}
