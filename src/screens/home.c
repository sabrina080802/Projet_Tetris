#include "home.h"
#include "../modes/marathon.h"
#include "../modes/duel.h"
#include "../ui/textures.h"
#include "../ui/ressources.h"
#include <SDL2/SDL.h>

int showHome(SDL_Renderer* renderer) {
    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    SDL_Texture* bgTex       = loadTexture(renderer, HOME_BG);
    SDL_Texture* logoTex     = loadTexture(renderer, HOME_LOGO);
    SDL_Texture* buttonTex[3] = {
        loadTexture(renderer, HOME_BTN_MARATHON),
        loadTexture(renderer, HOME_BTN_DUEL),
        loadTexture(renderer, QUITTER)
    };
    SDL_Texture* soundOnTex  = loadTexture(renderer, ICON_MUSIC_ON);
    SDL_Texture* soundOffTex = loadTexture(renderer, ICON_MUSIC_OFF);

    if (!bgTex || !logoTex || !buttonTex[0] || !buttonTex[1] || !buttonTex[2] || !soundOnTex || !soundOffTex)
        return 0;

    SDL_Rect bgRect = {0, 0, winW, winH};
    int logoW, logoH;
    SDL_QueryTexture(logoTex, NULL, NULL, &logoW, &logoH);
    SDL_Rect logoRect = {(winW - logoW) / 2, 80, logoW, logoH};

    SDL_Rect btnRect[3];
    int spacing = 40, totalH = 0;
    for (int i = 0; i < 3; i++) {
        SDL_QueryTexture(buttonTex[i], NULL, NULL, &btnRect[i].w, &btnRect[i].h);
        totalH += btnRect[i].h;
        if (i < 2) totalH += spacing;
    }
    int startY = (winH - totalH) / 2 + 40;
    for (int i = 0; i < 3; i++) {
        btnRect[i].x = (winW - btnRect[i].w) / 2;
        btnRect[i].y = startY;
        startY += btnRect[i].h + spacing;
    }

    SDL_Rect soundRect;
    SDL_QueryTexture(soundOnTex, NULL, NULL, &soundRect.w, &soundRect.h);
    soundRect.x = winW - soundRect.w - 20;
    soundRect.y = 20;

    int running = 1;
    int soundActive = 1;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT ||
                (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
                return 0;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x, my = e.button.y;
                // Toggle son
                if (mx >= soundRect.x && mx <= soundRect.x + soundRect.w &&
                    my >= soundRect.y && my <= soundRect.y + soundRect.h) {
                    soundActive = !soundActive;
                }
                // Choix mode
                for (int i = 0; i < 3; i++) {
                    if (mx >= btnRect[i].x && mx <= btnRect[i].x + btnRect[i].w &&
                        my >= btnRect[i].y && my <= btnRect[i].y + btnRect[i].h) {
                        switch (i) {
                            case 0:
                                showMarathon(renderer);
                                break;
                            case 1:
                                showDuel(renderer);
                                break;
                            case 2:
                                return 0;
                        }
                    }
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTex,    NULL, &bgRect);
        SDL_RenderCopy(renderer, logoTex,  NULL, &logoRect);
        for (int i = 0; i < 3; i++)
            SDL_RenderCopy(renderer, buttonTex[i], NULL, &btnRect[i]);
        SDL_RenderCopy(renderer, soundActive ? soundOnTex : soundOffTex,
                       NULL, &soundRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(bgTex);
    SDL_DestroyTexture(logoTex);
    for (int i = 0; i < 3; i++) SDL_DestroyTexture(buttonTex[i]);
    SDL_DestroyTexture(soundOnTex);
    SDL_DestroyTexture(soundOffTex);
    return 1;
}
