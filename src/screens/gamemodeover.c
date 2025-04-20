#include "gamemodeover.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "../ui/text.h"
#include "../ui/textures.h"
#include "../ui/ressources.h" 

int showGameOver(SDL_Renderer* renderer, int score, int lines, int elapsedSeconds) {
    TTF_Font* fontBig = loadFont(FONT_PATH, 64);
    TTF_Font* fontSmall = loadFont(FONT_PATH, 28);
    if (!fontBig || !fontSmall) return 1;

    SDL_Texture* bgTex = loadTexture(renderer, BG_GAMEOVER);
    SDL_Texture* btnRetry = loadTexture(renderer, BTN_RECOMMENCER);
    SDL_Texture* btnMenu = loadTexture(renderer, BTN_MENU); 

    SDL_Color white = {255, 255, 255, 255};

    char scoreText[64];
    snprintf(scoreText, sizeof(scoreText), "%d points", score);
    int wScore, hScore;
    SDL_Texture* scoreTex = renderText(renderer, fontSmall, scoreText, white, &wScore, &hScore);

    char linesText[64];
    snprintf(linesText, sizeof(linesText), "%d lignes", lines);
    int wLines, hLines;
    SDL_Texture* linesTex = renderText(renderer, fontSmall, linesText, white, &wLines, &hLines);

    char timeText[64];
    snprintf(timeText, sizeof(timeText), "%d min, %02d s", elapsedSeconds / 60, elapsedSeconds % 60);
    int wTime, hTime;
    SDL_Texture* timeTex = renderText(renderer, fontSmall, timeText, white, &wTime, &hTime);

    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    SDL_Rect bgRect = {winW / 2 - 250, winH / 2 - 200, 500, 400};
    SDL_Rect retryRect = {bgRect.x + 50, bgRect.y + 270, 170, 50};
    SDL_Rect menuRect = {bgRect.x + 280, bgRect.y + 270, 170, 50};

    SDL_Rect scoreRect = {bgRect.x + (bgRect.w - wScore) / 2, bgRect.y + 100, wScore, hScore};
    SDL_Rect linesRect = {bgRect.x + (bgRect.w - wLines) / 2, scoreRect.y + hScore + 10, wLines, hLines};
    SDL_Rect timeRect = {bgRect.x + (bgRect.w - wTime) / 2, linesRect.y + hLines + 10, wTime, hTime};

    SDL_Event e;
    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                return 0;
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x, my = e.button.y;
                if (mx >= retryRect.x && mx <= retryRect.x + retryRect.w &&
                    my >= retryRect.y && my <= retryRect.y + retryRect.h)
                    return 1;
                if (mx >= menuRect.x && mx <= menuRect.x + menuRect.w &&
                    my >= menuRect.y && my <= menuRect.y + menuRect.h)
                    return 0;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTex, NULL, &bgRect);
        SDL_RenderCopy(renderer, btnRetry, NULL, &retryRect);
        SDL_RenderCopy(renderer, btnMenu, NULL, &menuRect);
        SDL_RenderCopy(renderer, scoreTex, NULL, &scoreRect);
        SDL_RenderCopy(renderer, linesTex, NULL, &linesRect);
        SDL_RenderCopy(renderer, timeTex, NULL, &timeRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    return 0;
}