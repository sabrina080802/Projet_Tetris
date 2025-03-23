#include "marathon.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>

static const SDL_Color VIOLET_FONCE = {37, 17, 66, 127};  // #251142
static const SDL_Color VIOLET_BORD  = {137, 64, 247, 255}; // #8940f7
static const SDL_Color WHITE        = {255, 255, 255, 255};

static SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        fprintf(stderr, "Erreur chargement %s : %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    if (!tex) {
        fprintf(stderr, "Erreur création texture %s : %s\n", path, SDL_GetError());
    }
    return tex;
}
static SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font,
                               const char* text, SDL_Color color, int* w, int* h) {
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text, color);
    if (!surf) {
        fprintf(stderr, "Erreur SDL_ttf : %s\n", TTF_GetError());
        return NULL;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    *w = surf->w;
    *h = surf->h;
    SDL_FreeSurface(surf);
    if (!tex) {
        fprintf(stderr, "Erreur texture texte : %s\n", SDL_GetError());
    }
    return tex;
}

void showMarathon(SDL_Renderer* renderer) {

    if (TTF_Init() != 0) {
        fprintf(stderr, "Erreur TTF_Init : %s\n", TTF_GetError());
        return;
    }

    TTF_Font* fontBig = TTF_OpenFont("ressources/fonts/Iceland-Regular.ttf", 48);
    TTF_Font* fontSmall = TTF_OpenFont("ressources/fonts/Iceland-Regular.ttf", 28);
    if (!fontBig || !fontSmall) {
        fprintf(stderr, "Erreur chargement police : %s\n", TTF_GetError());
        TTF_Quit();
        return;
    }
    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    SDL_Texture* bgTex = loadTexture(renderer, "ressources/ingame-bg.png");
    if (!bgTex) {
        TTF_CloseFont(fontBig);
        TTF_CloseFont(fontSmall);
        TTF_Quit();
        return;
    }

    SDL_Texture* musicOnTex  = loadTexture(renderer, "ressources/bobActif.png");
    SDL_Texture* musicOffTex = loadTexture(renderer, "ressources/bobInactif.png");

    int wTitle, hTitle;
    SDL_Texture* marathonTitle = renderText(renderer, fontBig, "Marathon", WHITE, &wTitle, &hTitle);

    int wBest, hBest;
    SDL_Texture* bestScoreText = renderText(renderer, fontSmall,
        "Meilleur score\r1465 lignes\r52 min, 34s\r725646 pts", WHITE, &wBest, &hBest);

    int wStats, hStats;
    SDL_Texture* statsText = renderText(renderer, fontSmall, "Statistiques", WHITE, &wStats, &hStats);

    int wScore, hScore;
    SDL_Texture* scoreText = renderText(renderer, fontSmall, "Score\n001424", WHITE, &wScore, &hScore);

    int wLines, hLines;
    SDL_Texture* linesText = renderText(renderer, fontSmall, "Nbre lignes\n00001", WHITE, &wLines, &hLines);

    int wNext, hNext;
    SDL_Texture* nextText = renderText(renderer, fontSmall, "Suivant", WHITE, &wNext, &hNext);

    // ----------------------------------------------------------
    // POSITIONNEMENTS ET DIMENSIONS
    // ----------------------------------------------------------

    // Fond
    SDL_Rect bgRect = {0, 0, winW, winH};

    // Puits plus grand (600×800)
    float gridHeight = winH - 100;
    float gridWidth = 400 * gridHeight / 800;
    float gridX =  (winW - gridWidth)/2;
    float gridY = (winH - gridHeight)/2;
    int espace = 10; 


    SDL_Rect wellRect = {
        gridX,     // X
        gridY,     // Y
        gridWidth,                // largeur
        gridHeight                 // hauteur
    };
    SDL_Rect marathonTitleRect = {
        100,   // x
        50,    // y
        wTitle,
        hTitle
    };
    
    SDL_Rect bestScoreRect = {
        marathonTitleRect.x,
        marathonTitleRect.y + hTitle + 10,
        wBest, hBest
    };
    
    float statsWidth = 300;
    float statsHeight = 600;
    SDL_Rect statsRect = {
        gridX - statsWidth - 20,                     // x
        gridY + gridHeight - statsHeight,         // y
        statsWidth,                    // w
        statsHeight                     // h
    };
    SDL_Rect statsLabelRect = {
        statsRect.x + (statsRect.w - wStats)/2,
        statsRect.y - (hStats + 10),
        wStats, hStats
    };

    int marginRight = 20;

   SDL_Rect scoreRect = {
    wellRect.x + wellRect.w + marginRight, // X positionné à droite de wellRect
    wellRect.y,                            // Y aligné sur le haut de wellRect (à ajuster si besoin)
    250, 100
};
SDL_Rect scoreTextRect = {
    scoreRect.x + 10, scoreRect.y + 10,
    wScore, hScore
};

SDL_Rect linesRect = {
    scoreRect.x,                           // Même X que scoreRect
    scoreRect.y + scoreRect.h + 20,          // Y en dessous de scoreRect avec un espacement de 20 pixels
    250, 100
};
SDL_Rect linesTextRect = {
    linesRect.x + 10,     linesRect.y + 10 + espace, // ajout d'un espace vertical
    wLines, hLines
};
  SDL_Rect nextRect = {
    scoreRect.x,                           // Même X que scoreRect
    linesRect.y + linesRect.h + 20,         // placé 20 pixels sous linesRect
    250, 200
};
SDL_Rect nextTextRect = {
    nextRect.x + 10, nextRect.y + 10, // Ajustez ce calcul si vous souhaitez centrer ou repositionner le texte
    wNext, hNext
};
    SDL_Rect musicRect = {
        winW - 70,
        20,
        80,
        80
    };
    int musicActive = 1;
    
    int running = 1;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
            running = 0;
        else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            running = 0;
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx = e.button.x;
            int my = e.button.y;
            // Clique sur l'icône musique
            if (mx >= musicRect.x && mx <= musicRect.x + musicRect.w &&
                my >= musicRect.y && my <= musicRect.y + musicRect.h) {
                    musicActive = !musicActive;
                }
            }
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTex, NULL, &bgRect);
        int radius = 25;

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);
        roundedBoxRGBA(renderer, wellRect.x, wellRect.y, wellRect.w + wellRect.x, wellRect.h + wellRect.y, radius, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a); // Rouge
        roundedBoxRGBA(renderer, statsRect.x, statsRect.y, statsRect.x + statsRect.w, statsRect.y + statsRect.h,radius, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);  
        roundedBoxRGBA(renderer, linesRect.x, linesRect.y,linesRect.x + linesRect.w, linesRect.y + linesRect.h,radius, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);      
        roundedBoxRGBA(renderer, nextRect.x, nextRect.y,nextRect.x + nextRect.w, nextRect.y + nextRect.h,radius,VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);        
        roundedBoxRGBA(renderer, scoreRect.x, scoreRect.y,scoreRect.x + scoreRect.w, scoreRect.y + scoreRect.h, radius, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);

        SDL_SetRenderDrawColor(renderer, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);
        roundedRectangleRGBA(renderer, wellRect.x, wellRect.y, wellRect.w + wellRect.x, wellRect.h + wellRect.y, radius, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a); // Rouge
        roundedRectangleRGBA(renderer, statsRect.x, statsRect.y,statsRect.x + statsRect.w, statsRect.y + statsRect.h,radius,VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);        
        roundedRectangleRGBA(renderer, linesRect.x, linesRect.y, linesRect.x + linesRect.w, linesRect.y + linesRect.h, radius, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);
        roundedRectangleRGBA(renderer, nextRect.x, nextRect.y,nextRect.x + nextRect.w, nextRect.y + nextRect.h, radius, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);
        roundedRectangleRGBA(renderer, scoreRect.x, scoreRect.y, scoreRect.x + scoreRect.w, scoreRect.y + scoreRect.h,radius,VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);    

        if (marathonTitle) SDL_RenderCopy(renderer, marathonTitle, NULL, &marathonTitleRect);
        if (bestScoreText) SDL_RenderCopy(renderer, bestScoreText, NULL, &bestScoreRect);
        if (statsText)     SDL_RenderCopy(renderer, statsText, NULL, &statsLabelRect);
        if (scoreText)     SDL_RenderCopy(renderer, scoreText, NULL, &scoreTextRect);
        if (linesText)     SDL_RenderCopy(renderer, linesText, NULL, &linesTextRect);
        if (nextText)      SDL_RenderCopy(renderer, nextText, NULL, &nextTextRect);
    
        if (musicOnTex && musicOffTex) {
            if (musicActive) {
                SDL_RenderCopy(renderer, musicOnTex, NULL, &musicRect);
            } else {
                SDL_RenderCopy(renderer, musicOffTex, NULL, &musicRect);
            }
        }
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    if (marathonTitle) SDL_DestroyTexture(marathonTitle);
    if (bestScoreText) SDL_DestroyTexture(bestScoreText);
    if (statsText)     SDL_DestroyTexture(statsText);
    if (scoreText)     SDL_DestroyTexture(scoreText);
    if (linesText)     SDL_DestroyTexture(linesText);
    if (nextText)      SDL_DestroyTexture(nextText);

    TTF_CloseFont(fontBig);
    TTF_CloseFont(fontSmall);
    TTF_Quit();
}