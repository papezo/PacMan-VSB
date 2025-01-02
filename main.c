#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include "menu/menu.h"
#include "menu/loadScore.h"
#include "config/config.h"
#include "menu/sound.h"
#include <SDL2/SDL_mixer.h>
#include "game/core.h"

#include "game/game.h"

bool initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

void closeSDL()
{
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    if (!initSDL())
    {
        printf("Failed to initialize SDL or TTF!\n");
        return -1;
    }

    initAudio();
    loadSounds();

    SDL_Window *window = SDL_CreateWindow("Pac-Man", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        closeSDL();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        closeSDL();
        return -1;
    }

    initMenu();
    initScore();

    bool quit = false;
    bool wasInMenu = false;

    while (!quit)
    {

        SDL_Event e;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }

            menuSelection(&e, renderer, WINDOW_WIDTH);
        }

        if (isInMenu)
        {
            if (!wasInMenu)
            {
                playMenuMusic();
                wasInMenu = true;
            }

            if (inOptions)
            {
                renderOptions(renderer, WINDOW_WIDTH);
            }
            else
            {
                renderMenu(renderer, WINDOW_WIDTH);
            }
        }
        else if (gameRunning)
        {
            if (wasInMenu)
            {
                wasInMenu = false;
                stopMusic();
                playIntroMusic();
            }

            gameLoop(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

            if (gameState == GAME_STATE_GAMEOVER)
            {
                isInMenu = true;
                renderMenu(renderer, WINDOW_WIDTH);
                gameRunning = false;
            }
        }

        SDL_Delay(16);
    }

    closeMenu();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    closeSDL();
    closeAudio();

    return 0;
}
