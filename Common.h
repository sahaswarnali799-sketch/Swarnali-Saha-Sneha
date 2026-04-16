#ifndef COMMON_H
#define COMMON_H

// ══════════════════════════════════════════════════════════
//  WINDOW & WORLD CONFIG
// ══════════════════════════════════════════════════════════
#ifdef _WIN32
#include <Windows.h>
#endif

#define W          1000
#define H          800
#define WORLD_W    2000
#define WORLD_H    2000
#define CELL       20
#define project    "Snake Master"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ══════════════════════════════════════════════════════════
//  GAME STATES
// ══════════════════════════════════════════════════════════
enum State { SPLASH, MAIN_MENU, LEVEL_SELECT, PLAYING, PAUSED, GAME_OVER };
enum Level { FOREST = 1, DESERT = 2, SPACE = 3 };

#endif // COMMON_H
