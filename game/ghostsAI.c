#include "game.h"
#include "core.h"
#include "ghostsAI.h"
#include <SDL2/SDL.h>
#include <math.h> 
#include "wallChecker.h"
#include "AI/bfs/bfs.h"
#include "pacmanAI.h"
#include "../config/config.h"

const int ghostDelayy = 3000;

Ghost Blinky = {625.0f, 425.0f, 5.0f, 0, 0, false, GHOST_SCATTER, 0, 0, 21, 21, GHOST_TYPE_BLINKY};
Ghost Pinky = {580.0f, 510.0f, 5.0f, 3, 0, false, GHOST_SCATTER, 0, 0, 0, 27, GHOST_TYPE_PINKY};
Ghost Inky = {620.0f, 510.0f, 5.0f, 3, 0, false, GHOST_SCATTER, 0, 0, 0, 27, GHOST_TYPE_INKY};
Ghost Clyde = {660.0f, 510.0f, 5.0f, 3, 0, false, GHOST_SCATTER, 0, 0, 0, 27, GHOST_TYPE_CLYDE};

void initializeGhosts()
{
    Uint32 currentTime = SDL_GetTicks();

    Blinky.spawnTime = currentTime + 0;
    Pinky.spawnTime = currentTime + 7000;
    Inky.spawnTime = currentTime + 20000;
    Clyde.spawnTime = currentTime + 10000;

    Blinky.isActive = false;
    Pinky.isActive = false;
    Inky.isActive = false;
    Clyde.isActive = false;
}

void eatGhost(Ghost *ghost)
{
    if (ghost == NULL)
    {
        return; 
    }

    Uint32 currentTime = SDL_GetTicks();

    // Spawn na původní pozici
    if (ghost->type == GHOST_TYPE_BLINKY)
    {
        ghost->x = 625.0f;
        ghost->y = 425.0f;
        updateScore(200);
    }
    else if (ghost->type == GHOST_TYPE_PINKY)
    {
        ghost->x = 580.0f; 
        ghost->y = 510.0f;
        updateScore(400);
    }
    else if (ghost->type == GHOST_TYPE_INKY)
    {
        ghost->x = 620.0f; 
        ghost->y = 510.0f;
        updateScore(800);
    }
    else if (ghost->type == GHOST_TYPE_CLYDE)
    {
        ghost->x = 660.0f;
        ghost->y = 510.0f;
        updateScore(1600);
    }

    // Nastavení režimu zpět
    ghost->mode = GHOST_SCATTER;

    // Nastavení doby trvání režimu
    ghost->modeStartTime = currentTime;
    ghost->modeDuration = 10000;
}

void moveGhost(Ghost *ghost)
{
    float speed = ghost->speed;

    if (ghost->mode == GHOST_FRIGHTENED)
    {
        ghost->direction = rand() % 4; // Duch se pohybuje náhodně
    }

    switch (ghost->direction)
    {
    case 0: // dolů
        if (ghostWallChecker(ghost->x, ghost->y + speed))
        {
            ghost->y += speed;
        }
        else
        {
            ghost->direction = rand() % 4; 
        }
        break;
    case 1: // nahoru
        if (ghostWallChecker(ghost->x, ghost->y - speed))
        {
            ghost->y -= speed;
        }
        else
        {
            ghost->direction = rand() % 4;
        }
        break;
    case 2: // doleva
        if (ghostWallChecker(ghost->x - speed, ghost->y))
        {
            ghost->x -= speed;
        }
        else
        {
            ghost->direction = rand() % 4;
        }
        break;
    case 3: // doprava
        if (ghostWallChecker(ghost->x + speed, ghost->y))
        {
            ghost->x += speed;
        }
        else
        {
            ghost->direction = rand() % 4;
        }
        break;
    default:
        break;
    }
}

