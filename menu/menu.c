#include "menu.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL_mixer.h>
#include "../game/core.h"
#include "sound.h"
#include "../config/config.h"
#include "../game/game.h"
#include "menu.h"

bool isInMenu = true;
bool quit = false;

TTF_Font *font = NULL;
TTF_Font *titleFont = NULL;
TTF_Font *scoreFont = NULL;

SDL_Texture *backgroundTexture = NULL;

const char *menu[] = {"Start Game", "Settings", "Quit"};
int items = sizeof(menu) / sizeof(menu[0]);
SDL_Rect menuRects[3];
bool menuSelected[3] = {false, false, false};

bool inOptions = false;
int windowWidth = 1280;
int windowHeight = 1024;
bool gameRunning = false;

const char *optionsMenu[] = {"Master Volume", "Sound Effects", "Lives", "Back"};
int optionsItems = sizeof(optionsMenu) / sizeof(optionsMenu[0]);
SDL_Rect optionsRects[4];

int masterVolume = 50;
bool soundEffectsEnabled = true;
int lives = 3;

void setMasterVolume(int volume)
{
    if (volume < 0)
    {
        volume = 0;
    }
    else if (volume > 100)
    {
        volume = 100;
    }

    masterVolume = volume;

    int sdlVolume = volume * MIX_MAX_VOLUME / 100;
    Mix_Volume(-1, sdlVolume);
    Mix_VolumeMusic(sdlVolume);
}

