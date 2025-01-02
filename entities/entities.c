#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../game/core.h"
#include "../game/game.h"
#include "../game/wallChecker.h"
#include "../config/config.h"
#include <math.h>
#include <limits.h>
#include "../game/coins.h"


SDL_Texture *loadHeart(SDL_Renderer *renderer, const char *imagePath)
{
    SDL_Surface *tempSurface = IMG_Load(imagePath);
    SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(tempSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(tempSurface);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, formattedSurface);
    SDL_FreeSurface(formattedSurface);
    return texture;
}

SDL_Texture *loadGhosts(SDL_Renderer *renderer, const char *imagePath)
{
    SDL_Surface *tempSurface = IMG_Load(imagePath);
    SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(tempSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(tempSurface);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, formattedSurface);
    SDL_FreeSurface(formattedSurface);
    return texture;
}

SDL_Texture *loadPacMan(SDL_Renderer *renderer, const char *imagePath)
{
    SDL_Surface *tempSurface = IMG_Load(imagePath);
    SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(tempSurface, SDL_PIXELFORMAT_RGBA8888, 0);
    SDL_FreeSurface(tempSurface);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, formattedSurface);
    SDL_FreeSurface(formattedSurface);
    return texture;
}


void drawPacMan(SDL_Renderer *renderer, SDL_Texture *texture, float x, float y, int direction) {
    if (texture != NULL) {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        SDL_Rect renderQuad = {
            (int)roundf(x),
            (int)roundf(y),
            TILE_SIZE,  // šířka
            TILE_SIZE   // výška
        };

        double angle = 0.0;
        switch (direction) {
            case 1: angle = 270.0; break;
            case 0: angle = 90.0;  break; 
            case 2: angle = 180.0; break; 
            case 3: angle = 0.0;   break; 
        }

        SDL_RenderCopyEx(renderer, texture, NULL, &renderQuad, angle, NULL, SDL_FLIP_NONE);
    }
}



void drawGhosts(SDL_Renderer *renderer, Ghost *ghost)
{
    SDL_Texture *ghostTexture = NULL;

    if (ghost->mode == GHOST_FRIGHTENED) {
        ghostTexture = ghostFrighetenedTexture; 
    } else {
        switch (ghost->type) {
            case GHOST_TYPE_BLINKY:
                ghostTexture = blinkyTexture;
                break;
            case GHOST_TYPE_PINKY:
                ghostTexture = pinkyTexture;
                break;
            case GHOST_TYPE_INKY:
                ghostTexture = inkyTexture;
                break;
            case GHOST_TYPE_CLYDE:
                ghostTexture = clydeTexture;
                break;
            default:
                break;
        }
    }

    if (ghostTexture != NULL)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_Rect renderQuad = {ghost->x, ghost->y, 30, 30}; 
        SDL_RenderCopy(renderer, ghostTexture, NULL, &renderQuad);
    }
}



void drawHeart(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y)
{
    if (texture != NULL)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_Rect renderQuad = {x, y, 30, 30};
        SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
    }
}

void draw_circle(SDL_Renderer *renderer, int x, int y, int radius)
{
    for (int w = 0; w <= radius * 2; w++)
    {
        for (int h = 0; h <= radius * 2; h++)
        {
            int dx = radius - w; // Horizontal offset
            int dy = radius - h;  // Vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, x + dx, y + dy);
            }
        }
    }
}

void drawWalls(SDL_Renderer *renderer)
{
    int wallThickness = 4; // Tloušťka stěny
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    for (int row = 0; row < HEIGHT; row++)
    {
        for (int col = 0; col < WIDTH; col++)
        {
            if (gameField[row][col] == 1)
            {

                int x = offsetX + col * TILE_SIZE;
                int y = offsetY + row * TILE_SIZE;

                bool top = (row > 0 && gameField[row - 1][col] == 1);
                bool bottom = (row < HEIGHT - 1 && gameField[row + 1][col] == 1);
                bool left = (col > 0 && gameField[row][col - 1] == 1);
                bool right = (col < WIDTH - 1 && gameField[row][col + 1] == 1);

                if (!top)
                {
                    SDL_Rect line = {x, y, TILE_SIZE, wallThickness};
                    SDL_RenderFillRect(renderer, &line);
                }
                if (!bottom)
                {
                    SDL_Rect line = {x, y + TILE_SIZE - wallThickness, TILE_SIZE, wallThickness};
                    SDL_RenderFillRect(renderer, &line);
                }
                if (!left)
                {
                    SDL_Rect line = {x, y, wallThickness, TILE_SIZE};
                    SDL_RenderFillRect(renderer, &line);
                }
                if (!right)
                {
                    SDL_Rect line = {x + TILE_SIZE - wallThickness, y, wallThickness, TILE_SIZE};
                    SDL_RenderFillRect(renderer, &line);
                }
            }
        }
    }
}

