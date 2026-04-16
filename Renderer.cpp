#include "Renderer.h"
#include "Globals.h"
#include "Environment.h"
#include "Entity.h"
#include "Audio.h"
#include "glut.h"
#include <cstdio>
#include <cstring>
#include <cmath>

extern void iClear();
extern void iSetColor(double r, double g, double b);
extern void iLine(double x1, double y1, double x2, double y2);
extern void iFilledPolygon(double x[], double y[], int n);
extern void iRectangle(double left, double bottom, double dx, double dy);
extern void iFilledRectangle(double left, double bottom, double dx, double dy);
extern void iFilledCircle(double x, double y, double r, int slices=100);
extern void iCircle(double x, double y, double r, int slices=100);
extern void iText(double x, double y, char *str, void* font);
extern unsigned int iLoadImage(char filename[]);
extern void iShowImage(int x, int y, int width, int height, unsigned int texture);

void wFilledCircle(float wx_, float wy_, float r) {
	int sx = (int)(wx_ - camX);
	int sy = (int)(wy_ - camY);
	if (sx + r < 0 || sx - r > W || sy + r < 0 || sy - r > H) return;
	iFilledCircle(sx, sy, (int)r);
}
void wFilledRect(float wx_, float wy_, float ww, float wh) {
	int sx = (int)(wx_ - camX);
	int sy = (int)(wy_ - camY);
	if (sx > W || sy > H || sx + ww < 0 || sy + wh < 0) return;
	iFilledRectangle(sx, sy, (int)ww, (int)wh);
}
void wLine(float x1, float y1, float x2, float y2) {
	iLine((int)(x1 - camX), (int)(y1 - camY), (int)(x2 - camX), (int)(y2 - camY));
}

void drawForestBg() {
	for (int y = 0; y < H; y++) {
		int g = 15 + (int)(y * 25.0f / H);
		iSetColor(4, g, 8);
		iLine(0, y, W, y);
	}
	int treeWX[] = { 200, 400, 650, 900, 1150, 1400, 1700, 300, 550, 800, 1100, 1350, 1600, 1850 };
	int treeWY[] = { 150, 300, 200, 400, 150, 350, 280, 500, 600, 550, 700, 450, 600, 180 };
	for (int t = 0; t < 14; t++) {
		int sx = (int)(treeWX[t] - camX);
		int sy = (int)(treeWY[t] - camY);
		if (sx < -60 || sx > W + 60 || sy < -80 || sy > H + 80) continue;
		iSetColor(8, 60 + t * 4 % 40, 12);
		double px[3] = { (double)sx, (double)(sx - 28), (double)(sx + 28) };
		double py[3] = { (double)(sy + 70), (double)sy, (double)sy };
		iFilledPolygon(px, py, 3);
		iSetColor(45, 28, 8);
		iFilledRectangle(sx - 5, sy - 10, 10, 25);
	}
	iSetColor(0, 55, 15);
	for (int gx = (int)camX % 80; gx < W; gx += 80) iLine(gx, 0, gx, H);
	for (int gy = (int)camY % 80; gy < H; gy += 80) iLine(0, gy, W, gy);
	iSetColor(0, 120, 40);
	int bx1 = (int)(0 - camX), by1 = (int)(0 - camY);
	int bx2 = (int)(WORLD_W - camX), by2 = (int)(WORLD_H - camY);
	iRectangle(bx1, by1, bx2 - bx1, by2 - by1);
}

void drawDesertBg() {
	for (int y = H / 2; y < H; y++) {
		int r = 160 + (int)((y - H / 2)*80.0f / (H / 2));
		int g = 90 + (int)((y - H / 2)*30.0f / (H / 2));
		iSetColor(r, g, 15);
		iLine(0, y, W, y);
	}
	iSetColor(190, 150, 50);
	iFilledRectangle(0, 0, W, H / 2);
	int cwx[] = { 300, 600, 900, 1200, 1500, 1800, 250, 700, 1100, 1600 };
	int cwy[] = { 200, 400, 150, 350, 250, 400, 500, 550, 600, 200 };
	for (int c = 0; c < 10; c++) {
		int sx = (int)(cwx[c] - camX);
		int sy = (int)(cwy[c] - camY);
		if (sx < -40 || sx > W + 40) continue;
		iSetColor(35, 110, 25);
		iFilledRectangle(sx - 6, sy, 12, 65);
		iFilledRectangle(sx - 22, sy + 28, 18, 8);
		iFilledRectangle(sx + 6, sy + 42, 16, 8);
	}
	iSetColor(255, 215, 45); iFilledCircle(820, 520, 32);
	iSetColor(255, 240, 100); iFilledCircle(820, 520, 20);
	iSetColor(160, 100, 15);
	for (int gx = (int)camX % 80; gx < W; gx += 80) iLine(gx, 0, gx, H);
	for (int gy = (int)camY % 80; gy < H; gy += 80) iLine(0, gy, W, gy);
	iSetColor(200, 140, 20);
	int bx1 = (int)(0 - camX), by1 = (int)(0 - camY);
	int bx2 = (int)(WORLD_W - camX), by2 = (int)(WORLD_H - camY);
	iRectangle(bx1, by1, bx2 - bx1, by2 - by1);
}