void renderText(SDL_Renderer *renderer, const char *text, int x, int y, int width, int height, TTF_Font *font)
{
    SDL_Color color = {255, 255, 0};
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {x, y, width, height};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void renderOutlinedText(SDL_Renderer *renderer, const char *text, int x, int y, int width, int height, TTF_Font *font)
{
    SDL_Color outlineColor = {0, 0, 0};  
    SDL_Color textColor = {255, 255, 0}; 

    int offsets[8][2] = {{-2, -2}, {2, -2}, {-2, 2}, {2, 2}, {0, -2}, {0, 2}, {-2, 0}, {2, 0}};
    for (int i = 0; i < 8; i++)
    {
        SDL_Surface *outlineSurface = TTF_RenderText_Solid(font, text, outlineColor);
        SDL_Texture *outlineTexture = SDL_CreateTextureFromSurface(renderer, outlineSurface);
        SDL_Rect outlineRect = {x + offsets[i][0], y + offsets[i][1], width, height};
        SDL_RenderCopy(renderer, outlineTexture, NULL, &outlineRect);
        SDL_DestroyTexture(outlineTexture);
        SDL_FreeSurface(outlineSurface);
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {x, y, width, height};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void initOptionsMenu()
{
    for (int i = 0; i < optionsItems; i++)
    {
        int textWidth = 0;
        int textHeight = 0;
        TTF_SizeText(font, optionsMenu[i], &textWidth, &textHeight);

        optionsRects[i].w = textWidth + 20;
        optionsRects[i].h = textHeight + 10;

        optionsRects[i].x = (windowWidth - optionsRects[i].w) / 2;
        optionsRects[i].y = (windowHeight - (optionsItems * optionsRects[i].h)) / 2 + i * (optionsRects[i].h + 100);
    }
}

void initMenu()
{
    font = TTF_OpenFont("font/Emulogic-zrEw.ttf", 36);
    titleFont = TTF_OpenFont("font/Emulogic-zrEw.ttf", 72);
    scoreFont = TTF_OpenFont("font/Emulogic-zrEw.ttf", 36);

    if (!font || !titleFont || !scoreFont)
    {
        printf("Font could not be loaded! SDL_ttf Error: %s\n", TTF_GetError());
    }

    for (int i = 0; i < items; i++)
    {
        int textWidth = 0;
        int textHeight = 0;
        TTF_SizeText(font, menu[i], &textWidth, &textHeight);

        menuRects[i].w = textWidth + 20;
        menuRects[i].h = textHeight + 10;

        menuRects[i].x = (windowWidth - menuRects[i].w) / 2;
        menuRects[i].y = (windowHeight - (items * menuRects[i].h)) / 2 + i * (menuRects[i].h + 50);
    }

    initOptionsMenu();
}

void renderOptions(SDL_Renderer *renderer, int screenWidth)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);                     

    int titleWidth = 0, titleHeight = 0;
    TTF_SizeText(titleFont, "Settings", &titleWidth, &titleHeight);
    renderText(renderer, "Settings", (windowWidth - titleWidth) / 2, 50, titleWidth, titleHeight, titleFont);

    int sectionX = 100;      // X souřadnice texty
    int sectionWidth = 1080; // Šířka sekcí
    int sectionHeight = 60;  // Výška sekcí
    int spacingY = 30;       //  mezera mezi text
    int baseY = 250;         // Počáteční Y souřadnice

    int textWidth = 0, textHeight = 0;

    // Master Volume
    int masterY = baseY;
    TTF_SizeText(font, "Master Volume", &textWidth, &textHeight);
    renderText(renderer, "Master Volume", sectionX + 40, masterY + (sectionHeight - textHeight) / 2, textWidth, textHeight, font);

    int sliderWidth = 400;
    int sliderHeight = 20;
    int sliderX = sectionX + sectionWidth - sliderWidth - 50;
    int sliderY = masterY + (sectionHeight - sliderHeight) / 2;

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect sliderBackground = {sliderX, sliderY, sliderWidth, sliderHeight};
    SDL_RenderFillRect(renderer, &sliderBackground);

    // Posuvník zvuku
    int sliderPosition = (masterVolume * sliderWidth) / 100;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect sliderForeground = {sliderX, sliderY, sliderPosition, sliderHeight};
    SDL_RenderFillRect(renderer, &sliderForeground);

    // Sound Effects
    int soundEffectsY = masterY + sectionHeight + spacingY;
    SDL_Rect soundEffectsRect = {sectionX, soundEffectsY, sectionWidth, sectionHeight};
    TTF_SizeText(font, "Sound Effects", &textWidth, &textHeight);
    renderText(renderer, "Sound Effects", sectionX + 40, soundEffectsY + (sectionHeight - textHeight) / 2, textWidth, textHeight, font);

    const char *toggleText = soundEffectsEnabled ? "ON" : "OFF";
    TTF_SizeText(font, toggleText, &textWidth, &textHeight);
    renderText(renderer, toggleText, sectionX + sectionWidth - textWidth - 50, soundEffectsY + (sectionHeight - textHeight) / 2, textWidth, textHeight, font);

    // Lives
    int livesY = soundEffectsY + sectionHeight + spacingY;
    TTF_SizeText(font, "Lives", &textWidth, &textHeight);
    renderText(renderer, "Lives", sectionX + 40, livesY + (sectionHeight - textHeight) / 2, textWidth, textHeight, font);

    char livesText[10];
    sprintf(livesText, "%d", lives);
    TTF_SizeText(font, livesText, &textWidth, &textHeight);
    renderText(renderer, livesText, 1070, livesY + (sectionHeight - textHeight) / 2, textWidth, textHeight, font);

    // Tlačítko -
    SDL_Rect minusButton = {1020, livesY + (sectionHeight - 40) / 2, 40, 40};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &minusButton);
    renderText(renderer, "-", minusButton.x + 10, minusButton.y, 20, 40, font);

    // Tlačítko +
    SDL_Rect plusButton = {1120, livesY + (sectionHeight - 40) / 2, 40, 40};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &plusButton);
    renderText(renderer, "+", plusButton.x + 10, plusButton.y, 20, 40, font);

    // Save and Exit
    int buttonWidth = 300;
    int buttonHeight = 70;

    SDL_Rect saveButton = {700, 900, buttonWidth, buttonHeight};

    TTF_SizeText(font, "Apply Changes", &textWidth, &textHeight);
    renderText(renderer, "Apply Changes",
               saveButton.x + (buttonWidth - textWidth) / 2,
               saveButton.y + (buttonHeight - textHeight) / 2,
               textWidth, textHeight, font);

    // Back
    SDL_Rect backButton = {200, 900, buttonWidth, buttonHeight};

    TTF_SizeText(font, "Back", &textWidth, &textHeight);
    renderText(renderer, "Back",
               backButton.x + (buttonWidth - textWidth) / 2,
               backButton.y + (buttonHeight - textHeight) / 2,
               textWidth, textHeight, font);

    // Uložení tlačítek pro detekci kliknutí
    optionsRects[0] = sliderBackground; // Master Volume 
    optionsRects[1] = soundEffectsRect; // Sound Effects 
    optionsRects[2] = minusButton;      // Tlačítko -
    optionsRects[3] = plusButton;       // Tlačítko +
    SDL_RenderPresent(renderer);
}
void menuSelection(SDL_Event *e, SDL_Renderer *renderer, int screenWidth)
{
    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);

    static bool dragging = false; 

    if (inOptions)
    {
        int sectionX = 100;      
        int sectionWidth = 1080; 
        int sliderWidth = 400;   
        int sliderHeight = 20;   
        int sliderX = sectionX + sectionWidth - sliderWidth - 50;
        int sliderY = 250 + (60 - sliderHeight) / 2; 

        int soundEffectsY = 250 + 60 + 30; 
        SDL_Rect soundEffectsRect = {sectionX, soundEffectsY, sectionWidth, 60};

        int livesY = soundEffectsY + 60 + 30;
        SDL_Rect minusButton = {1020, livesY + (60 - 40) / 2, 40, 40};
        SDL_Rect plusButton = {1120, livesY + (60 - 40) / 2, 40, 40};

        if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
        {

            SDL_Rect backButton = {200, 900, 300, 70};
            if (mouseX >= backButton.x && mouseX <= backButton.x + backButton.w &&
                mouseY >= backButton.y && mouseY <= backButton.y + backButton.h)
            {
                inOptions = false;
                return;
            }

            SDL_Rect applyButton = {700, 900, 300, 70};
            if (mouseX >= applyButton.x && mouseX <= applyButton.x + applyButton.w &&
                mouseY >= applyButton.y && mouseY <= applyButton.y + applyButton.h)
            {
                return;
            }

            if (mouseY >= sliderY && mouseY <= sliderY + sliderHeight &&
                mouseX >= sliderX && mouseX <= sliderX + sliderWidth)
            {
                dragging = true;
            }

            if (mouseX >= soundEffectsRect.x && mouseX <= soundEffectsRect.x + soundEffectsRect.w &&
                mouseY >= soundEffectsRect.y && mouseY <= soundEffectsRect.y + soundEffectsRect.h)
            {
                soundEffectsEnabled = !soundEffectsEnabled;
            }
            if (mouseX >= minusButton.x && mouseX <= minusButton.x + minusButton.w &&
                mouseY >= minusButton.y && mouseY <= minusButton.y + minusButton.h)
            {
                if (lives > 1)
                {
                    lives--;
                }
            }

            if (mouseX >= plusButton.x && mouseX <= plusButton.x + plusButton.w &&
                mouseY >= plusButton.y && mouseY <= plusButton.y + plusButton.h)
            {
                if(lives < 5)
                {
                    lives++;
                }
                
            }
        }

        if (dragging && e->type == SDL_MOUSEMOTION)
        {
            int newVolume = ((mouseX - sliderX) * 100) / sliderWidth;

            if (newVolume < 0)
                newVolume = 0;
            if (newVolume > 100)
                newVolume = 100;

            setMasterVolume(newVolume);
        }

        if (e->type == SDL_MOUSEBUTTONUP && e->button.button == SDL_BUTTON_LEFT)
        {
            dragging = false;
        }
    }
    else
    {
        for (int i = 0; i < items; i++)
        {
            if (mouseX >= menuRects[i].x && mouseX <= menuRects[i].x + menuRects[i].w &&
                mouseY >= menuRects[i].y && mouseY <= menuRects[i].y + menuRects[i].h)
            {
                if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT)
                {
                    if (i == 0)
                    {
                        isInMenu = false;
                        gameRunning = true;

                        stopMusic();      
                        playIntroMusic();
                        startGame(renderer, windowWidth, windowHeight);
                    }
                    else if (i == 1) // Settings
                    {
                        inOptions = true;;
                    }
                    else if (i == 2) // Quit
                    {
                        quit = true;
                        exit(0);
                    }
                }
            }
        }
    }
}

