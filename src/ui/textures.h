#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL2/SDL.h>

extern SDL_Texture* blockTextures[7];
extern SDL_Texture* ghostTexture;

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);
void initPiecesTextures(SDL_Renderer* renderer);

#endif
