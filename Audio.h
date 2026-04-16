#ifndef AUDIO_H
#define AUDIO_H

#ifdef _WIN32
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

extern bool coinPlaying;
extern int  coinTimer;
extern char currentBgmFile[64];

void playMusic(const char* file);
void stopMusic();
void playMusicAndRemember(const char* file);
void playCoinSFX();
void playBgMusic();
void setLevelMusic();
void updateMusicTimer();

#endif // AUDIO_H