void drawWallLines(SDL_Renderer *renderer, int x, int y, int tileSize, int top, int right, int bottom, int left)
{
    int wallThickness = tileSize / 5; // Tloušťka stěny
    int halfTile = tileSize / 2;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); 

    if (top)
    {
        SDL_Rect topRect = {x + (tileSize - wallThickness) / 2, y, wallThickness, halfTile};
        SDL_RenderFillRect(renderer, &topRect);
    }

    if (bottom)
    {
        SDL_Rect bottomRect = {x + (tileSize - wallThickness) / 2, y + halfTile, wallThickness, halfTile};
        SDL_RenderFillRect(renderer, &bottomRect);
    }

    if (left)
    {
        SDL_Rect leftRect = {x, y + (tileSize - wallThickness) / 2, halfTile, wallThickness};
        SDL_RenderFillRect(renderer, &leftRect);
    }

    if (right)
    {
        SDL_Rect rightRect = {x + halfTile, y + (tileSize - wallThickness) / 2, halfTile, wallThickness};
        SDL_RenderFillRect(renderer, &rightRect);
    }

    if (!top && !right && !bottom && !left)
    {
        SDL_Rect centerRect = {x + (tileSize - wallThickness) / 2, y + (tileSize - wallThickness) / 2, wallThickness, wallThickness};
        SDL_RenderFillRect(renderer, &centerRect);
    }
}

void renderReady(SDL_Renderer *renderer)
{
    SDL_Color color = {255, 255, 0, 255};
    TTF_Font *readyFont = TTF_OpenFont("font/Emulogic-zrEw.ttf", 14);

    const char *readyText = "READY!";
    SDL_Surface *readySurface = TTF_RenderText_Solid(readyFont, readyText, color);
    SDL_Texture *readyTexture = SDL_CreateTextureFromSurface(renderer, readySurface);

    int textWidth = 0;
    int textHeight = 0;
    TTF_SizeText(readyFont, readyText, &textWidth, &textHeight);

    SDL_Rect textRect = {
        600,
        580,
        textWidth,
        textHeight};

    SDL_RenderCopy(renderer, readyTexture, NULL, &textRect);

    SDL_DestroyTexture(readyTexture);
    SDL_FreeSurface(readySurface);
    TTF_CloseFont(readyFont);
}

void renderGameOver(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

    SDL_Color color = {255, 0, 0, 255}; 
    TTF_Font *gameOverFont = TTF_OpenFont("font/Emulogic-zrEw.ttf", 72);

    const char *gameOverText = "GAME OVER";
    SDL_Surface *gameOverSurface = TTF_RenderText_Solid(gameOverFont, gameOverText, color);
    SDL_Texture *gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);

    int textWidth, textHeight;
    TTF_SizeText(gameOverFont, gameOverText, &textWidth, &textHeight);
    SDL_Rect textRect = {(WINDOW_WIDTH - textWidth) / 2, (WINDOW_HEIGHT - textHeight) / 2, textWidth, textHeight};
    SDL_RenderCopy(renderer, gameOverTexture, NULL, &textRect);

    SDL_DestroyTexture(gameOverTexture);
    SDL_FreeSurface(gameOverSurface);
    TTF_CloseFont(gameOverFont);

    SDL_RenderPresent(renderer);

    SDL_Delay(5000);
    exit(0); 
}

void initText(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void closeText()
{
    if (gameFont)
    {
        TTF_CloseFont(gameFont);
        gameFont = NULL;
    }
    if (gameTitleFont)
    {
        TTF_CloseFont(gameTitleFont);
        gameTitleFont = NULL;
    }
}

void closeGame()
{
    if (heartTexture)
    {
        SDL_DestroyTexture(heartTexture);
        heartTexture = NULL;
    }
}