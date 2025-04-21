#include "gamemodeover.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "../ui/text.h"
#include "../ui/textures.h"
#include "../ui/ressources.h"


int showGameOver(SDL_Renderer* renderer, int score, int lines, int elapsedSeconds) {
    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    SDL_Surface* screen = SDL_CreateRGBSurfaceWithFormat(0, winW, winH, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, screen->pixels, screen->pitch);
    SDL_Texture* snapTex = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_FreeSurface(screen);

    SDL_Texture* bgTex    = loadTexture(renderer, BG_GAMEOVER);  
    SDL_Texture* btnRetry = loadTexture(renderer, BTN_RECOMMENCER);
    SDL_Texture* btnMenu  = loadTexture(renderer, BTN_MENU);
    TTF_Font* fontSmall = loadFont(FONT_PATH, 28);

    if (!bgTex || !btnRetry || !btnMenu || !fontSmall || !snapTex) return 1;

    SDL_SetTextureBlendMode(bgTex, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(snapTex, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Color white = {255, 255, 255, 255};

    char scoreText[64], linesText[64], timeText[64];
    snprintf(scoreText, sizeof(scoreText), "%d points", score);
    snprintf(linesText, sizeof(linesText), "%d lignes", lines);
    snprintf(timeText,  sizeof(timeText), "%d min, %02d s", elapsedSeconds / 60, elapsedSeconds % 60);

    int wScore, hScore, wLines, hLines, wTime, hTime;
    SDL_Texture* scoreTex = renderText(renderer, fontSmall, scoreText, white, &wScore, &hScore);
    SDL_Texture* linesTex = renderText(renderer, fontSmall, linesText, white, &wLines, &hLines);
    SDL_Texture* timeTex  = renderText(renderer, fontSmall, timeText,  white, &wTime,  &hTime);

    SDL_Rect fullRect     = {0, 0, winW, winH}; 
    int centerX           = winW / 2;
    int yBase             = winH / 2 - 50;

    SDL_Rect retryRect = {centerX - 200, yBase + 130, 170, 50};
    SDL_Rect menuRect  = {centerX + 30,  yBase + 130, 170, 50};

    SDL_Rect scoreRect = {centerX - wScore / 2, yBase - 30, wScore, hScore};
    SDL_Rect linesRect = {centerX - wLines / 2, yBase + 10, wLines, hLines};
    SDL_Rect timeRect  = {centerX - wTime  / 2, yBase + 50, wTime,  hTime};

    SDL_Event e;
    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) return 0;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x, my = e.button.y;
                if (mx >= retryRect.x && mx <= retryRect.x + retryRect.w &&
                    my >= retryRect.y && my <= retryRect.y + retryRect.h)
                    return 1;
                if (mx >= menuRect.x && mx <= menuRect.x + menuRect.w &&
                    my >= menuRect.y && my <= menuRect.y + menuRect.h)
                    return 0;
            }
        }

        SDL_RenderCopy(renderer, snapTex, NULL, &fullRect);  
        SDL_RenderCopy(renderer, bgTex, NULL, &fullRect);   
        SDL_RenderCopy(renderer, btnRetry, NULL, &retryRect);
        SDL_RenderCopy(renderer, btnMenu,  NULL, &menuRect);
        SDL_RenderCopy(renderer, scoreTex, NULL, &scoreRect);
        SDL_RenderCopy(renderer, linesTex, NULL, &linesRect);
        SDL_RenderCopy(renderer, timeTex,  NULL, &timeRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(scoreTex);
    SDL_DestroyTexture(linesTex);
    SDL_DestroyTexture(timeTex);
    SDL_DestroyTexture(btnRetry);
    SDL_DestroyTexture(btnMenu);
    SDL_DestroyTexture(bgTex);
    SDL_DestroyTexture(snapTex);
    TTF_CloseFont(fontSmall);

    return 0;
}
