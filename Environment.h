#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "Common.h"

// ══════════════════════════════════════════════════════════
//  FOOD
// ══════════════════════════════════════════════════════════
#define MAX_FOOD   80

struct Food {
	float x, y, r;
	int   cr, cg, cb;
	int   value;
	float pulse;
	bool  big;
};

extern Food foods[MAX_FOOD];
extern int  foodCount;

void levelFoodColor(int& r, int& g, int& b);
void spawnFood(float sx = -1.0f, float sy = -1.0f, bool big = false);
void initFood();

// ══════════════════════════════════════════════════════════
//  PARTICLES
// ══════════════════════════════════════════════════════════
#define MAX_PARTS  60

struct Particle {
	float x, y, vx, vy, life;
	int   r, g, b;
};

extern Particle parts[MAX_PARTS];
extern int partCount;

void spawnParticles(float x, float y, int cr, int cg, int cb, int count);
void updateParticles();

// ══════════════════════════════════════════════════════════
//  OBSTACLES  — square hazards
// ══════════════════════════════════════════════════════════
#define MAX_OBSTACLES 12

struct Obstacle {
	float x, y;
	float halfSize;
	int   cr, cg, cb;
	int   penalty;
	float life;
	float flashTimer;
	bool  active;
};

extern Obstacle obstacles[MAX_OBSTACLES];
extern int obstacleCount;
extern int obstacleSpawnTimer;
extern float obstacleHitFlash;
extern char  obstacleHitMsg[32];
extern float obstacleHitMsgTimer;
extern float obstacleHitMsgX, obstacleHitMsgY;

void getObstacleColor(int& r, int& g, int& b);
int getObstaclePenalty();
void spawnObstacle();
void initObstacles();
void updateObstacles();
void checkObstacleCollision();

#endif // ENVIRONMENT_H
