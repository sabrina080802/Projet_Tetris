#pragma once

#include <SDL2/SDL.h>
#include "../logic/pieces.h"
#include "../logic/grid.h"
#include "../logic/pieces.h"
#include "../ui/ressources.h"
#include "../ui/textures.h"
#include "../ui/text.h"
#include "../ui/render.h"
#include "../screens/pause.h"
#include "../logic/score.h"
#include "../logic/timer.h"
#include "../screens/gamemodeover.h"
#include "../views/gamemode_internal.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

typedef struct
{
    char modeName[32];
    char bestScore[64];
    char scoreText[32];
    char linesText[32];
    char nextText[32];
} GameModeInfo;

int showGameMode(SDL_Renderer *renderer, GameModeInfo modeInfo, Tetromino *current, Tetromino *next, SDL_Texture *blockTextures[]);
