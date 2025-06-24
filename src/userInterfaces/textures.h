#pragma once
#include <SDL2/SDL.h>
#include "ressources.h"
#include <SDL2/SDL_image.h>

extern SDL_Texture *blockTextures[8];
extern SDL_Texture *ghostTexture;

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path);
void initPiecesTextures(SDL_Renderer *renderer);
