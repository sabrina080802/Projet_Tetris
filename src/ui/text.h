#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Color color, int* w, int* h);
TTF_Font* loadFont(const char* path, int size);
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path);

#endif
