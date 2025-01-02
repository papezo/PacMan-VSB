#include "game.h"
#include "core.h"
#include "ghostsAI.h"
#include <SDL2/SDL.h>
#include <math.h> 
#include "wallChecker.h"
#include "AI/bfs/bfs.h"
#include "pacmanAI.h"
#include "../config/config.h"
#include "../menu/loadScore.h"  
#include "coins.h"
#include "../config/config.h"

Uint32 lastDeathTime = 0; // Stores the time when Pac-Man died
const int respawnDelay = 3000;

void initScore() {
    maxScore = loadHighScore();
    gameScore = 0;
}

void updateScore(int points) {
    gameScore += points;
    if (gameScore > maxScore) {
        maxScore = gameScore;
        saveHighScore(maxScore);
    }
}

void updatePacManPosition(float deltaTime)
{
    const float speed = pacManSpeed * deltaTime;
    float newX = pacManX;
    float newY = pacManY;

    // Spočítání aktuální pozice Pac-Mana v gameField
    int pacManRow = (int)((pacManY - offsetY) / TILE_SIZE);
    int pacManCol = (int)((pacManX - offsetX) / TILE_SIZE);

    // Kontrola pro teleportaci (tunelem označeným číslem 5)
    if (gameField[pacManRow][pacManCol] == 5) {
        // Pokud je Pac-Man v oblasti s číslem 5, teleportuj se na opačnou stranu mapy
        if (pacManCol < 5) { // Levý tunel teleportuje napravo
            pacManCol = 20;
        } else { // Pravý tunel teleportuje nalevo
            pacManCol = 1;
        }

        pacManX = pacManCol * TILE_SIZE + offsetX;
        return; 
    }

  // Pokračování pohybu Pac-Mana podle směru  
    if (nextDirection != -1)
    {
        bool canMove = false;

        if (nextDirection == 0 && pacManWallChecker(pacManX, pacManY + speed)) {
            newY += speed;
            canMove = true;
        }
        else if (nextDirection == 1 && pacManWallChecker(pacManX, pacManY - speed)) {
            newY -= speed;
            canMove = true;
        }
        else if (nextDirection == 2 && pacManWallChecker(pacManX - speed, pacManY)) {
            newX -= speed;
            canMove = true;
        }
        else if (nextDirection == 3 && pacManWallChecker(pacManX + speed, pacManY)) {
            newX += speed;
            canMove = true;
        }

        if (canMove)
        {
            pacManX = newX;
            pacManY = newY;
            direction = nextDirection;
            nextDirection = -1; // Resetování směru
        }
    }
    else
    {
        if (direction == 0 && pacManWallChecker(pacManX, pacManY + speed)) {
            pacManY += speed;
        }
        else if (direction == 1 && pacManWallChecker(pacManX, pacManY - speed)) {
            pacManY -= speed;
        }
        else if (direction == 2 && pacManWallChecker(pacManX - speed, pacManY)) {
            pacManX -= speed;
        }
        else if (direction == 3 && pacManWallChecker(pacManX + speed, pacManY)) {
            pacManX += speed;
        }
    }
}

void resetGame() {
    pacManX = 640.0f;
    pacManY = 302.0f;
    direction = 3;

    Blinky.x = 625.0f;
    Blinky.y = 425.0f;
    Pinky.x = 580.0f;
    Pinky.y = 510.0f;
    Inky.x = 620.0f;
    Inky.y = 510.0f;
    Clyde.x = 660.0f;
    Clyde.y = 510.0f;

    Blinky.mode = GHOST_SCATTER;
    Pinky.mode = GHOST_SCATTER;
    Inky.mode = GHOST_SCATTER;
    Clyde.mode = GHOST_SCATTER;

    Blinky.isActive = false;
    Pinky.isActive = false;
    Inky.isActive = false;
    Clyde.isActive = false;

    Blinky.spawnTime = SDL_GetTicks() + 0;  
    Pinky.spawnTime = SDL_GetTicks() + 7000;  
    Inky.spawnTime = SDL_GetTicks() + 20000;   
    Clyde.spawnTime = SDL_GetTicks() + 10000;  
}

