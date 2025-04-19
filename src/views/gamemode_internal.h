#ifndef GAMEMODE_INTERNAL_H
#define GAMEMODE_INTERNAL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);
SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int* w, int* h);
void initPieceBag();
int getNextPieceType();

#endif
