#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <SDL2/SDL.h>
#include "../logic/pieces.h"

typedef struct {
    char modeName[32];
    char bestScore[64];
    char scoreText[32];
    char linesText[32];
    char nextText[32];
} GameModeInfo;


int showGameMode(SDL_Renderer* renderer, GameModeInfo modeInfo, Tetromino* current, Tetromino* next, SDL_Texture* blockTextures[]);

#endif
