#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gamemode_constants.h"
#include "../logic/grid.h"
#include "gamemode_constants.h"

// UI constants
#define UI_MARGIN 20
#define UI_SPACING 10
#define UI_WIDTH 250
#define MUSIC_BUTTON_SIZE 80
#define PAUSE_BUTTON_SIZE 80

// Couleurs
#define WHITE (SDL_Color){255, 255, 255, 255}
#define VIOLET_FONCE (SDL_Color){37, 17, 66, 127}
#define VIOLET_BORD (SDL_Color){137, 64, 247, 255}

// UI Rectangles
typedef struct
{
    SDL_Rect bgRect;
    SDL_Rect wellRect;
    SDL_Rect titleRect;
    SDL_Rect bestScoreRect;
    SDL_Rect statsRect;
    SDL_Rect statsLabelRect;
    SDL_Rect scoreRect;
    SDL_Rect scoreTextRect;
    SDL_Rect linesRect;
    SDL_Rect linesTextRect;
    SDL_Rect nextRect;
    SDL_Rect nextLabelRect;
    SDL_Rect musicRect;
    SDL_Rect pauseRect;
} GameModeUI;

void initGameModeUI(SDL_Renderer *renderer, GameModeUI *ui);
