#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Rect rect;
    SDL_Color bgColor;
    SDL_Color borderColor;
    int borderRadius;
} Frame;

Frame createFrame(SDL_Renderer *r, SDL_Color bg, SDL_Color borderColor, int radius);
void renderFrame(Frame *frame);