void drawSpaceBg() {
	iSetColor(4, 4, 18);
	iFilledRectangle(0, 0, W, H);
	for (int i = 0; i < 200; i++) {
		int sx = (int)(((i * 677) % WORLD_W) - camX);
		int sy = (int)(((i * 443) % WORLD_H) - camY);
		if (sx < 0 || sx >= W || sy < 0 || sy >= H) continue;
		int br = 80 + (i * 97) % 156;
		iSetColor(br, br, br + 20);
		iFilledCircle(sx, sy, (i % 7 == 0) ? 2 : 1);
	}
	for (int i = 0; i < 80; i++) {
		iSetColor(180 + i, 180 + i, 200 + i);
		iFilledCircle((int)starX[i], (int)starY[i], 1);
	}
	int plx = (int)(1200 - camX), ply = (int)(800 - camY);
	if (plx > -80 && plx < W + 80 && ply > -80 && ply < H + 80) {
		iSetColor(70, 35, 110);  iFilledCircle(plx, ply, 50);
		iSetColor(90, 55, 140);  iFilledCircle(plx, ply, 36);
	}
	iSetColor(0, 30, 65);
	for (int gx = (int)camX % 80; gx < W; gx += 80) iLine(gx, 0, gx, H);
	for (int gy = (int)camY % 80; gy < H; gy += 80) iLine(0, gy, W, gy);
	iSetColor(0, 140, 190);
	int bx1 = (int)(0 - camX), by1 = (int)(0 - camY);
	int bx2 = (int)(WORLD_W - camX), by2 = (int)(WORLD_H - camY);
	iRectangle(bx1, by1, bx2 - bx1, by2 - by1);
	iSetColor(0, 80, 120);
	iRectangle(bx1 + 2, by1 + 2, bx2 - bx1 - 4, by2 - by1 - 4);
}

void drawFood() {
	for (int i = 0; i < foodCount; i++) {
		Food& f = foods[i];
		int sx = (int)(f.x - camX);
		int sy = (int)(f.y - camY);
		if (sx < -15 || sx > W + 15 || sy < -15 || sy > H + 15) continue;
		float pr = f.r + sinf(f.pulse) * 1.8f;
		iSetColor(f.cr / 3, f.cg / 3, f.cb / 3);
		iFilledCircle(sx, sy, (int)(pr*2.8f));
		iSetColor(f.cr, f.cg, f.cb);
		iFilledCircle(sx, sy, (int)pr);
		iSetColor(220, 240, 255);
		iFilledCircle(sx - (int)(pr*0.3f), sy + (int)(pr*0.3f), (int)(pr*0.3f));
	}
}

void drawObstacles() {
	for (int i = 0; i < obstacleCount; i++) {
		Obstacle& o = obstacles[i];
		if (!o.active) continue;
		int sx = (int)(o.x - camX);
		int sy = (int)(o.y - camY);
		int hs = (int)o.halfSize;
		if (sx + hs < 0 || sx - hs > W || sy + hs < 0 || sy - hs > H) continue;

		float brightness = 1.0f;
		if (o.flashTimer < 20.0f) {
			brightness = ((int)o.flashTimer % 4 < 2) ? 1.0f : 0.3f;
		}
		if (o.life < 60.0f) {
			if ((int)o.life % 8 < 4) brightness = 0.2f;
		}

		iSetColor((int)(o.cr * 0.25f * brightness), (int)(o.cg * 0.25f * brightness), (int)(o.cb * 0.25f * brightness));
		iFilledRectangle(sx - hs - 3, sy - hs - 3, hs * 2 + 6, hs * 2 + 6);

		iSetColor((int)(o.cr * brightness), (int)(o.cg * brightness), (int)(o.cb * brightness));
		iFilledRectangle(sx - hs, sy - hs, hs * 2, hs * 2);

		iSetColor((int)(fminf(255, o.cr * 1.3f) * brightness), (int)(fminf(255, o.cg * 1.3f) * brightness), (int)(fminf(255, o.cb * 1.3f) * brightness));
		iFilledRectangle(sx - hs + 4, sy - hs + 4, hs * 2 - 8, hs * 2 - 8);

		iSetColor(255, 255, 255);
		iLine(sx - 4, sy - 4, sx + 4, sy + 4);
		iLine(sx - 4, sy + 4, sx + 4, sy - 4);

		iSetColor((int)(o.cr * brightness), (int)(o.cg * brightness), (int)(o.cb * brightness));
		iRectangle(sx - hs, sy - hs, hs * 2, hs * 2);
	}

	if (obstacleHitMsgTimer > 0) {
		int mx = (int)(obstacleHitMsgX - camX);
		int my = (int)(obstacleHitMsgY - camY + (30.0f - obstacleHitMsgTimer) * 1.5f);
		iSetColor(255, 60, 60);
		iText(mx - 15, my, obstacleHitMsg, GLUT_BITMAP_HELVETICA_18);
	}
}

