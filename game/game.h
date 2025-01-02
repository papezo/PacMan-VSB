#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h> 
#include <stdbool.h> 

#define TILE_SIZE 30
#define WIDTH 22
#define HEIGHT 20

extern int maxScore;   
extern int gameScore;  

extern int offsetX; 
extern int offsetY;  

extern int gameField[HEIGHT][WIDTH]; 


typedef enum
{
    GAME_STATE_MENU,
    GAME_STATE_READY,
    GAME_STATE_PLAYING,
    GAME_STATE_GAMEOVER,
} GameState;

extern GameState gameState; 


typedef enum
{
    GHOST_CHASE,
    GHOST_SCATTER,
    GHOST_FRIGHTENED
} GhostMode;

typedef enum
{
    GHOST_TYPE_BLINKY,
    GHOST_TYPE_PINKY,
    GHOST_TYPE_INKY,
    GHOST_TYPE_CLYDE
} GhostType;


typedef struct
{
    float x, y;
    float speed;
    int direction;
    Uint32 spawnTime;
    bool isActive;
    GhostMode mode;
    Uint32 modeStartTime;
    Uint32 modeDuration;
    int cornerRow, cornerCol;
    GhostType type;
} Ghost;

typedef enum
{
    FRUIT_CHERRY,
    FRUIT_STRAWBERRY,
    FRUIT_ORANGE,
    FRUIT_APPLE,
} FruitType;

typedef struct
{
    bool active;
    int row, col;
    Uint32 spawnTime;
    Uint32 duration;
    FruitType type;
    int points;
} Fruit;

typedef struct
{
    FruitType type;
    char *name;
    int points;
    char *texturePath;
} FruitInfo;


typedef struct
{
    int x, y;
    int width, height;
    SDL_Color color;
} Rectangle;

extern Rectangle field[HEIGHT][WIDTH];

#endif
