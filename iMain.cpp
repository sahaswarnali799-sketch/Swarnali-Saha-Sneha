#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#include "Common.h"
#include "Globals.h"
#include "Entity.h"
#include "Environment.h"
#include "Audio.h"
#include "Renderer.h"
#include "iGraphics.h"

// ══════════════════════════════════════════════════════════
//  iDraw
// ══════════════════════════════════════════════════════════
void iDraw() {
	iClear();
	if (state == SPLASH)     { drawSplash();     return; }
	if (state == MAIN_MENU)  { if (showCreds) drawCredits(); else drawMainMenu(); return; }
	if (state == LEVEL_SELECT) { drawLevelSelect(); return; }

	switch (currentLevel) {
	case FOREST: drawForestBg(); break;
	case DESERT: drawDesertBg(); break;
	case SPACE:  drawSpaceBg();  break;
	}
	drawFood();
	drawObstacles();
	drawAllSnakes();
	drawParticles();
	drawBotNames();
	drawMinimap();
	drawHUD();
	
	if (obstacleHitFlash > 0 && state != GAME_OVER) {
		iSetColor(255, 0, 0);
		int thickness = (int)(obstacleHitFlash * 8);
		iFilledRectangle(0, 0, W, thickness);
		iFilledRectangle(0, H - thickness, W, thickness);
		iFilledRectangle(0, 0, thickness, H);
		iFilledRectangle(W - thickness, 0, thickness, H);
	}
	
	if (state == GAME_OVER) drawGameOver();
}

// ══════════════════════════════════════════════════════════
//  MATH HELPERS USED IN GAMELOOP
// ══════════════════════════════════════════════════════════
float fDist(float x1, float y1, float x2, float y2) {
	float dx = x1 - x2, dy = y1 - y2;
	return sqrtf(dx*dx + dy*dy);
}

float wrapAngle(float a) {
	while (a >  (float)M_PI) a -= 2.0f*(float)M_PI;
	while (a < -(float)M_PI) a += 2.0f*(float)M_PI;
	return a;
}

// startGame implementation -> belongs in main application because it ties everything together
void startGame() {
	score = 0; foodsEaten = 0; botsKilled = 0;
	partCount = 0; levelUpTimer = 0; gameOverTimer = 0;
	mouseActive = false;
	initPlayer();
	initBots();
	initFood();
	initObstacles();
	state = PLAYING;
	setLevelMusic();
}

void checkUnlock() {
	if (currentLevel == FOREST && score >= 100 && !unlocked[2]) {
		unlocked[2] = true;
		strcpy(levelUpText, "DESERT UNLOCKED!");
		strcpy(levelUpSub, "Score 300 in Desert to unlock Space!");
		levelUpTimer = 150.0f;
	}
	if (currentLevel == DESERT && score >= 300 && !unlocked[3]) {
		unlocked[3] = true;
		strcpy(levelUpText, "SPACE UNLOCKED!");
		strcpy(levelUpSub, "The final frontier awaits!");
		levelUpTimer = 150.0f;
	}
}

