#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

extern bool isInMenu;
extern int score;
extern int currentLevel;
extern TTF_Font *font;
extern TTF_Font *titleFont;
extern TTF_Font *scoreFont;
extern const char *menu[];
extern int items;
extern SDL_Rect menuRects[];
extern bool menuSelected[];

extern const char *optionsMenu[];
extern int optionsItems;
extern SDL_Rect optionsRects[];
extern bool inOptions;

extern int masterVolume;
extern bool gameRunning;

extern bool quit;


void initMenu();
void renderMenu(SDL_Renderer *renderer, int screenWidth);
void menuSelection(SDL_Event *e, SDL_Renderer *renderer, int screenWidth);
void closeMenu();
void renderText(SDL_Renderer *renderer, const char *text, int x, int y, int width, int height, TTF_Font *font);
void renderOptions(SDL_Renderer *renderer, int screenWidth);


extern void initGame(SDL_Renderer *renderer, int screenWidth, int screenHeight);

#endif 