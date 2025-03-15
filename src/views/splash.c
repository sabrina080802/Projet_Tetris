#include "splash.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

void showSplash(SDL_Renderer* renderer) {
    SDL_Surface* splashSurface = IMG_Load("ressources/Accueil.png");
    if (!splashSurface) {
        fprintf(stderr, "Erreur de chargement de Accueil.png : %s\n", IMG_GetError());
        return;
    }
    SDL_Texture* splashTexture = SDL_CreateTextureFromSurface(renderer, splashSurface);
    SDL_FreeSurface(splashSurface);
    if (!splashTexture) {
        fprintf(stderr, "Erreur de création de la texture pour Accueil.png : %s\n", SDL_GetError());
        return;
    }
    Uint32 startTime = SDL_GetTicks();
    int running = 1;
    SDL_Event event;
    while (running && (SDL_GetTicks() - startTime < 5000)) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = 0;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, splashTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
    SDL_DestroyTexture(splashTexture);
}
