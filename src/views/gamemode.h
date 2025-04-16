#ifndef GAMEMODE_H
#define GAMEMODE_H

#include <SDL2/SDL.h>
#include "../logic/pieces.h"


typedef struct {
    const char* modeName;
    const char* bestScore;
    const char* scoreText;
    const char* linesText;
    const char* nextText;
} GameModeInfo;

void showGameMode(SDL_Renderer* renderer, GameModeInfo modeInfo, Tetromino* current, SDL_Texture* blockTextures[]);

#endif
