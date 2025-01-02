#ifndef FRUIT_H
#define FRUIT_H

#include "game.h"

#define FRUIT_CHERRY 0
#define FRUIT_STRAWBERRY 1
#define FRUIT_APPLE 2
#define FRUIT_ORANGE 3

extern Fruit fruit;
extern Rectangle field[HEIGHT][WIDTH];

extern void spawnFruit();
extern void updateFruit();
extern void eatFruit(int row, int col);
extern void drawFruit(SDL_Renderer *renderer);

#endif