void switchGhostMode(Ghost *ghost, Uint32 currentTime)
{
    Uint32 elapsed = currentTime - ghost->modeStartTime;

    if (elapsed >= ghost->modeDuration)
    {
        if (ghost->mode == GHOST_CHASE)
        {
            ghost->mode = GHOST_SCATTER;
        }
        else if (ghost->mode == GHOST_SCATTER)
        {
            ghost->mode = GHOST_CHASE;
        }
        ghost->modeStartTime = currentTime; //reset času
    }
}
void updateGhostMovement() {
    // Pokud životy jsou více než 0 a duchové by měli být aktivní
    if (lives > 0) {
        Uint32 currentTime = SDL_GetTicks();

        // Kontrola, zda duchové mohou začít hýbat (aktivace po zpoždění)
        startGhostMovement(&Blinky, currentTime);
        startGhostMovement(&Pinky, currentTime);
        startGhostMovement(&Inky, currentTime);
        startGhostMovement(&Clyde, currentTime);

        // Pokud jsou aktivní, mohou se začít hýbat
        if (Blinky.isActive) {
            moveGhost(&Blinky);
        }
        if (Pinky.isActive) {
            moveGhost(&Pinky);
        }
        if (Inky.isActive) {
            moveGhost(&Inky);
        }
        if (Clyde.isActive) {
            moveGhost(&Clyde);
        }
    }
}



void updateGhostPosition(Ghost *ghost, Uint32 currentTime)
{
    if (!ghost->isActive)
        return;

    Uint32 elapsed = currentTime - ghost->modeStartTime;
    if (elapsed >= ghost->modeDuration)
    {
        switchGhostMode(ghost, currentTime);
    }

    //Pohyb duchů podle režimu
    moveGhost(ghost); 

    int pacManRow = (int)((pacManY - offsetY) / TILE_SIZE);
    int pacManCol = (int)((pacManX - offsetX) / TILE_SIZE);

    int ghostRow = (int)((ghost->y - offsetY) / TILE_SIZE);
    int ghostCol = (int)((ghost->x - offsetX) / TILE_SIZE);

    if (gameField[ghostRow][ghostCol] == 5) {
        // Pokud je duch v oblasti označené číslem 5, teleportuj ho na opačnou stranu mapy
        if (ghostCol < 5) { // Levý tunel teleportuje naprav
            ghostCol = 20;
        } else { // Pravý tunel teleportuje nalevo
            ghostCol = 1;
        }

        ghost->x = ghostCol * TILE_SIZE + offsetX;
        ghostRow = (int)((ghost->y - offsetY) / TILE_SIZE);
        

        return; 
    }

    int targetRow = pacManRow, targetCol = pacManCol;
    switch (ghost->mode)
    {
    case GHOST_CHASE:
        if (ghost->type == GHOST_TYPE_BLINKY)
        {
            targetRow = pacManRow;
            targetCol = pacManCol;
        }
        break;

    case GHOST_SCATTER:
        targetRow = ghost->cornerRow;
        targetCol = ghost->cornerCol;
        break;
    default:
        break;
    }

    int bestDirection = bfsNextMove(ghostRow, ghostCol, targetRow, targetCol);

    if (bestDirection == -1)
    {
        ghost->direction = rand() % 4; // Pokud nevyjde BFS, pohybuj se nádhoě
    }
    else
    {
        ghost->direction = bestDirection;
    }

    moveGhost(ghost); 
}

void updateAllGhosts(Uint32 currentTime)
{
    startGhostMovement(&Blinky, currentTime);
    startGhostMovement(&Pinky, currentTime);
    startGhostMovement(&Inky, currentTime);
    startGhostMovement(&Clyde, currentTime);

    updateGhostPosition(&Blinky, currentTime);
    updateGhostPosition(&Pinky, currentTime);
    updateGhostPosition(&Inky, currentTime);
    updateGhostPosition(&Clyde, currentTime);
}

void startGhostMovement(Ghost *ghost, Uint32 currentTime) {
    if (!ghost->isActive && currentTime - ghost->spawnTime >= ghostDelayy) {
        ghost->isActive = true;  
        ghost->modeStartTime = currentTime;  
    }
}