// ══════════════════════════════════════════════════════════
//  GAME LOOP
// ══════════════════════════════════════════════════════════
void gameLoop() {
	splashTimer++;
	nebulaAngle += 0.003f;
	snakeWiggle += 0.08f;
	updateMusicTimer();

	if (state == SPLASH || state == LEVEL_SELECT || (state == PLAYING && currentLevel == SPACE)) {
		for (int i = 0; i < 80; i++) {
			starY[i] -= starSpd[i];
			if (starY[i] < 0) { starY[i] = H; starX[i] = (float)(rand() % W); }
		}
	}

	if (levelUpTimer > 0) levelUpTimer--;
	if (gameOverTimer > 0) {
		gameOverTimer--;
		updateParticles();
		if (gameOverTimer <= 0) state = GAME_OVER;
		return;
	}

	updateParticles();
	if (state != PLAYING) return;

	updateObstacles();

	if (plrAlive && mouseActive) {
		float dx = (float)(mouseX)-W / 2.0f;
		float dy = (float)(mouseY)-H / 2.0f;
		if (dx*dx + dy*dy > 600.0f) {
			float ma = atan2f(dy, dx);
			float diff = wrapAngle(ma - plrAngle);
			plrAngle += diff * 0.14f;
		}
	}

	if (plrAlive) {
		for (int i = plrLen - 1; i > 0; i--) plrSeg[i] = plrSeg[i - 1];
		float nx = plrSeg[0].x + cosf(plrAngle) * plrSpeed;
		float ny = plrSeg[0].y + sinf(plrAngle) * plrSpeed;

		if (currentLevel == SPACE) {
			if (nx < 0)        nx += WORLD_W;
			if (nx > WORLD_W)  nx -= WORLD_W;
			if (ny < 0)        ny += WORLD_H;
			if (ny > WORLD_H)  ny -= WORLD_H;
		}
		else {
			if (nx < 2 || nx > WORLD_W - 2 || ny < 2 || ny > WORLD_H - 2) {
				killPlayer(); return;
			}
		}

		plrSeg[0] = { nx, ny };
		if (plrGrow > 0) { plrGrow--; if (plrLen < MAX_SEGS - 1) plrLen++; }

		camX = nx - W / 2.0f;
		camY = ny - H / 2.0f;
		if (camX < 0) camX = 0;
		if (camY < 0) camY = 0;
		if (camX > WORLD_W - W) camX = WORLD_W - W;
		if (camY > WORLD_H - H) camY = WORLD_H - H;

		for (int i = foodCount - 1; i >= 0; i--) {
			if (fDist(plrSeg[0].x, plrSeg[0].y, foods[i].x, foods[i].y) < foods[i].r + 10.0f) {
				int pts = foods[i].value * ((currentLevel == FOREST) ? 10 : (currentLevel == DESERT) ? 15 : 20);
				score += pts;
				foodsEaten++;
				plrGrow += foods[i].value * 4;
				spawnParticles(foods[i].x, foods[i].y, foods[i].cr, foods[i].cg, foods[i].cb, 8);
				playCoinSFX();
				foods[i] = foods[--foodCount];
				spawnFood();
				checkUnlock();
			}
		}

		for (int i = 22; i < plrLen; i++) {
			if (fDist(plrSeg[0].x, plrSeg[0].y, plrSeg[i].x, plrSeg[i].y) < 8.0f) {
				killPlayer(); return;
			}
		}

		for (int bi = 0; bi < botCount; bi++) {
			Bot& b = bots[bi];
			if (!b.alive) continue;
			for (int j = 5; j < b.len; j++) {
				if (fDist(plrSeg[0].x, plrSeg[0].y, b.segs[j].x, b.segs[j].y) < 11.0f) {
					killPlayer(); return;
				}
			}
		}

		checkObstacleCollision();
	}

	for (int bi = 0; bi < botCount; bi++) {
		Bot& b = bots[bi];
		if (!b.alive) {
			b.turnTimer++;
			if (b.turnTimer >= 0) spawnBot(bi);
			continue;
		}

		float nearDist = 350.0f;
		float targetAng = b.angle;
		for (int fi = 0; fi < foodCount; fi++) {
			float d = fDist(b.segs[0].x, b.segs[0].y, foods[fi].x, foods[fi].y);
			if (d < nearDist) {
				nearDist = d;
				targetAng = atan2f(foods[fi].y - b.segs[0].y, foods[fi].x - b.segs[0].x);
			}
		}

		float diff = wrapAngle(targetAng - b.angle);
		b.angle += diff * 0.07f;

		float margin = 100;
		if (b.segs[0].x < margin)          b.angle += 0.12f;
		if (b.segs[0].x > WORLD_W - margin) b.angle -= 0.12f;
		if (b.segs[0].y < margin)          b.angle += 0.12f;
		if (b.segs[0].y > WORLD_H - margin) b.angle -= 0.12f;

		b.turnTimer += 1.0f;
		if ((int)b.turnTimer % 70 == 0) b.angle += ((rand() % 200) - 100)*0.008f;

		for (int j = b.len - 1; j > 0; j--) b.segs[j] = b.segs[j - 1];
		float nx = b.segs[0].x + cosf(b.angle) * b.speed;
		float ny = b.segs[0].y + sinf(b.angle) * b.speed;
		nx = fmaxf(5, fminf((float)(WORLD_W - 5), nx));
		ny = fmaxf(5, fminf((float)(WORLD_H - 5), ny));
		b.segs[0] = { nx, ny };

		if (b.grow > 0) { b.grow--; if (b.len < MAX_BOT_SEGS - 1) b.len++; }

		for (int fi = foodCount - 1; fi >= 0; fi--) {
			if (fDist(nx, ny, foods[fi].x, foods[fi].y) < foods[fi].r + 8.0f) {
				b.grow += foods[fi].value * 3;
				foods[fi] = foods[--foodCount];
				spawnFood();
			}
		}

		if (plrAlive) {
			for (int j = 5; j < plrLen; j++) {
				if (fDist(nx, ny, plrSeg[j].x, plrSeg[j].y) < 10.0f) {
					killBot(bi); break;
				}
			}
		}
	}

	while (foodCount < (currentLevel == FOREST ? 40 : currentLevel == DESERT ? 55 : 70))
		spawnFood();
}

