#include "game.h"
#include "../entities/entities.h"
#include "../menu/loadScore.h"
#include "../config/config.h"
#include "wallChecker.h"
#include "../menu/menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "../menu/sound.h"
#include "points.h"
#include <math.h> // kvůli sqrtf
#include "pacmanAI.h"
#include "fruit.h"
#include "field.h"
#include "coins.h"
#include "../game/AI/bfs/bfs.h"
#include "ghostsAI.h"

const int ghostDelay = 3000;
const int frames = 15;

GameState gameState = GAME_STATE_MENU;

extern int gameField[HEIGHT][WIDTH];
extern Rectangle field[HEIGHT][WIDTH];

TTF_Font *gameFont = NULL;
TTF_Font *gameTitleFont = NULL;

SDL_Texture *heartTexture = NULL;
SDL_Texture *pacmanOpenTexture = NULL;
SDL_Texture *pacmanClosedTexture = NULL;
SDL_Texture *blinkyTexture = NULL;
SDL_Texture *clydeTexture = NULL;
SDL_Texture *inkyTexture = NULL;
SDL_Texture *pinkyTexture = NULL;
SDL_Texture *ghostFrighetenedTexture = NULL;

extern Ghost Blinky;
extern Ghost Pinky;
extern Ghost Inky;
extern Ghost Clyde;

int maxScore = 0;
int gameScore = 0;

int pacmanFrameCounter = 0;

bool isPacmanOpen = true;

float pacManX = 640.0f;
float pacManY = 302.0f;
float pacManSpeed = 80.0f; // Rychlost pohybu v pixelech

int offsetX = 0;
int offsetY = 0;
int direction = 3;      // 0 dolů, 1 nahoru, 2 vlevo, 3 vpravo
int nextDirection = -1; // -1 nechceme změnu směru
int frameCounter = 60;
int gameTicks = 0;

Uint32 readyStartTime = 0;

