#ifndef BFS_H
#define BFS_H

#include <SDL2/SDL.h>     
#include <stdbool.h>     
#include "../../game.h"

typedef struct
{
    int row;
    int col;
} Node;

static int directions[4][2] = {
    {1, 0},  // dolů
    {-1, 0}, // nahoru
    {0, -1}, // vlevo
    {0, 1}   // vpravo
};

int bfsNextMove(int startRow, int startCol, int goalRow, int goalCol);

#endif