// ══════════════════════════════════════════════════════════
//  KEYBOARD
// ══════════════════════════════════════════════════════════
void fixedUpdate() {
	if (isSpecialKeyPressed(GLUT_KEY_END)) { stopMusic(); exit(0); }

	if (state == MAIN_MENU && !showCreds) {
		if (isSpecialKeyPressed(GLUT_KEY_UP))   menuSel = (menuSel + 2) % 3;
		if (isSpecialKeyPressed(GLUT_KEY_DOWN)) menuSel = (menuSel + 1) % 3;
	}
	if (state == LEVEL_SELECT) {
		if (isSpecialKeyPressed(GLUT_KEY_LEFT))  levelSel = (levelSel + 2) % 3;
		if (isSpecialKeyPressed(GLUT_KEY_RIGHT)) levelSel = (levelSel + 1) % 3;
	}

	if (state == PLAYING && plrAlive) {
		bool up = isKeyPressed('w') || isKeyPressed('W') || isSpecialKeyPressed(GLUT_KEY_UP);
		bool down = isKeyPressed('s') || isKeyPressed('S') || isSpecialKeyPressed(GLUT_KEY_DOWN);
		bool left = isKeyPressed('a') || isKeyPressed('A') || isSpecialKeyPressed(GLUT_KEY_LEFT);
		bool right = isKeyPressed('d') || isKeyPressed('D') || isSpecialKeyPressed(GLUT_KEY_RIGHT);

		if (up || down || left || right) {
			mouseActive = false;
			float targetAngle = plrAngle;
			if (up   && right) targetAngle = (float)M_PI / 4.0f;
			else if (up   && left)  targetAngle = 3.0f*(float)M_PI / 4.0f;
			else if (down && right) targetAngle = -(float)M_PI / 4.0f;
			else if (down && left)  targetAngle = -3.0f*(float)M_PI / 4.0f;
			else if (up)            targetAngle = (float)M_PI / 2.0f;
			else if (down)          targetAngle = -(float)M_PI / 2.0f;
			else if (left)          targetAngle = (float)M_PI;
			else if (right)         targetAngle = 0.0f;

			float diff = wrapAngle(targetAngle - plrAngle);
			plrAngle += diff * 0.28f;
		}
	}

	if (isKeyPressed('\r') || isKeyPressed(13)) {
		if (state == SPLASH)      { state = LEVEL_SELECT; return; }
		if (showCreds)            { showCreds = false;  return; }
		if (state == MAIN_MENU) {
			if (menuSel == 0) { state = LEVEL_SELECT; return; }
			if (menuSel == 1) { showCreds = true;     return; }
			if (menuSel == 2) { stopMusic(); exit(0); }
		}
		if (state == LEVEL_SELECT) {
			if (!unlocked[levelSel + 1]) return;
			currentLevel = (Level)(levelSel + 1);
			startGame();
			return;
		}
	}

	if (isKeyPressed(27)) {
		if (showCreds)            { showCreds = false; return; }
		if (state == LEVEL_SELECT){ state = LEVEL_SELECT; return; }
		if (state == PLAYING || state == PAUSED || state == GAME_OVER) {
			stopMusic(); state = LEVEL_SELECT; playBgMusic(); return;
		}
	}

	if (isKeyPressed('p') || isKeyPressed('P')) {
		if (state == PLAYING) { state = PAUSED;  return; }
		if (state == PAUSED)  { state = PLAYING; return; }
	}

	if (state == GAME_OVER) {
		if (isKeyPressed('r') || isKeyPressed('R')) { startGame(); return; }
		if (isKeyPressed('l') || isKeyPressed('L')) { stopMusic(); state = LEVEL_SELECT; playBgMusic(); return; }
	}
}

