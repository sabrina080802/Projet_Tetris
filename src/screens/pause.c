#include "pause.h"
#include "../ui/ressources.h"
#include "../ui/textures.h"

int showPause(SDL_Renderer* renderer) {
    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    SDL_Surface* screen = SDL_CreateRGBSurfaceWithFormat(
        0, winW, winH, 32, SDL_PIXELFORMAT_RGBA32
    );
    SDL_RenderReadPixels(renderer, NULL,
                         SDL_PIXELFORMAT_RGBA32,
                         screen->pixels, screen->pitch);
    SDL_Texture* snapTex = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_FreeSurface(screen);

    SDL_Texture* pauseBg       = loadTexture(renderer, BG_PAUSE);
    SDL_Texture* btnContinuer  = loadTexture(renderer, BTN_CONTINUER);
    SDL_Texture* btnQuitter    = loadTexture(renderer, BTN_QUITTER);
    if (!snapTex || !pauseBg || !btnContinuer || !btnQuitter ) {
        SDL_DestroyTexture(snapTex);
        return 1;
    }
    SDL_Rect fullRect      = {0, 0, winW, winH};
    SDL_GetRendererOutputSize(renderer, &winW, &winH);    
    int btnW = 200,  btnH = 70;
    int btnX = (winW - btnW) / 2;
    int yContinueOffset = 40;  
    int yQuitOffset     = 10;  

    SDL_Rect continuerRect = {
        btnX,
        winH/2 - btnH - 20 + yContinueOffset,
        btnW,
        btnH
    };

    SDL_Rect quitterRect = {
        btnX,
        winH/2 + 20 + yQuitOffset,
        btnW,
        btnH
    };

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(pauseBg,      SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(btnContinuer, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(btnQuitter,   SDL_BLENDMODE_BLEND);


    SDL_Event e;
    while (1) {
        if (!SDL_WaitEvent(&e)) break;
        if (e.type == SDL_QUIT) { break; }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx = e.button.x, my = e.button.y;
            if (mx >= continuerRect.x && mx <= continuerRect.x + continuerRect.w &&
                my >= continuerRect.y && my <= continuerRect.y + continuerRect.h) {
                break;  
            }
            if (mx >= quitterRect.x && mx <= quitterRect.x + quitterRect.w &&
                my >= quitterRect.y && my <= quitterRect.y + quitterRect.h) {
                SDL_DestroyTexture(snapTex);
                SDL_DestroyTexture(pauseBg);
                return 1;  
            }
        }

        SDL_RenderCopy(renderer, snapTex, NULL, &fullRect);
        SDL_RenderCopy(renderer, pauseBg, NULL, &fullRect);
        SDL_RenderCopy(renderer, btnContinuer, NULL, &continuerRect);
        SDL_RenderCopy(renderer, btnQuitter,   NULL, &quitterRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(snapTex);
    SDL_DestroyTexture(pauseBg);
    SDL_DestroyTexture(btnContinuer);
    SDL_DestroyTexture(btnQuitter);

    return 0;  
}
