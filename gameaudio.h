#ifndef GAMEAUDIO_H
#define GAMEAUDIO_H

#include <nusys.h>

void initializeAudio();

void playSound(u32 soundId);
void playSoundAtDoublePitch(u32 soundId);
void stopLastPlayedSound();

void playMusic(u32 musicId);
void stopPlayingMusic();
void fadeOutMusic();
int isMusicPlaying();

#endif