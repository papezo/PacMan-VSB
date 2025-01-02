#include "game.h"
#include "core.h"
#include "../entities/entities.h"
#include "pacmanAI.h"
#include "ghostsAI.h"


void drawCoinsOnMap(SDL_Renderer *renderer)
{
    int coinSize = TILE_SIZE / 5;
    int coinOffSet = (TILE_SIZE - coinSize) / 2;

    int powerPalletSize = TILE_SIZE / 1.5;
    int powerPalletOffset = (TILE_SIZE - powerPalletSize) / 2;

    int pacManCol = (pacManX - offsetX) / TILE_SIZE;
    int pacManRow = (pacManY - offsetY) / TILE_SIZE;

    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            if (gameField[row][col] == 2)  // Peníz
            {
                SDL_Rect coinRect = {
                    field[row][col].x + coinOffSet,
                    field[row][col].y + coinOffSet,
                    coinSize,
                    coinSize};

                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
                SDL_RenderFillRect(renderer, &coinRect);

                if (pacManCol == col && pacManRow == row)
                {
                    gameField[row][col] = 0;  
                    updateScore(10); 
                }
            }

            if (gameField[row][col] == 3)  // Booster
            {
                int centerX = field[row][col].x + TILE_SIZE / 2;
                int centerY = field[row][col].y + TILE_SIZE / 2;
                int radius = TILE_SIZE / 4;

                SDL_SetRenderDrawColor(renderer, 255, 218, 185, 255);  
                draw_circle(renderer, centerX, centerY, radius);

                if (pacManCol == col && pacManRow == row)
                {
                    gameField[row][col] = 2;  
                    updateScore(50);  

                    Blinky.mode = GHOST_FRIGHTENED;
                    Pinky.mode = GHOST_FRIGHTENED;
                    Inky.mode = GHOST_FRIGHTENED;
                    Clyde.mode = GHOST_FRIGHTENED;

                    Uint32 currentTime = SDL_GetTicks();
                    Blinky.modeDuration = Pinky.modeDuration = Inky.modeDuration = Clyde.modeDuration = 10000;
                    Blinky.modeStartTime = Pinky.modeStartTime = Inky.modeStartTime = Clyde.modeStartTime = currentTime;
                }
            }
        }
    }
}
