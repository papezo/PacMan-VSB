#ifndef PACMANAI_H
#define PACMANAI_H

#include "game.h"


extern float pacManX;
extern float pacManY;
extern int direction;
extern int nextDirection;
extern int pacmanFrameCounter;
extern bool isPacmanOpen;
extern float pacManSpeed;

extern Uint32 lastDeathTime ;
extern const int respawnDelay;

extern int gameScore;
extern int maxScore;

void initScore();
void updateScore(int points);
void updatePacManPosition(float deltaTime);
void resetGame();
void checkCollisionWithGhosts();

#endif
