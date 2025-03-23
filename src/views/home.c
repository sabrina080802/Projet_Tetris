#include "home.h"
#include "marathon.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>

static SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        fprintf(stderr, "Erreur chargement %s : %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if (!tex) {
        fprintf(stderr, "Erreur texture %s : %s\n", path, SDL_GetError());
    }
    return tex;
}
void showHome(SDL_Renderer* renderer) {
    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    SDL_Texture* bgTex = loadTexture(renderer, "ressources/background.png");
    SDL_Texture* logoTex = loadTexture(renderer, "ressources/logo.png");
    SDL_Texture* marathonTex = loadTexture(renderer, "ressources/marathon.png");
    SDL_Texture* classiqueTex = loadTexture(renderer, "ressources/classique.png");
    SDL_Texture* duelTex = loadTexture(renderer, "ressources/duel.png");
    SDL_Texture* quitterTex = loadTexture(renderer, "ressources/quitter.png");
    SDL_Texture* soundOnTex = loadTexture(renderer, "ressources/bobActif.png");
    SDL_Texture* soundOffTex = loadTexture(renderer, "ressources/bobInactif.png");
    if (!bgTex || !logoTex || !marathonTex || !classiqueTex || !duelTex ||
        !quitterTex || !soundOnTex || !soundOffTex) {
        if (bgTex) SDL_DestroyTexture(bgTex);
        if (logoTex) SDL_DestroyTexture(logoTex);
        if (marathonTex) SDL_DestroyTexture(marathonTex);
        if (classiqueTex) SDL_DestroyTexture(classiqueTex);
        if (duelTex) SDL_DestroyTexture(duelTex);
        if (quitterTex) SDL_DestroyTexture(quitterTex);
        if (soundOnTex) SDL_DestroyTexture(soundOnTex);
        if (soundOffTex) SDL_DestroyTexture(soundOffTex);
        return;
    }
SDL_Rect bgRect = {0, 0, winW, winH};

    int logoW, logoH;
    SDL_QueryTexture(logoTex, NULL, NULL, &logoW, &logoH);
    SDL_Rect logoRect = {(winW - logoW) / 2, 50, logoW, logoH};

    int menuSpacing = 40; 
    int marathonW, marathonH;
    SDL_QueryTexture(marathonTex, NULL, NULL, &marathonW, &marathonH);
    SDL_Rect marathonRect = {(winW - marathonW) / 2, logoRect.y + logoRect.h + 80, marathonW, marathonH};

    int classiqueW, classiqueH;
    SDL_QueryTexture(classiqueTex, NULL, NULL, &classiqueW, &classiqueH);
    SDL_Rect classiqueRect = {(winW - classiqueW) / 2, marathonRect.y + marathonH + menuSpacing, classiqueW, classiqueH};

    int duelW, duelH;
    SDL_QueryTexture(duelTex, NULL, NULL, &duelW, &duelH);
    SDL_Rect duelRect = {(winW - duelW) / 2, classiqueRect.y + classiqueH + menuSpacing, duelW, duelH};

    int quitterW, quitterH;
    SDL_QueryTexture(quitterTex, NULL, NULL, &quitterW, &quitterH);
    SDL_Rect quitterRect = {(winW - quitterW) / 2, duelRect.y + duelH + menuSpacing, quitterW, quitterH};

    int soundW, soundH;
    SDL_QueryTexture(soundOnTex, NULL, NULL, &soundW, &soundH);
    int margin = 20;
    SDL_Rect soundRect = {winW - soundW - margin, margin, soundW, soundH};

    int running = 1;
    int soundActive = 1;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = 0;
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;
              if (mx >= soundRect.x && mx <= soundRect.x + soundRect.w &&
                    my >= soundRect.y && my <= soundRect.y + soundRect.h) {
                    soundActive = !soundActive;
            } else if (mx >= marathonRect.x && mx <= marathonRect.x + marathonRect.w &&
                           my >= marathonRect.y && my <= marathonRect.y + marathonRect.h) {
                    SDL_DestroyTexture(bgTex);
                    SDL_DestroyTexture(logoTex);
                    SDL_DestroyTexture(marathonTex);
                    SDL_DestroyTexture(classiqueTex);
                    SDL_DestroyTexture(duelTex);
                    SDL_DestroyTexture(quitterTex);
                    SDL_DestroyTexture(soundOnTex);
                    SDL_DestroyTexture(soundOffTex);
                    running = 0;
                    showMarathon(renderer);
                    return;
                } else if (mx >= classiqueRect.x && mx <= classiqueRect.x + classiqueRect.w &&
                           my >= classiqueRect.y && my <= classiqueRect.y + classiqueRect.h) {
                    running = 0;
                    // showClassique(renderer);
                } else if (mx >= duelRect.x && mx <= duelRect.x + duelRect.w &&
                           my >= duelRect.y && my <= duelRect.y + duelRect.h) {
                    running = 0;
                    // showDuel(renderer);
                } else if (mx >= quitterRect.x && mx <= quitterRect.x + quitterRect.w &&
                           my >= quitterRect.y && my <= quitterRect.y + quitterRect.h) {
                    running = 0;
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTex, NULL, &bgRect);
        SDL_RenderCopy(renderer, logoTex, NULL, &logoRect);
        SDL_RenderCopy(renderer, marathonTex, NULL, &marathonRect);
        SDL_RenderCopy(renderer, classiqueTex, NULL, &classiqueRect);
        SDL_RenderCopy(renderer, duelTex, NULL, &duelRect);
        SDL_RenderCopy(renderer, quitterTex, NULL, &quitterRect);
        if (soundActive)
            SDL_RenderCopy(renderer, soundOnTex, NULL, &soundRect);
        else
            SDL_RenderCopy(renderer, soundOffTex, NULL, &soundRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(bgTex);
    SDL_DestroyTexture(logoTex);
    SDL_DestroyTexture(marathonTex);
    SDL_DestroyTexture(classiqueTex);
    SDL_DestroyTexture(duelTex);
    SDL_DestroyTexture(quitterTex);
    SDL_DestroyTexture(soundOnTex);
    SDL_DestroyTexture(soundOffTex);
}