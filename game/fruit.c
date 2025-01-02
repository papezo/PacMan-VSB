#include "game.h"
#include "fruit.h"
#include "../entities/entities.h"
#include "../menu/loadScore.h"
#include "../menu/menu.h"
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include "../menu/loadScore.h"  
#include "pacmanAI.h"
#include "coins.h"

Fruit fruit = {false, 10, 11, 0, 10000, FRUIT_CHERRY, 100};

FruitInfo fruitInfo[] = {
    {FRUIT_CHERRY, "Cherry", 100, "img/fruits/cherry.png"},
    {FRUIT_STRAWBERRY, "Strawberry", 200, "img/fruits/strawberry.png"},
    {FRUIT_APPLE, "Apple", 300, "img/fruits/apple.png"},
    {FRUIT_ORANGE, "Orange", 500, "img/fruits/orange.png"}};



void spawnFruit()
{
    if (!fruit.active)
    {
        int emptyPositions[HEIGHT * WIDTH][2];
        int emptyCount = 0;

        for (int row = 0; row < HEIGHT; row++)
        {
            for (int col = 0; col < WIDTH; col++)
            {
                if (gameField[row][col] == 0)  
                {
                    emptyPositions[emptyCount][0] = row;
                    emptyPositions[emptyCount][1] = col;
                    emptyCount++;
                }
            }
        }

        if (emptyCount > 0)
        {
            int randomIndex = rand() % emptyCount;
            fruit.row = emptyPositions[randomIndex][0];
            fruit.col = emptyPositions[randomIndex][1];

            fruit.active = true;
            fruit.spawnTime = SDL_GetTicks();

            int typeIndex = rand() % 4;
            fruit.type = fruitInfo[typeIndex].type;
            fruit.points = fruitInfo[typeIndex].points;

            gameField[fruit.row][fruit.col] = 4;
        }
    }
}

void updateFruit()
{
    if (fruit.active && SDL_GetTicks() - fruit.spawnTime > fruit.duration)
    {
        fruit.active = false;
        gameField[fruit.row][fruit.col] = 0;
    }
}

void eatFruit(int row, int col)
{
    if (fruit.active && fruit.row == row && fruit.col == col)
    {
        updateScore(fruit.points);
        fruit.active = false;
        gameField[fruit.row][fruit.col] = 0;
    }
}

void drawFruit(SDL_Renderer *renderer)
{
    if (fruit.active)
    {
        SDL_Texture *fruitTexture = IMG_LoadTexture(renderer, fruitInfo[fruit.type].texturePath);
        if (fruitTexture)
        {
            SDL_Rect fruitRect = {
                field[fruit.row][fruit.col].x,
                field[fruit.row][fruit.col].y,
                TILE_SIZE,
                TILE_SIZE};
            SDL_RenderCopy(renderer, fruitTexture, NULL, &fruitRect);
            SDL_DestroyTexture(fruitTexture);
        }
    }
}
