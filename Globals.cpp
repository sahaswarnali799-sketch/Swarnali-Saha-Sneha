#include "Globals.h"

State state = SPLASH;
Level currentLevel = FOREST;

float camX = 0, camY = 0;

int  score = 0;
int  bestScore[4] = { 0, 0, 0, 0 };
bool unlocked[4] = { false, true, true, true }; // All 3 levels always available
int  foodsEaten = 0;
int  botsKilled = 0;

int  menuSel = 0;
int  levelSel = 0;
bool showCreds = false;

int   splashTimer = 0;
float nebulaAngle = 0.0f;  // rotating nebula ring animation
float snakeWiggle = 0.0f;  // snake body wiggle on level select
float starX[150], starY[150], starSpd[150];
bool  starsInit = false;

// ── Level Select background image texture ──
unsigned int levelSelectTex = 0;
bool         levelSelectTexLoaded = false;

float levelUpTimer = 0.0f;
char  levelUpText[64] = "";
char  levelUpSub[64] = "";
float gameOverTimer = 0.0f;

int   mouseX = W / 2, mouseY = H / 2;
bool  mouseActive = false;
