#include "game.h"

int gameField[HEIGHT][WIDTH];
Rectangle field[HEIGHT][WIDTH];

void importGameField(const char *map)
{
    FILE *file = fopen(map, "r");
    if (file == NULL)
    {
        printf("Failed.\n");
        return;
    }

    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            if (fscanf(file, "%d", &gameField[row][col]) != 1)
            {
                printf("Failed.\n");
                fclose(file);
                return;
            }
        }
    }

    fclose(file);
}

void initializeField(int screenWidth, int screenHeight)
{
    int mazeWidth = WIDTH * TILE_SIZE;
    int mazeHeight = HEIGHT * TILE_SIZE;

    offsetX = (screenWidth - mazeWidth) / 2;
    offsetY = (screenHeight - mazeHeight) / 2;

    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            field[row][col].x = col * TILE_SIZE + offsetX;
            field[row][col].y = row * TILE_SIZE + offsetY;
            field[row][col].width = TILE_SIZE;
            field[row][col].height = TILE_SIZE;

            if (gameField[row][col] == 1)
            {
                field[row][col].color = (SDL_Color){0, 0, 255, 255}; 
            }
            else
            {
                field[row][col].color = (SDL_Color){0, 0, 0, 255}; 
            }
        }
    }
}
