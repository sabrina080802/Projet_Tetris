#include "duel_gameover.h"

int showDuelGameOver(SDL_Renderer *r, int score1, int lines1, int score2, int lines2)
{
    int winW, winH;
    SDL_GetRendererOutputSize(r, &winW, &winH);

    SDL_Surface *shot = SDL_CreateRGBSurfaceWithFormat(
        0, winW, winH, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_RenderReadPixels(r, NULL, SDL_PIXELFORMAT_RGBA32,
                         shot->pixels, shot->pitch);
    SDL_Texture *snap = SDL_CreateTextureFromSurface(r, shot);
    SDL_FreeSurface(shot);

    SDL_Texture *bg = loadTexture(r, BG_GAMEOVER);
    SDL_Texture *btnRetry = loadTexture(r, BTN_RECOMMENCER);
    SDL_Texture *btnMenu = loadTexture(r, BTN_MENU);
    TTF_Font *font = loadFont(FONT_PATH, 28);

    if (!snap || !bg || !btnRetry || !btnMenu || !font)
        return 1;

    SDL_SetTextureBlendMode(bg, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(snap, SDL_BLENDMODE_BLEND);

    SDL_Color white = {255, 255, 255, 255};

    /* texte gagnant / scores */
    const char *winner =
        (score1 > score2)   ? "Joueur 1 gagne !"
        : (score2 > score1) ? "Joueur 2 gagne !"
                            : "Égalité !";

    char p1Txt[64], p2Txt[64];
    snprintf(p1Txt, sizeof p1Txt, "P1  %6d pts  %4d lignes", score1, lines1);
    snprintf(p2Txt, sizeof p2Txt, "P2  %6d pts  %4d lignes", score2, lines2);

    int wWin, hWin, wP1, hP1, wP2, hP2;
    SDL_Texture *tWin = renderText(r, font, winner, white, &wWin, &hWin);
    SDL_Texture *tP1 = renderText(r, font, p1Txt, white, &wP1, &hP1);
    SDL_Texture *tP2 = renderText(r, font, p2Txt, white, &wP2, &hP2);

    /* positionnement */
    SDL_Rect full = {0, 0, winW, winH};
    int cx = winW / 2;
    int top = winH / 2 - 90;

    SDL_Rect rWin = {cx - wWin / 2, top, wWin, hWin};
    SDL_Rect rP1 = {cx - wP1 / 2, top + 50, wP1, hP1};
    SDL_Rect rP2 = {cx - wP2 / 2, top + 90, wP2, hP2};

    SDL_Rect retryR = {cx - 200, top + 160, 170, 50};
    SDL_Rect menuR = {cx + 30, top + 160, 170, 50};

    SDL_Event e;
    while (1)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                return 0;

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x, my = e.button.y;

                if (mx >= retryR.x && mx <= retryR.x + retryR.w &&
                    my >= retryR.y && my <= retryR.y + retryR.h)
                    return 1; /* Recommencer */

                if (mx >= menuR.x && mx <= menuR.x + menuR.w &&
                    my >= menuR.y && my <= menuR.y + menuR.h)
                    return 0; /* Quitter vers menu */
            }
        }

        SDL_RenderCopy(r, snap, NULL, &full);
        SDL_SetTextureAlphaMod(bg, 200);
        SDL_RenderCopy(r, bg, NULL, &full);

        SDL_RenderCopy(r, tWin, NULL, &rWin);
        SDL_RenderCopy(r, tP1, NULL, &rP1);
        SDL_RenderCopy(r, tP2, NULL, &rP2);

        SDL_RenderCopy(r, btnRetry, NULL, &retryR);
        SDL_RenderCopy(r, btnMenu, NULL, &menuR);

        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
}
