#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#define SOUND_HEIGHT 100

extern const int STATS_PIECE_SIZE;

extern const SDL_Color WHITE;
extern const SDL_Color VIOLET_FONCE;
extern const SDL_Color VIOLET_BORD;

float lerp(float a, float b, float delta);