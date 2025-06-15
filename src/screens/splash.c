#include "splash.h"

void showSplash(SDL_Renderer *renderer)
{
    SDL_Texture *splashTexture = loadTexture(renderer, "ressources/Accueil.png");
    Uint32 startTime = SDL_GetTicks();
    int running = 1;

    SDL_Event event;
    while (running && (SDL_GetTicks() - startTime < SPLASH_TIME))
    {
        while (SDL_PollEvent(&event))
        {
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
