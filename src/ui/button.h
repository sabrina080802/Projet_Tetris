#pragma once
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "textures.h"

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect rect;
} Button;

Button createButton(SDL_Renderer *r, const char *tex_path);
bool containsPoint(int x, int y, SDL_Rect rect);
void renderButton(Button *btn);
