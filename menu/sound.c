#include <SDL2/SDL_mixer.h>
#include <stdio.h>

Mix_Music *menuMusic = NULL;
Mix_Music *introMusic = NULL;
Mix_Chunk *coinSound = NULL;

void initAudio()
{
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 8192);
    Mix_AllocateChannels(8); 
}

void loadSounds()
{
    menuMusic = Mix_LoadMUS("img/sound.ogg");
    introMusic = Mix_LoadMUS("img/intro.mp3");
    coinSound = Mix_LoadWAV("img/eating.mp3");
}

void playMenuMusic()
{
    Mix_PlayMusic(menuMusic, -1); 
}

void playIntroMusic()
{
    Mix_PlayMusic(introMusic, 1); 
}

void playCoinSound()
{
    Mix_PlayChannel(-1, coinSound, 0); 
}

void stopMusic()
{
    Mix_HaltMusic();
}

void closeAudio()
{
    Mix_FreeMusic(menuMusic);
    menuMusic = NULL;

    Mix_FreeMusic(introMusic);
    introMusic = NULL;

    Mix_FreeChunk(coinSound);
    coinSound = NULL;

    Mix_CloseAudio();
}