void gameLoop(SDL_Renderer *renderer, int screenWidth, int screenHeight)
{
    gameFont = TTF_OpenFont("font/Emulogic-zrEw.ttf", 24);
    gameTitleFont = TTF_OpenFont("font/Emulogic-zrEw.ttf", 24);
    int running = 1;
    Uint32 lastTime = SDL_GetTicks(); // Čas posledního snímku
    Uint32 currentTime, deltaTime;

    initializeGhosts();
    gameState = GAME_STATE_READY;
    readyStartTime = SDL_GetTicks();

    while (running == 1)
    {
        updateFruit();
        if (!fruit.active && (gameScore % 500 == 0 && gameScore != 0))
        {
            spawnFruit();
        }

        currentTime = SDL_GetTicks();
        deltaTime = currentTime - lastTime; // Výpočet rozdílu času mezi snímky
        lastTime = currentTime;

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = 0;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                SDL_Keycode code = e.key.keysym.sym;

                if (code == SDLK_UP)
                {
                    nextDirection = 1;
                }
                else if (code == SDLK_DOWN)
                {
                    nextDirection = 0;
                }
                else if (code == SDLK_LEFT)
                {
                    nextDirection = 2;
                }
                else if (code == SDLK_RIGHT)
                {
                    nextDirection = 3;
                }
            }
        }

        if (Blinky.mode == GHOST_FRIGHTENED && currentTime - Blinky.modeStartTime >= Blinky.modeDuration)
        {
            Blinky.mode = GHOST_SCATTER;
        }
        if (Pinky.mode == GHOST_FRIGHTENED && currentTime - Pinky.modeStartTime >= Pinky.modeDuration)
        {
            Pinky.mode = GHOST_SCATTER;
        }
        if (Inky.mode == GHOST_FRIGHTENED && currentTime - Inky.modeStartTime >= Inky.modeDuration)
        {
            Inky.mode = GHOST_SCATTER;
        }
        if (Clyde.mode == GHOST_FRIGHTENED && currentTime - Clyde.modeStartTime >= Clyde.modeDuration)
        {
            Clyde.mode = GHOST_SCATTER;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (gameState == GAME_STATE_READY)
        {
            renderReady(renderer);
            if (currentTime - readyStartTime >= 5000)
            {
                gameState = GAME_STATE_PLAYING;
            }
        }
        else if (gameState == GAME_STATE_PLAYING)
        {
            updatePacManPosition(deltaTime / 1000.0f);
            updateAllGhosts(currentTime);
            checkCollisionWithGhosts();
        }
        else if (gameState == GAME_STATE_GAMEOVER)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
            SDL_RenderClear(renderer);

            renderGameOver(renderer); // Zobrazí Game Over text
            SDL_RenderPresent(renderer);

            // Čekání na stisk klávesy pro ukončení hry
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_KEYDOWN)
                {
                    exit(0); // Ukončení hry po stisknutí libovolné klávesy
                }
            }
            SDL_Delay(16);
            continue;
        }

        // Animace Pac-Mana
        pacmanFrameCounter++;
        if (pacmanFrameCounter >= 10)
        {
            pacmanFrameCounter = 0;
            isPacmanOpen = !isPacmanOpen;
        }

        SDL_Texture *currentPacmanTexture = isPacmanOpen ? pacmanOpenTexture : pacmanClosedTexture;

        // Vykreslení herního pole
        for (int row = 0; row < HEIGHT; row++)
        {
            for (int col = 0; col < WIDTH; col++)
            {
                if (gameField[row][col] == 1)
                {
                    drawWalls(renderer);
                }
            }
        }

        // Vykreslení skóre a životů
        SDL_Color textColor = {255, 255, 255, 255};

        char scoreText[256];
        snprintf(scoreText, sizeof(scoreText), "1UP");
        initText(renderer, scoreText, gameFont, textColor, 300, 50);

        char gameScoreText[256];
        snprintf(gameScoreText, sizeof(gameScoreText), "%d", gameScore);
        initText(renderer, gameScoreText, gameFont, textColor, 310, 110);

        char topScoreText[256];
        snprintf(topScoreText, sizeof(topScoreText), "HIGH SCORE");
        initText(renderer, topScoreText, gameFont, textColor, 550, 50);

        char topScoreValueText[256];
        snprintf(topScoreValueText, sizeof(topScoreValueText), "%d", maxScore);
        initText(renderer, topScoreValueText, gameFont, textColor, 650, 110);

        char livesText[50];
        snprintf(livesText, sizeof(livesText), "Lives");
        initText(renderer, livesText, gameFont, textColor, 900, 50);

        for (int i = 0; i < lives; i++)
        {
            drawHeart(renderer, heartTexture, 910 + (i * 40), 110);
        }

        // Vykreslení Pac-Mana a duchů
        drawCoinsOnMap(renderer);
        drawPacMan(renderer, currentPacmanTexture, pacManX, pacManY, direction);

        drawGhosts(renderer, &Blinky);
        drawGhosts(renderer, &Pinky);
        drawGhosts(renderer, &Inky);
        drawGhosts(renderer, &Clyde);

        int pacManRow = (pacManY - offsetY) / TILE_SIZE;
        int pacManCol = (pacManX - offsetX) / TILE_SIZE;

        if (pacManRow == fruit.row && pacManCol == fruit.col)
        {
            eatFruit(pacManRow, pacManCol);
        }
        drawFruit(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void startGame(SDL_Renderer *renderer, int screenWidth, int screenHeight)
{
    srand(time(NULL));
    importGameField("maps/level1.txt");
    initializePowerPallets();

    SDL_Window *window = SDL_CreateWindow("Pac-Man Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    heartTexture = loadHeart(renderer, "img/pacman1.jpg");
    pacmanOpenTexture = loadPacMan(renderer, "img/pacman1.jpg");
    pacmanClosedTexture = loadPacMan(renderer, "img/pacman2.jpg");
    blinkyTexture = loadGhosts(renderer, "img/Ghosts/Blinky.png");
    clydeTexture = loadGhosts(renderer, "img/Ghosts/Clyde.png");
    inkyTexture = loadGhosts(renderer, "img/Ghosts/Inky.png");
    pinkyTexture = loadGhosts(renderer, "img/Ghosts/Pinky.png");
    ghostFrighetenedTexture = loadGhosts(renderer, "img/Ghosts/scaredGhost.png");

    initializeField(screenWidth, screenHeight);
    gameLoop(renderer, screenWidth, screenHeight);

    SDL_DestroyTexture(heartTexture);
    SDL_DestroyTexture(blinkyTexture);
    SDL_DestroyTexture(clydeTexture);
    SDL_DestroyTexture(inkyTexture);
    SDL_DestroyTexture(pinkyTexture);
    SDL_DestroyTexture(ghostFrighetenedTexture);

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
