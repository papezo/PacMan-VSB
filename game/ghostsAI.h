#ifndef GHOST_AI_H
#define GHOST_AI_H

#include "game.h"

extern Ghost Blinky;   
extern Ghost Pinky;
extern Ghost Inky;
extern Ghost Clyde;

void initializeGhosts();
void startGhostMovement(Ghost *ghost, Uint32 currentTime);
void switchGhostMode(Ghost *ghost, Uint32 currentTime);
void moveGhost(Ghost *ghost);
void updateGhostPosition(Ghost *ghost, Uint32 currentTime);
void updateGhostMovement();
void updateAllGhosts(Uint32 currentTime);
void eatGhost(Ghost *ghost);

#endif