void drawOneSnake(Seg* segs, int len, int headR, int headG, int headB,
	int bodyR, int bodyG, int bodyB, bool isPlayer) {
	for (int i = len - 1; i >= 1; i--) {
		int sx1 = (int)(segs[i].x - camX);
		int sy1 = (int)(segs[i].y - camY);
		int sx2 = (int)(segs[i - 1].x - camX);
		int sy2 = (int)(segs[i - 1].y - camY);
		if (sx1 < -20 && sx2 < -20) continue;
		if (sx1 > W + 20 && sx2 > W + 20) continue;
		if (sy1 < -20 && sy2 < -20) continue;
		if (sy1 > H + 20 && sy2 > H + 20) continue;
		float t = (float)i / len;
		int r = (int)(bodyR * (0.5f + 0.5f*(1 - t)));
		int g = (int)(bodyG * (0.5f + 0.5f*(1 - t)));
		int b = (int)(bodyB * (0.5f + 0.5f*(1 - t)));
		int radius = (int)((isPlayer ? 8.0f : 7.0f) * (1.0f - t*0.35f));
		if (radius < 2) radius = 2;
		float dx = (float)(sx2 - sx1), dy = (float)(sy2 - sy1);
		float d = sqrtf(dx*dx + dy*dy);
		if (d < 1) continue;
		int steps = (int)d / 2 + 1;
		for (int s = 0; s <= steps; s++) {
			float frac = (float)s / steps;
			int px2 = sx1 + (int)(dx*frac);
			int py2 = sy1 + (int)(dy*frac);
			if (px2 < -radius || px2 > W + radius || py2 < -radius || py2 > H + radius) continue;
			iSetColor(r, g, b);
			iFilledCircle(px2, py2, radius);
		}
	}
	int hsx = (int)(segs[0].x - camX);
	int hsy = (int)(segs[0].y - camY);
	if (hsx < -20 || hsx > W + 20 || hsy < -20 || hsy > H + 20) return;
	int hr = isPlayer ? 9 : 8;
	iSetColor(headR / 4, headG / 4, headB / 4);
	iFilledCircle(hsx, hsy, hr + 6);
	iSetColor(headR / 2, headG / 2, headB / 2);
	iFilledCircle(hsx, hsy, hr + 3);
	iSetColor(headR, headG, headB);
	iFilledCircle(hsx, hsy, hr);
	float eyeAngle = (len > 1) ?
		atan2f(segs[0].y - segs[1].y, segs[0].x - segs[1].x) :
		(isPlayer ? plrAngle : 0.0f);
	int ex1x = hsx + (int)(cosf(eyeAngle + 0.6f) * hr * 0.65f);
	int ex1y = hsy + (int)(sinf(eyeAngle + 0.6f) * hr * 0.65f);
	int ex2x = hsx + (int)(cosf(eyeAngle - 0.6f) * hr * 0.65f);
	int ex2y = hsy + (int)(sinf(eyeAngle - 0.6f) * hr * 0.65f);
	iSetColor(255, 255, 255);
	iFilledCircle(ex1x, ex1y, 2); iFilledCircle(ex2x, ex2y, 2);
	iSetColor(0, 0, 0);
	iFilledCircle(ex1x + 1, ex1y + 1, 1); iFilledCircle(ex2x + 1, ex2y + 1, 1);
}

void drawAllSnakes() {
	for (int i = 0; i < botCount; i++) {
		Bot& b = bots[i];
		if (!b.alive) continue;
		drawOneSnake(b.segs, b.len, b.colorR, b.colorG, b.colorB,
			b.colorR * 2 / 3, b.colorG * 2 / 3, b.colorB * 2 / 3, false);
	}
	if (plrAlive) {
		int hr, hg, hb, br, bg, bb;
		switch (currentLevel) {
		case FOREST: hr = 30;  hg = 220; hb = 60;  br = 10;  bg = 140; bb = 40;  break;
		case DESERT: hr = 255; hg = 180; hb = 0;   br = 200; bg = 120; bb = 0;   break;
		case SPACE:  hr = 0;   hg = 200; hb = 255; br = 0;   bg = 100; bb = 180; break;
		default:     hr = 30;  hg = 220; hb = 60;  br = 10;  bg = 140; bb = 40;  break;
		}
		drawOneSnake(plrSeg, plrLen, hr, hg, hb, br, bg, bb, true);
	}
}

