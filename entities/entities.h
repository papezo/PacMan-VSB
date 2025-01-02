#ifndef ENTITIES_H
#define ENTITIES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>


// Funkce pro načítání textur
SDL_Texture *loadHeart(SDL_Renderer *renderer, const char *imagePath);
SDL_Texture *loadPacMan(SDL_Renderer *renderer, const char *imagePath);
SDL_Texture *loadGhosts(SDL_Renderer *renderer, const char *imagePath);


void drawPacMan(SDL_Renderer *renderer, SDL_Texture *texture, float x, float y, int direction);
void drawHeart(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y);
void drawGhosts(SDL_Renderer *renderer, Ghost *ghost);


void renderReady(SDL_Renderer *renderer);
void renderGameOver(SDL_Renderer *renderer);
void draw_circle(SDL_Renderer *renderer, int x, int y, int radius);
void drawWallLines(SDL_Renderer *renderer, int x, int y, int tileSize, int top, int right, int bottom, int left);
void drawWalls(SDL_Renderer *renderer);
void initText(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y);

void closeText();
void closeGame();

#endif