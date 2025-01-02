#ifndef FIELD_H
#define FIELD_H

#include "game.h"


extern int gameField[HEIGHT][WIDTH];
extern Rectangle field[HEIGHT][WIDTH];

void importGameField(const char *map);
void initializeField(int screenWidth, int screenHeight);

#endif
