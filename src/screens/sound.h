#pragma once
#include "../ui/textures.h"
#include "../ui/ressources.h"
#include "../ui/button.h"
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Rect rect;
    SDL_Texture *textureOn;
    SDL_Texture *textureOff;
    bool isActive;
} SoundComponent;

bool tryToggleSound(int x, int y, SoundComponent *sound);
SoundComponent createSound(SDL_Renderer *r);
void renderSound(SoundComponent *sound);
