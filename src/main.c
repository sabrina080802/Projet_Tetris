#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur d'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }

    // Initialisation de SDL_image pour le support PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "Erreur d'initialisation de SDL_image : %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Création de la fenêtre en plein écran exclusif
    SDL_Window* window = SDL_CreateWindow("Tetris SDL",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          1871, 1000,
                                          SDL_WINDOW_FULLSCREEN);
    if (!window) {
        fprintf(stderr, "Erreur lors de la création de la fenêtre : %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Création du renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Erreur lors de la création du renderer : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Chargement de l'image PNG "Accueil.png"
    SDL_Surface* imageSurface = IMG_Load("ressources/Accueil.png");
    if (!imageSurface) {
        fprintf(stderr, "Erreur de chargement de l'image Accueil.png : %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!imageTexture) {
        fprintf(stderr, "Erreur de création de la texture pour Accueil.png : %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Chargement de l'image PNG "background.png"
    SDL_Surface* bgSurface = IMG_Load("ressources/background.png");
    if (!bgSurface) {
        fprintf(stderr, "Erreur de chargement de l'image background.png : %s\n", IMG_GetError());
        SDL_DestroyTexture(imageTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    if (!backgroundTexture) {
        fprintf(stderr, "Erreur de création de la texture pour background.png : %s\n", SDL_GetError());
        SDL_DestroyTexture(imageTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    Uint32 startTime = SDL_GetTicks();
    int running = 1;
    SDL_Event event;
    while (running) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }
            }
        }

        Uint32 elapsed = SDL_GetTicks() - startTime;
        SDL_RenderClear(renderer);
        if (elapsed < 5000) {  
            SDL_RenderCopy(renderer, imageTexture, NULL, NULL);
        } else {             
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);  
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
