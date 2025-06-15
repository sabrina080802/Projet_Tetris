#pragma once

#include "screens/sound.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define DEFAULT_W 800
#define DEFAULT_H 600

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SoundComponent sounds;
} App;

int initApp(App *app, const char *title, int width, int height, Uint32 flags);
void cleanupApp(App *app);
