#include "Audio.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

bool coinPlaying = false;
int  coinTimer = 0;
char currentBgmFile[64] = "";

void playMusic(const char* file) {
#ifdef _WIN32
	PlaySoundA(file, NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
#else
	char cmd[220];
	sprintf(cmd, "killall -q aplay 2>/dev/null; aplay -q \"%s\" &", file);
	system(cmd);
#endif
}

void stopMusic() {
#ifdef _WIN32
	PlaySoundA(NULL, NULL, 0);
#else
	system("killall -q aplay 2>/dev/null");
#endif
}

void playMusicAndRemember(const char* file) {
	strncpy(currentBgmFile, file, 63);
	currentBgmFile[63] = '\0';
	playMusic(file);
}

void playCoinSFX() {
#ifdef _WIN32
	PlaySoundA("coin.wav", NULL, SND_FILENAME | SND_ASYNC);
	coinPlaying = true;
	coinTimer = 30;
#else
	char cmd[100];
	sprintf(cmd, "aplay -q coin.wav &");
	system(cmd);
#endif
}

void playBgMusic() {
	playMusicAndRemember("bg.wav");
}

void setLevelMusic() {
	playMusicAndRemember("level.wav");
}

void updateMusicTimer() {
#ifdef _WIN32
	if (coinPlaying && coinTimer > 0) {
		coinTimer--;
		if (coinTimer <= 0) {
			coinPlaying = false;
			if (currentBgmFile[0] != '\0')
				playMusic(currentBgmFile);
		}
	}
#endif
}
