#ifndef WALLCHECKER_H
#define WALLCHECKER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

int pacManWallChecker(float x, float y);
bool ghostWallChecker(float x, float y);


#endif