void iKeyboard(unsigned char key) {}
void iSpecialKeyboard(unsigned char key) {}

// ══════════════════════════════════════════════════════════
//  MOUSE
// ══════════════════════════════════════════════════════════
void iMouseMove(int mx, int my) {
	mouseX = mx; mouseY = my;
	if (state == PLAYING) mouseActive = true;
}
void iPassiveMouseMove(int mx, int my) {
	mouseX = mx; mouseY = my;
	if (state == PLAYING) mouseActive = true;
}
void iMouse(int btn, int st, int mx, int my) {
	if (btn != GLUT_LEFT_BUTTON || st != GLUT_DOWN) return;

	if (state == SPLASH)                { state = LEVEL_SELECT; return; }

	if (state == MAIN_MENU && !showCreds) {
		int bY[3] = { H / 2 + 60, H / 2, H / 2 - 60 };
		for (int i = 0; i < 3; i++) {
			if (mx >= W / 2 - 130 && mx <= W / 2 + 130 &&
				my >= bY[i] - 14 && my <= bY[i] + 26) {
				menuSel = i;
				if (i == 0) { state = LEVEL_SELECT; return; }
				if (i == 1) { showCreds = true;     return; }
				if (i == 2) { stopMusic(); exit(0); }
			}
		}
		return;
	}

	if (showCreds) { showCreds = false; return; }

	if (state == LEVEL_SELECT) {
		int cx[3] = { 160, W / 2, 840 };
		int cy2[3] = { H / 2 + 10, H / 2 + 10, H / 2 + 10 };
		for (int i = 0; i < 3; i++) {
			int dx = mx - cx[i], dy = my - cy2[i];
			if (dx*dx + dy*dy <= 72 * 72) {
				levelSel = i;
				if (!unlocked[i + 1]) return;
				currentLevel = (Level)(i + 1);
				startGame();
				return;
			}
		}
		return;
	}

	if (state == GAME_OVER) {
		if (mx >= W / 2 - 148 && mx <= W / 2 + 148 &&
			my >= H / 2 - 100 && my <= H / 2 - 75) {
			startGame(); return;
		}
	}
}

// ══════════════════════════════════════════════════════════
//  MAIN
// ══════════════════════════════════════════════════════════
int main() {
	srand((unsigned int)time(NULL));
	iSetTimer(16, gameLoop);
	iInitialize(W, H, project);
	playBgMusic();
	printf("Snake Master Started!\n");
	printf("  SPLASH  : Press ENTER or Click to go to Menu\n");
	printf("  MENU    : UP/DOWN arrows to navigate, ENTER to select\n");
	printf("  LEVELS  : LEFT/RIGHT arrows to choose, ENTER to play\n");
	printf("  PLAYING : WASD or Arrow Keys to move, P=Pause, ESC=Menu\n");
	iStart();
	return 0;
}