void renderMenu(SDL_Renderer *renderer, int screenWidth)
{
    SDL_Surface *background = IMG_Load("img/wallpaper.jpg");

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, background);
    SDL_FreeSurface(background);

    SDL_RenderClear(renderer);

    SDL_Rect backgroundRect = {0, 0, windowWidth, windowHeight};
    SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

    SDL_DestroyTexture(backgroundTexture);

    int titleWidth = 0;
    TTF_SizeText(titleFont, "Pac Man", &titleWidth, NULL);
    renderOutlinedText(renderer, "Pac Man", (windowWidth - titleWidth) / 2, 20, titleWidth, 50, titleFont);

    int scoreWidth = 0;
    char scoreText[50];
    sprintf(scoreText, "Top Score: %d", maxScore);
    TTF_SizeText(scoreFont, scoreText, &scoreWidth, NULL);
    renderText(renderer, scoreText, (windowWidth - scoreWidth) / 2, 90, scoreWidth, 30, scoreFont);

    for (int i = 0; i < items; i++)
    {
        renderText(renderer, menu[i], menuRects[i].x, menuRects[i].y, menuRects[i].w, menuRects[i].h, font);
    }

    SDL_RenderPresent(renderer);
}

void closeMenu()
{
    TTF_CloseFont(font);
    TTF_CloseFont(titleFont);
    TTF_CloseFont(scoreFont);
}
