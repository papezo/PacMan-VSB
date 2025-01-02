#include "points.h"
#include <stdlib.h>
#include <time.h>

#include "core.h"
#include "game.h"
#include "wallChecker.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <stdio.h>

int powerPalletPos[4][2] = {
    {1, 1},
    {1, 20},
    {18, 1},
    {18, 20},
};


void initializePowerPallets()
{
    for (int i = 0; i < 4; i++)
    {
        int row = powerPalletPos[i][0];
        int col = powerPalletPos[i][1];

        if (gameField[row][col] == 2)
        {
            gameField[row][col] = 0; // Označit pozici jako power pallet
        }
    }
}

