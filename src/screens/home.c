#include "home.h"
#include "../modes/marathon.h"
#include "../modes/classique.h"
#include "../modes/duel.h"
#include "../ui/textures.h"
#include "../ui/ressources.h"
#include <SDL2/SDL.h>

int showHome(SDL_Renderer* renderer) {
    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    // Chargement des textures
    SDL_Texture* bgTex        = loadTexture(renderer, HOME_BG);
    SDL_Texture* logoTex      = loadTexture(renderer, HOME_LOGO);
    SDL_Texture* textures[4]  = {
        loadTexture(renderer, HOME_BTN_MARATHON),
        loadTexture(renderer, HOME_BTN_CLASSIQUE),
        loadTexture(renderer, HOME_BTN_DUEL),
        loadTexture(renderer, QUITTER)
    };
    SDL_Texture* soundOnTex   = loadTexture(renderer, ICON_MUSIC_ON);
    SDL_Texture* soundOffTex  = loadTexture(renderer, ICON_MUSIC_OFF);

    if (!bgTex || !logoTex || !textures[0] || !textures[1] || !textures[2] || !textures[3] || !soundOnTex || !soundOffTex)
        return 0;

    SDL_Rect bgRect = {0, 0, winW, winH};

    // Logo
    int logoW, logoH;
    SDL_QueryTexture(logoTex, NULL, NULL, &logoW, &logoH);
    SDL_Rect logoRect = { (winW - logoW) / 2, 80, logoW, logoH };

    // Boutons
    SDL_Rect rects[4];
    int spacing = 40;
    int btnTotalHeight = 0;
    int btnHeights[4];

    // On mesure la hauteur totale pour centrer les boutons
    for (int i = 0; i < 4; i++) {
        SDL_QueryTexture(textures[i], NULL, NULL, &rects[i].w, &rects[i].h);
        btnHeights[i] = rects[i].h;
        btnTotalHeight += rects[i].h;
        if (i < 3) btnTotalHeight += spacing;
    }

    int startY = (winH - btnTotalHeight) / 2 + 40;

    for (int i = 0; i < 4; i++) {
        rects[i].x = (winW - rects[i].w) / 2;
        rects[i].y = startY;
        startY += rects[i].h + spacing;
    }

    // Son
    SDL_Rect soundRect;
    SDL_QueryTexture(soundOnTex, NULL, NULL, &soundRect.w, &soundRect.h);
    soundRect.x = winW - soundRect.w - 20;
    soundRect.y = 20;

    int running = 1;
    int soundActive = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                return 0;

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x, my = e.button.y;

                // Clic son
                if (mx >= soundRect.x && mx <= soundRect.x + soundRect.w &&
                    my >= soundRect.y && my <= soundRect.y + soundRect.h) {
                    soundActive = !soundActive;
                }

                // Clic boutons
                for (int i = 0; i < 4; i++) {
                    if (mx >= rects[i].x && mx <= rects[i].x + rects[i].w &&
                        my >= rects[i].y && my <= rects[i].y + rects[i].h) {
                        switch (i) {
                            case 0: if (showMarathon(renderer) == 1) continue; break;
                            case 1: if (showClassique(renderer) == 1) continue; break;
                            case 2: if (showDuel(renderer) == 1) continue; break;
                            case 3: return 0;
                        }
                    }
                }
            }
        }

        // Affichage
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTex, NULL, &bgRect);
        SDL_RenderCopy(renderer, logoTex, NULL, &logoRect);
        for (int i = 0; i < 4; i++)
            SDL_RenderCopy(renderer, textures[i], NULL, &rects[i]);
        SDL_RenderCopy(renderer, soundActive ? soundOnTex : soundOffTex, NULL, &soundRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Nettoyage
    SDL_DestroyTexture(bgTex);
    SDL_DestroyTexture(logoTex);
    for (int i = 0; i < 4; i++) SDL_DestroyTexture(textures[i]);
    SDL_DestroyTexture(soundOnTex);
    SDL_DestroyTexture(soundOffTex);

    return 1;
}
