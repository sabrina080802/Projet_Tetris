#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include "../textures.h"

typedef struct
{
    const char *text;
    SDL_Renderer *renderer;
    SDL_Rect rect;
    SDL_Texture *texture;
} Label;

void initLabel(Label *label, const char *text, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color);
void renderLabel(Label *label);
void destroyLabel(Label *label);

const char *msTimeToString(float msTime);