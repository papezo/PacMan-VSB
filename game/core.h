#ifndef PACMAN_H
#define PACMAN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

extern int gameScore;

extern SDL_Texture *heartTexture;
extern SDL_Texture *pacmanOpenTexture;
extern SDL_Texture *pacmanClosedTexture;
extern SDL_Texture *blinkyTexture;
extern SDL_Texture *pinkyTexture;
extern SDL_Texture *inkyTexture;
extern SDL_Texture *clydeTexture;
extern SDL_Texture *ghostFrighetenedTexture;
extern int pacmanFrameCounter;
extern bool isPacmanOpen;

extern float pacManX;
extern float pacManY;

extern TTF_Font *font;
extern TTF_Font *gameFont;
extern TTF_Font *gameTitleFont;


extern int offsetX;
extern int offsetY;

void drawHeart(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y);
void initializeField(int screenWidth, int screenHeight);
void initText(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y);
void gameLoop(SDL_Renderer *renderer, int screenWidth, int screenHeight);  
void initScore();

void closeGame();
void closeText();
extern void startGame(SDL_Renderer *renderer, int screenWidth, int screenHeight);
extern void importGameField(const char *map);



#endif 
