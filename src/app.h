#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} App;

int initApp(App* app, const char* title, int width, int height, Uint32 flags);
void cleanupApp(App* app);

#endif
