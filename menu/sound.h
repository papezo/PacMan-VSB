#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>

void initAudio();
void loadSounds();
void playMenuMusic();
void playIntroMusic();
void playCoinSound();
void stopMusic();
void closeAudio();

#endif 
