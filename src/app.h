#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <limits.h>
#include <stdio.h>
#include "userInterfaces/fonts.h"
#include "userInterfaces/components/sound.h"

#define DEFAULT_W 800
#define DEFAULT_H 600

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SoundComponent sounds;
    TTF_Font *smallFont;
    TTF_Font *bigFont;
    Mix_Music *music;
    Mix_Chunk *lineSound;
} App;

int initApp(App *app, const char *title, int width, int height, Uint32 flags);
void cleanupApp(App *app);