void checkCollisionWithGhosts() {
    int pacManRow = (int)((pacManY - offsetY) / TILE_SIZE);
    int pacManCol = (int)((pacManX - offsetX) / TILE_SIZE);

    if (gameState == GAME_STATE_PLAYING) {
        if (Blinky.mode == GHOST_FRIGHTENED && pacManRow == (int)((Blinky.y - offsetY) / TILE_SIZE) && pacManCol == (int)((Blinky.x - offsetX) / TILE_SIZE)) {
            gameScore += 100;
            Blinky.x = 625.0f;  // Reset pozice
            Blinky.y = 425.0f;
            Blinky.mode = GHOST_SCATTER;
            updateScore(200);
        }
        if (Pinky.mode == GHOST_FRIGHTENED && pacManRow == (int)((Pinky.y - offsetY) / TILE_SIZE) && pacManCol == (int)((Pinky.x - offsetX) / TILE_SIZE)) {
            gameScore += 100;
            Pinky.x = 580.0f;  // Reset pozice
            Pinky.y = 510.0f;
            Pinky.mode = GHOST_SCATTER;
            updateScore(400);
        }
        if (Inky.mode == GHOST_FRIGHTENED && pacManRow == (int)((Inky.y - offsetY) / TILE_SIZE) && pacManCol == (int)((Inky.x - offsetX) / TILE_SIZE)) {
            gameScore += 100;
            Inky.x = 620.0f;  // Reset pozice
            Inky.y = 510.0f;
            Inky.mode = GHOST_SCATTER;
            updateScore(800);
        }
        if (Clyde.mode == GHOST_FRIGHTENED && pacManRow == (int)((Clyde.y - offsetY) / TILE_SIZE) && pacManCol == (int)((Clyde.x - offsetX) / TILE_SIZE)) {
            gameScore += 100;
            Clyde.x = 660.0f;  // Reset pozice
            Clyde.y = 510.0f;
            Clyde.mode = GHOST_SCATTER;
            updateScore(1600);
        }

        // Pokud Pac-Man narazí na ducha v režimu chase/scatter (a není v režimu frightened)
        if (pacManRow == (int)((Blinky.y - offsetY) / TILE_SIZE) && pacManCol == (int)((Blinky.x - offsetX) / TILE_SIZE) && Blinky.mode != GHOST_FRIGHTENED) {
            lives--;
            if (lives <= 0) {
                gameState = GAME_STATE_GAMEOVER;  // Konec hry
                return;
            } else {
                resetGame();
                SDL_Delay(2000);  // Pauza po ztrátě života
            }
        }
        if (pacManRow == (int)((Pinky.y - offsetY) / TILE_SIZE) && pacManCol == (int)((Pinky.x - offsetX) / TILE_SIZE) && Pinky.mode != GHOST_FRIGHTENED) {
            lives--;
            if (lives <= 0) {
                gameState = GAME_STATE_GAMEOVER;
                return;
            } else {
                resetGame();
                SDL_Delay(2000);
            }
        }
        if (pacManRow == (int)((Inky.y - offsetY) / TILE_SIZE) && pacManCol == (int)((Inky.x - offsetX) / TILE_SIZE) && Inky.mode != GHOST_FRIGHTENED) {
            lives--;
            if (lives <= 0) {
                gameState = GAME_STATE_GAMEOVER;
                return;
            } else {
                resetGame();
                SDL_Delay(2000);
            }
        }
        if (pacManRow == (int)((Clyde.y - offsetY) / TILE_SIZE) && pacManCol == (int)((Clyde.x - offsetX) / TILE_SIZE) && Clyde.mode != GHOST_FRIGHTENED) {
            lives--;
            if (lives <= 0) {
                gameState = GAME_STATE_GAMEOVER;
                return;
            } else {
                resetGame();
                SDL_Delay(2000);
            }
        }
    }
}
