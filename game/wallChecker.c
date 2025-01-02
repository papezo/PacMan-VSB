#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "game.h"
#include "wallChecker.h"
#include "ghostsAI.h"


int pacManWallChecker(float x, float y)
{
    x -= offsetX;
    y -= offsetY;

    const float TOLERANCE = 8.0f; 

    float leftBoundary = x + TOLERANCE;
    float rightBoundary = x + TILE_SIZE - TOLERANCE;
    float topBoundary = y + TOLERANCE;
    float bottomBoundary = y + TILE_SIZE - TOLERANCE;

    int left = (int)(leftBoundary / TILE_SIZE);
    int right = (int)(rightBoundary / TILE_SIZE);
    int top = (int)(topBoundary / TILE_SIZE);
    int bottom = (int)(bottomBoundary / TILE_SIZE);

    if (gameField[top][left] == 1 || gameField[top][right] == 1 ||
        gameField[bottom][left] == 1 || gameField[bottom][right] == 1)
    {
        return 0; // Stěna
    }

    return 1; // Volný prostor
}



bool ghostWallChecker(float x, float y)
{
    x -= offsetX;
    y -= offsetY;

    const float tolerance = 4.0f;

    float leftBoundary   = x + tolerance;
    float rightBoundary  = x + TILE_SIZE - tolerance;
    float topBoundary    = y + tolerance;
    float bottomBoundary = y + TILE_SIZE - tolerance;

    int left   = (int)(leftBoundary / TILE_SIZE);
    int right  = (int)(rightBoundary  / TILE_SIZE);
    int top    = (int)(topBoundary    / TILE_SIZE);
    int bottom = (int)(bottomBoundary / TILE_SIZE);

    if (top < 0 || top >= HEIGHT || left < 0 || left >= WIDTH ||
        right < 0 || right >= WIDTH || bottom < 0 || bottom >= HEIGHT)
    {
        return false; 
    }

    if (gameField[top][left] == 1    || gameField[top][right] == 1 ||
        gameField[bottom][left] == 1 || gameField[bottom][right] == 1)
    {
        return false; // Stěna
    }

    return true; 
}