void drawBotNames() {
	for (int i = 0; i < botCount; i++) {
		Bot& b = bots[i];
		if (!b.alive) continue;
		int sx = (int)(b.segs[0].x - camX);
		int sy = (int)(b.segs[0].y - camY);
		if (sx < -20 || sx > W + 20 || sy < -20 || sy > H + 20) continue;
		iSetColor(b.colorR, b.colorG, b.colorB);
		iText(sx - 20, sy + 20, b.name, GLUT_BITMAP_HELVETICA_12);
	}
}

void drawParticles() {
	for (int i = 0; i < partCount; i++) {
		Particle& p = parts[i];
		int sx = (int)(p.x - camX);
		int sy = (int)(p.y - camY);
		if (sx < 0 || sx > W || sy < 0 || sy > H) continue;
		int alpha = (int)(p.life * 3);
		if (alpha > 3) alpha = 3;
		iSetColor(p.r, p.g, p.b);
		iFilledCircle(sx, sy, alpha + 1);
	}
}

void drawHUD() {
	iSetColor(0, 0, 0);
	iFilledRectangle(0, H - 38, W, 38);
	iSetColor(20, 30, 40);
	iFilledRectangle(0, H - 36, W, 34);
	const char* lnames[] = { "", "FOREST", "DESERT", "SPACE" };
	char buf[80];
	switch (currentLevel) {
	case FOREST: iSetColor(0, 200, 100); break;
	case DESERT: iSetColor(255, 140, 0); break;
	case SPACE:  iSetColor(180, 80, 255); break;
	}
	sprintf(buf, "[%s]", lnames[currentLevel]);
	iText(10, H - 24, buf, GLUT_BITMAP_HELVETICA_18);
	iSetColor(0, 220, 120);
	sprintf(buf, "SCORE: %d", score);
	iText(130, H - 24, buf, GLUT_BITMAP_HELVETICA_18);
	iSetColor(255, 170, 0);
	sprintf(buf, "BEST: %d", bestScore[currentLevel] > score ? bestScore[currentLevel] : score);
	iText(310, H - 24, buf, GLUT_BITMAP_HELVETICA_18);
	iSetColor(120, 120, 140);
	int goal = (currentLevel == FOREST) ? 100 : (currentLevel == DESERT) ? 300 : 0;
	if (goal > 0) sprintf(buf, "GOAL: %d -> NEXT LEVEL", goal);
	else          sprintf(buf, "FINAL LEVEL");
	iText(480, H - 24, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(100, 160, 200);
	sprintf(buf, "LEN:%d  BOTS KILLED:%d", plrLen, botsKilled);
	iText(730, H - 24, buf, GLUT_BITMAP_HELVETICA_12);
	iSetColor(50, 60, 70);
	iText(10, 8, "WASD/ARROWS=Move  P=Pause  ESC=Menu", GLUT_BITMAP_HELVETICA_12);

	if (levelUpTimer > 0) {
		iSetColor(0, 0, 0);
		iFilledRectangle(W / 2 - 260, H / 2 - 50, 520, 90);
		switch (currentLevel) {
		case FOREST: iSetColor(0, 200, 100); break;
		case DESERT: iSetColor(255, 140, 0); break;
		case SPACE:  iSetColor(170, 60, 255); break;
		}
		iRectangle(W / 2 - 260, H / 2 - 50, 520, 90);
		iText(W / 2 - 180, H / 2 + 8, levelUpText, GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(180, 180, 180);
		iText(W / 2 - 200, H / 2 - 20, levelUpSub, GLUT_BITMAP_HELVETICA_12);
	}
	if (state == PAUSED) {
		iSetColor(0, 0, 0);
		iFilledRectangle(W / 2 - 120, H / 2 - 38, 240, 76);
		iSetColor(0, 200, 120);
		iRectangle(W / 2 - 120, H / 2 - 38, 240, 76);
		iText(W / 2 - 48, H / 2 - 5, "PAUSED", GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(160, 160, 160);
		iText(W / 2 - 80, H / 2 - 28, "Press P to Continue", GLUT_BITMAP_HELVETICA_12);
	}
}

void drawMinimap() {
	int mx = W - 135, my = 25;
	int mw = 120, mh = 80;
	float sx = (float)mw / WORLD_W;
	float sy = (float)mh / WORLD_H;
	iSetColor(0, 8, 18);
	iFilledRectangle(mx, my, mw, mh);
	for (int i = 0; i < foodCount; i++) {
		int fx = mx + (int)(foods[i].x * sx);
		int fy = my + (int)(foods[i].y * sy);
		iSetColor(foods[i].cr / 2, foods[i].cg / 2, foods[i].cb / 2);
		iFilledRectangle(fx, fy, 2, 2);
	}
	for (int i = 0; i < botCount; i++) {
		if (!bots[i].alive) continue;
		int bx = mx + (int)(bots[i].segs[0].x * sx);
		int by = my + (int)(bots[i].segs[0].y * sy);
		iSetColor(bots[i].colorR, bots[i].colorG, bots[i].colorB);
		iFilledRectangle(bx - 1, by - 1, 4, 4);
	}
	if (plrAlive && plrLen > 0) {
		int px2 = mx + (int)(plrSeg[0].x * sx);
		int py2 = my + (int)(plrSeg[0].y * sy);
		switch (currentLevel) {
		case FOREST: iSetColor(0, 255, 120); break;
		case DESERT: iSetColor(255, 180, 0); break;
		case SPACE:  iSetColor(0, 220, 255); break;
		}
		iFilledCircle(px2, py2, 3);
		iSetColor(60, 80, 100);
		int vx = mx + (int)(camX * sx);
		int vy = my + (int)(camY * sy);
		int vw = (int)(W * sx);
		int vh = (int)(H * sy);
		iRectangle(vx, vy, vw, vh);
	}
	switch (currentLevel) {
	case FOREST: iSetColor(0, 140, 60); break;
	case DESERT: iSetColor(180, 100, 10); break;
	case SPACE:  iSetColor(80, 50, 180); break;
	}
	iRectangle(mx, my, mw, mh);
}

void initStars() {
	if (starsInit) return;
	for (int i = 0; i < 150; i++) {
		starX[i] = (float)(rand() % W);
		starY[i] = (float)(rand() % H);
		starSpd[i] = 0.2f + (rand() % 10) * 0.06f;
	}
	starsInit = true;
}

void drawSplash() {
	iSetColor(4, 4, 18);
	iFilledRectangle(0, 0, W, H);
	initStars();
	for (int i = 0; i < 120; i++) {
		int b2 = 80 + i * 2;
		iSetColor(b2, b2, b2 + 20);
		iFilledCircle((int)starX[i], (int)starY[i], 1);
	}
	iSetColor(0, 30, 15);
	iFilledCircle(W / 2, H / 2 + 40, 200);
	iSetColor(0, 20, 10);
	iFilledCircle(W / 2, H / 2 + 40, 160);
	iSetColor(0, 200, 100);
	iText(W / 2 - 165, H / 2 + 100, "SNAKE  MASTER", GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(0, 160, 80);
	iText(W / 2 - 148, H / 2 + 20, "3 Levels  |  Bots  |  Smooth Movement", GLUT_BITMAP_HELVETICA_12);
	if ((splashTimer / 18) % 2 == 0) {
		iSetColor(255, 215, 45);
		iText(W / 2 - 150, H / 2 - 30, "Press ENTER or Click to Start", GLUT_BITMAP_HELVETICA_18);
	}
	iSetColor(50, 60, 70);
	iText(W / 2 - 160, H / 2 - 80, "WASD / Arrow Keys = Move   P = Pause   ESC = Menu", GLUT_BITMAP_HELVETICA_12);
}

void drawMainMenu() {
	iSetColor(5, 4, 20);
	iFilledRectangle(0, 0, W, H);
	iSetColor(0, 35, 80);
	for (int i = 0; i < 14; i++) {
		iLine(i * 72, 0, i * 72, H);
		iLine(0, i * 50, W, i * 50);
	}
	iSetColor(0, 195, 115);
	iText(W / 2 - 160, H - 100, "SNAKE  MASTER", GLUT_BITMAP_TIMES_ROMAN_24);

	const char* labels[] = { "PLAY GAME", "CREDITS", "EXIT" };
	int bY[] = { H / 2 + 60, H / 2, H / 2 - 60 };
	for (int i = 0; i < 3; i++) {
		bool sel = (menuSel == i);
		if (sel) {
			iSetColor(0, 50, 90);
			iFilledRectangle(W / 2 - 130, bY[i] - 14, 260, 40);
		}
		iSetColor(sel ? 0 : 0, sel ? 170 : 55, sel ? 250 : 115);
		iRectangle(W / 2 - 130, bY[i] - 14, 260, 40);
		iSetColor(sel ? 255 : 170, sel ? 255 : 195, sel ? 255 : 195);
		int off = (int)(strlen(labels[i]) * 4);
		iText(W / 2 - off, bY[i] + 6, (char*)labels[i], GLUT_BITMAP_HELVETICA_18);
		if (sel) {
			iSetColor(0, 200, 120);
			iText(W / 2 - 150, bY[i] + 6, ">", GLUT_BITMAP_HELVETICA_18);
		}
	}
	iSetColor(55, 55, 60);
	iText(W / 2 - 175, 20, "UP/DOWN = Navigate  |  ENTER = Select  |  ESC = Back", GLUT_BITMAP_HELVETICA_12);
}

void drawCredits() {
	iSetColor(4, 4, 18);
	iFilledRectangle(0, 0, W, H);
	iSetColor(0, 195, 115);
	iText(W / 2 - 80, H - 100, "CREDITS", GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(200, 200, 200);
	iText(W / 2 - 200, H / 2 + 80, "Game Design   : Snake MASTER", GLUT_BITMAP_HELVETICA_18);
	iText(W / 2 - 200, H / 2 + 45, "Programming   : iGraphics + C++", GLUT_BITMAP_HELVETICA_18);
	iText(W / 2 - 200, H / 2 + 10, "Levels        : Forest  |  Desert  |  Space", GLUT_BITMAP_HELVETICA_18);
	iText(W / 2 - 200, H / 2 - 25, "Bots          : 4 / 6 / 8 per level", GLUT_BITMAP_HELVETICA_18);
	iText(W / 2 - 200, H / 2 - 60, "Music         : Level-specific BGM", GLUT_BITMAP_HELVETICA_18);
	iText(W / 2 - 200, H / 2 - 95, "Gameplay      : Wormate.io style smooth snake", GLUT_BITMAP_HELVETICA_18);
	iSetColor(255, 205, 45);
	iText(W / 2 - 140, H / 2 - 145, "Press ESC or ENTER to go back", GLUT_BITMAP_HELVETICA_18);
}

void drawGlowCircle(int cx, int cy, int rad, int r, int g, int b, bool selected) {
	for (int layer = 4; layer >= 1; layer--) {
		int gr = r / (layer + 1);
		int gg = g / (layer + 1);
		int gb = b / (layer + 1);
		iSetColor(gr, gg, gb);
		iFilledCircle(cx, cy, rad + layer * (selected ? 9 : 6));
	}
	iSetColor(6, 2, 18);
	iFilledCircle(cx, cy, rad - 4);
	iSetColor(r, g, b);
	iCircle(cx, cy, rad);
	iCircle(cx, cy, rad - 1);
	iCircle(cx, cy, rad - 2);
	if (selected) {
		iSetColor(r, g, b);
		iCircle(cx, cy, rad + 2);
	}
}

void drawLevelSelect() {
	initStars();

	if (!levelSelectTexLoaded) {
		levelSelectTex = iLoadImage("level_select_bg.jpg");
		levelSelectTexLoaded = true;
	}

	if (levelSelectTex != 0) {
		iShowImage(0, 0, W, H, levelSelectTex);
	} else {
		iSetColor(2, 1, 12);
		iFilledRectangle(0, 0, W, H);
	}

	for (int i = 0; i < 80; i++) {
		int br = 180 + (i * 113) % 75;
		iSetColor(br, br, br + 30);
		iFilledCircle((int)starX[i], (int)starY[i], (i % 9 == 0) ? 2 : 1);
	}

	int ncx = W / 2, ncy = H / 2 + 30;

	for (int ring = 5; ring >= 1; ring--) {
		int alpha = ring * 8;
		iSetColor(alpha * 2, alpha / 2, alpha * 4);
		iFilledCircle(ncx, ncy, 230 + ring * 8);
	}
	for (int ring = 4; ring >= 1; ring--) {
		int alpha = ring * 12;
		iSetColor(alpha * 3, alpha, 0);
		iFilledCircle(ncx, ncy, 130 + ring * 10);
	}
	iSetColor(4, 2, 14);
	iFilledCircle(ncx, ncy, 120);

	int plx = W / 2 + 210, ply = H - 160;
	iSetColor(60, 35, 10);
	iFilledCircle(plx, ply, 26);
	iSetColor(200, 130, 40);
	iFilledCircle(plx, ply, 20);
	iSetColor(220, 160, 60);
	iFilledCircle(plx, ply, 15);
	iSetColor(180, 110, 30);
	for (int rx = -34; rx <= 34; rx++) {
		int rabs = rx < 0 ? -rx : rx;
		int ry = (int)(4.0f * sinf((float)rx / 34.0f * (float)M_PI));
		if (rabs > 18 || (rabs < 17 && rabs > 3)) {
			iFilledCircle(plx + rx, ply + ry, 2);
		}
	}

	{
		int snakeSegs = 14;
		for (int seg = snakeSegs; seg >= 1; seg--) {
			float t = (float)seg / snakeSegs;
			int sx = (int)(ncx - 80 + seg * 52 - snakeSegs * 26);
			int sy = (int)(ncy - 155 + sinf(snakeWiggle + seg * 0.5f) * 12.0f);
			int segR = (int)(68 + 50 * t);
			int segG = (int)(200 + 40 * (1.0f - t));
			int segB = (int)(180 * (1.0f - t * 0.5f));
			iSetColor(segR / 3, segG / 3, segB / 3);
			iFilledCircle(sx, sy, 9);
			iSetColor(segR, segG, segB);
			iFilledCircle(sx, sy, 7);
		}
		int hx = (int)(ncx - 80 + 1 * 52 - snakeSegs * 26);
		int hy = (int)(ncy - 155 + sinf(snakeWiggle + 0.5f) * 12.0f);
		iSetColor(20, 80, 60);
		iFilledCircle(hx, hy, 12);
		iSetColor(60, 220, 160);
		iFilledCircle(hx, hy, 10);
		iSetColor(255, 80, 120);
		iFilledCircle(hx - 3, hy + 4, 3);
		iFilledCircle(hx + 3, hy + 4, 3);
		iSetColor(0, 0, 0);
		iFilledCircle(hx - 2, hy + 4, 1);
		iFilledCircle(hx + 4, hy + 4, 1);
		iSetColor(220, 40, 80);
		iLine(hx - 12, hy, hx - 18, hy + 3);
		iLine(hx - 12, hy, hx - 18, hy - 3);
	}

	iSetColor(80, 50, 0);
	iText(W / 2 - 148, H - 76, "SELECT LEVEL", GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(80, 50, 0);
	iText(W / 2 - 146, H - 74, "SELECT LEVEL", GLUT_BITMAP_TIMES_ROMAN_24);
	iSetColor(255, 220, 20);
	iText(W / 2 - 147, H - 75, "SELECT LEVEL", GLUT_BITMAP_TIMES_ROMAN_24);

	struct LvlInfo {
		const char* topLabel;
		const char* name;
		int cx, cy;
		int r, g, b;
	};
	LvlInfo lvls[3] = {
		{ "LEVEL 1:", "FOREST", 185,    H / 2 + 15, 0,   210, 80  },
		{ "LEVEL 2:", "DESERT", W / 2,  H / 2 + 15, 220, 140, 0   },
		{ "LEVEL 3:", "SPACE",  815,    H / 2 + 15, 150, 60,  255  },
	};

	for (int i = 0; i < 3; i++) {
		bool sel = (levelSel == i);
		int rad = sel ? 80 : 68;

		drawGlowCircle(lvls[i].cx, lvls[i].cy, rad, lvls[i].r, lvls[i].g, lvls[i].b, sel);

		if (i == 0) {
			iSetColor(lvls[i].r, lvls[i].g, lvls[i].b);
			double tx[3] = { (double)lvls[i].cx, (double)(lvls[i].cx - 22), (double)(lvls[i].cx + 22) };
			double ty[3] = { (double)(lvls[i].cy + 30), (double)(lvls[i].cy - 4), (double)(lvls[i].cy - 4) };
			iFilledPolygon(tx, ty, 3);
			double tx2[3] = { (double)lvls[i].cx, (double)(lvls[i].cx - 17), (double)(lvls[i].cx + 17) };
			double ty2[3] = { (double)(lvls[i].cy + 44), (double)(lvls[i].cy + 14), (double)(lvls[i].cy + 14) };
			iFilledPolygon(tx2, ty2, 3);
			iSetColor(120, 70, 20);
			iFilledRectangle(lvls[i].cx - 4, lvls[i].cy - 22, 8, 24);
		}
		else if (i == 1) {
			iSetColor(lvls[i].r, lvls[i].g, lvls[i].b);
			iFilledRectangle(lvls[i].cx - 5, lvls[i].cy - 24, 10, 52);
			iFilledRectangle(lvls[i].cx - 22, lvls[i].cy + 2, 17, 8);
			iFilledRectangle(lvls[i].cx + 5,  lvls[i].cy + 14, 17, 8);
			iFilledRectangle(lvls[i].cx - 28, lvls[i].cy - 12, 8, 22);
			iFilledRectangle(lvls[i].cx + 20, lvls[i].cy,      8, 22);
		}
		else {
			iSetColor(lvls[i].r, lvls[i].g, lvls[i].b);
			double rx[4] = { (double)(lvls[i].cx - 10), (double)(lvls[i].cx + 10), (double)(lvls[i].cx + 10), (double)(lvls[i].cx - 10) };
			double ry[4] = { (double)(lvls[i].cy - 8),  (double)(lvls[i].cy - 8),  (double)(lvls[i].cy + 26), (double)(lvls[i].cy + 26) };
			iFilledPolygon(rx, ry, 4);
			double nx2[3] = { (double)(lvls[i].cx), (double)(lvls[i].cx - 10), (double)(lvls[i].cx + 10) };
			double ny2[3] = { (double)(lvls[i].cy - 30), (double)(lvls[i].cy - 8), (double)(lvls[i].cy - 8) };
			iFilledPolygon(nx2, ny2, 3);
			iSetColor(200, 100, 255);
			double fx1[3] = { (double)(lvls[i].cx - 10), (double)(lvls[i].cx - 22), (double)(lvls[i].cx - 10) };
			double fy1[3] = { (double)(lvls[i].cy + 10),  (double)(lvls[i].cy + 26), (double)(lvls[i].cy + 26) };
			iFilledPolygon(fx1, fy1, 3);
			double fx2[3] = { (double)(lvls[i].cx + 10), (double)(lvls[i].cx + 22), (double)(lvls[i].cx + 10) };
			double fy2[3] = { (double)(lvls[i].cy + 10),  (double)(lvls[i].cy + 26), (double)(lvls[i].cy + 26) };
			iFilledPolygon(fx2, fy2, 3);
			iSetColor(180, 220, 255);
			iFilledCircle(lvls[i].cx, lvls[i].cy + 2, 6);
		}

		iSetColor(sel ? 255 : 210, sel ? 255 : 210, sel ? 200 : 160);
		int lw = (int)(strlen(lvls[i].topLabel) * 4);
		iText(lvls[i].cx - lw, lvls[i].cy - 42, (char*)lvls[i].topLabel, GLUT_BITMAP_HELVETICA_12);
		int nw = (int)(strlen(lvls[i].name) * 5);
		iText(lvls[i].cx - nw, lvls[i].cy + 46, (char*)lvls[i].name, GLUT_BITMAP_HELVETICA_12);

		char buf2[24];
		iSetColor(140, 140, 170);
		sprintf(buf2, "Best: %d", bestScore[i + 1]);
		iText(lvls[i].cx - (int)(strlen(buf2)*3), lvls[i].cy - rad - 22, buf2, GLUT_BITMAP_HELVETICA_12);

		if (sel) {
			iSetColor(255, 240, 80);
			iText(lvls[i].cx - 44, lvls[i].cy + rad + 28, "PRESS ENTER", GLUT_BITMAP_HELVETICA_12);
		}
	}

	iSetColor(100, 100, 130);
	iText(W / 2 - 220, 30, "CLICK a level or use LEFT/RIGHT + ENTER to play  |  ESC = Menu", GLUT_BITMAP_HELVETICA_12);
}

void drawGameOver() {
	iSetColor(0, 0, 0);
	iFilledRectangle(0, 0, W, H);
	iSetColor(8, 4, 20);
	iFilledRectangle(0, 0, W, H);
	iSetColor(0, 0, 0);
	iFilledRectangle(W / 2 - 200, H / 2 - 130, 400, 260);
	iSetColor(200, 40, 50);
	iRectangle(W / 2 - 200, H / 2 - 130, 400, 260);
	iSetColor(180, 30, 40);
	iRectangle(W / 2 - 198, H / 2 - 128, 396, 256);
	iSetColor(255, 70, 80);
	iText(W / 2 - 100, H / 2 + 90, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24);
	char buf[60];
	iSetColor(255, 200, 0);
	sprintf(buf, "Score: %d", score);
	iText(W / 2 - 80, H / 2 + 50, buf, GLUT_BITMAP_HELVETICA_18);
	sprintf(buf, "Best: %d", bestScore[currentLevel]);
	iText(W / 2 - 80, H / 2 + 22, buf, GLUT_BITMAP_HELVETICA_18);
	iSetColor(0, 200, 120);
	const char* ln[] = { "", "FOREST", "DESERT", "SPACE" };
	sprintf(buf, "Level: %s", ln[currentLevel]);
	iText(W / 2 - 80, H / 2 - 6, buf, GLUT_BITMAP_HELVETICA_18);
	iSetColor(160, 160, 200);
	sprintf(buf, "Length: %d   Foods: %d   Bots: %d", plrLen, foodsEaten, botsKilled);
	iText(W / 2 - 155, H / 2 - 34, buf, GLUT_BITMAP_HELVETICA_12);
	if (currentLevel == FOREST && bestScore[1] >= 100 && !unlocked[2]) {
		iSetColor(255, 160, 0);
		iText(W / 2 - 148, H / 2 - 60, "DESERT UNLOCKED! Go to Level Select!", GLUT_BITMAP_HELVETICA_12);
	}
	if (currentLevel == DESERT && bestScore[2] >= 300 && !unlocked[3]) {
		iSetColor(170, 80, 255);
		iText(W / 2 - 148, H / 2 - 60, "SPACE UNLOCKED! Go to Level Select!", GLUT_BITMAP_HELVETICA_12);
	}
	iSetColor(180, 180, 180);
	iText(W / 2 - 148, H / 2 - 90, "R = Restart  |  L = Level Select  |  ESC = Menu", GLUT_BITMAP_HELVETICA_18);
}
