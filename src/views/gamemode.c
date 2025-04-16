#include "gamemode.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include "gamemode_internal.h"
#include "../logic/grid.h"


#define GRID_COLS 10
#define GRID_ROWS 20

int grid[GRID_ROWS][GRID_COLS] = {0}; 

// Couleurs
static const SDL_Color VIOLET_FONCE = {37, 17, 66, 127};  // #251142
static const SDL_Color VIOLET_BORD  = {137, 64, 247, 255}; // #8940f7
static const SDL_Color WHITE        = {255, 255, 255, 255};

// Charge une texture depuis une image
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
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

// Génère une texture à partir d’un texte
SDL_Texture* renderText(SDL_Renderer* renderer, TTF_Font* font,
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

void showGameMode(SDL_Renderer* renderer, GameModeInfo modeInfo, Tetromino* current, SDL_Texture* blockTextures[]) {
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
    SDL_Texture* modeTitleTex = renderText(renderer, fontBig, modeInfo.modeName, WHITE, &wTitle, &hTitle);

    int wBest, hBest;
    SDL_Texture* bestScoreTex = renderText(renderer, fontSmall, modeInfo.bestScore, WHITE, &wBest, &hBest);

    int wStats, hStats;
    SDL_Texture* statsTex = renderText(renderer, fontSmall, "Statistiques", WHITE, &wStats, &hStats);

    int wScore, hScore;
    SDL_Texture* scoreTex = renderText(renderer, fontSmall, modeInfo.scoreText, WHITE, &wScore, &hScore);

    int wLines, hLines;
    SDL_Texture* linesTex = renderText(renderer, fontSmall, modeInfo.linesText, WHITE, &wLines, &hLines);

    int wNext, hNext;
    SDL_Texture* nextTex = renderText(renderer, fontSmall, modeInfo.nextText, WHITE, &wNext, &hNext);

    SDL_Rect bgRect = {0, 0, winW, winH};

    float gridHeight = winH - 100;
    float gridWidth = 400 * gridHeight / 800;
    float gridX = (winW - gridWidth) / 2;
    float gridY = (winH - gridHeight) / 2;

    SDL_Rect wellRect = {gridX, gridY, gridWidth, gridHeight};

    SDL_Rect titleRect = {100, 50, wTitle, hTitle};
    SDL_Rect bestScoreRect = {titleRect.x, titleRect.y + hTitle + 10, wBest, hBest};

    float statsWidth = 300, statsHeight = 600;
    SDL_Rect statsRect = {
        gridX - statsWidth - 20,
        gridY + gridHeight - statsHeight,
        statsWidth, statsHeight
    };
    SDL_Rect statsLabelRect = {
        statsRect.x + (statsRect.w - wStats) / 2,
        statsRect.y - (hStats + 10),
        wStats, hStats
    };

    int marginRight = 20, espace = 10;
    SDL_Rect scoreRect = {wellRect.x + wellRect.w + marginRight, wellRect.y, 250, 100};
    SDL_Rect scoreTextRect = {scoreRect.x + 10, scoreRect.y + 10, wScore, hScore};

    SDL_Rect linesRect = {scoreRect.x, scoreRect.y + scoreRect.h + 20, 250, 100};
    SDL_Rect linesTextRect = {linesRect.x + 10, linesRect.y + 10 + espace, wLines, hLines};

    SDL_Rect nextRect = {scoreRect.x, linesRect.y + linesRect.h + 20, 250, 200};
    SDL_Rect nextTextRect = {nextRect.x + 10, nextRect.y + 10, wNext, hNext};

    int blockSizeW = wellRect.w / GRID_COLS;
    int blockSizeH = wellRect.h / GRID_ROWS;
    int blockSize = blockSizeW < blockSizeH ? blockSizeW : blockSizeH;
    int gridRows = gridHeight / blockSize;
    wellRect.w = blockSize * GRID_COLS;
    wellRect.h = blockSize * GRID_ROWS;
    wellRect.x = (winW - wellRect.w) / 2;
    wellRect.y = (winH - wellRect.h) / 2;


    Uint32 lastFallTime = SDL_GetTicks();
    Uint32 fallDelay = 500;


    SDL_Rect musicRect = {winW - 70, 20, 80, 80};
    int musicActive = 1;

    int running = 1;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
                running = 0;

            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x, my = e.button.y;
                if (mx >= musicRect.x && mx <= musicRect.x + musicRect.w &&
                    my >= musicRect.y && my <= musicRect.y + musicRect.h)
                    musicActive = !musicActive;
            }

           else if (e.type == SDL_KEYDOWN) {
            Tetromino test = *current;

            switch (e.key.keysym.sym) {

                case SDLK_LEFT:
                    test.x--;
                    if (!collides(&test, grid)) current->x = test.x;
                    break;

                case SDLK_RIGHT:
                    test.x++;
                    if (!collides(&test, grid)) current->x = test.x;
                    break;

                case SDLK_DOWN:
                    test.y++;
                    if (!collides(&test, grid)) current->y = test.y;
                    break;

                case SDLK_z:
                case SDLK_UP:
                    rotateTetromino(&test);
                    if (!collides(&test, grid)) *current = test;
                    break;

                case SDLK_RETURN:
                    *current = createTetromino(rand() % 7);
                    current->y = 0;
                    break;
            }
        }
    }   

       Uint32 now = SDL_GetTicks();
        if (now - lastFallTime > fallDelay) {
            Tetromino test = *current;
            test.y++;

            if (!collides(&test, grid)) {
                current->y++;
            } else {
                // FIXER LA PIECE DANS LA GRILLE
                for (int row = 0; row < 4; row++) {
                    for (int col = 0; col < 4; col++) {
                        if (current->shape[row][col]) {
                            int gx = current->x + col;
                            int gy = current->y + row;
                            if (gx >= 0 && gx < GRID_COLS && gy >= 0 && gy < GRID_ROWS) {
                                grid[gy][gx] = current->type + 1;
                            }
                        }
                    }
                }

                // NOUVELLE PIECE
                *current = createTetromino(rand() % 7);

                if (collides(current, grid)) {
                    running = 0; // game over
                }
            }

            lastFallTime = now;
        }


        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, bgTex, NULL, &bgRect);
        drawGrid(renderer, blockTextures, blockSize, wellRect.x, wellRect.y); 
        drawTetromino(renderer, current, blockTextures, blockSize, wellRect.x, wellRect.y);

        int radius = 25;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // Backgrounds
        roundedBoxRGBA(renderer, wellRect.x, wellRect.y, wellRect.x + wellRect.w, wellRect.y + wellRect.h, radius, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);
        roundedBoxRGBA(renderer, statsRect.x, statsRect.y, statsRect.x + statsRect.w, statsRect.y + statsRect.h, radius, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);
        roundedBoxRGBA(renderer, scoreRect.x, scoreRect.y, scoreRect.x + scoreRect.w, scoreRect.y + scoreRect.h, radius, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);
        roundedBoxRGBA(renderer, linesRect.x, linesRect.y, linesRect.x + linesRect.w, linesRect.y + linesRect.h, radius, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);
        roundedBoxRGBA(renderer, nextRect.x, nextRect.y, nextRect.x + nextRect.w, nextRect.y + nextRect.h, radius, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);

        // Bords
        roundedRectangleRGBA(renderer, wellRect.x, wellRect.y, wellRect.x + wellRect.w, wellRect.y + wellRect.h, radius, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);
        roundedRectangleRGBA(renderer, statsRect.x, statsRect.y, statsRect.x + statsRect.w, statsRect.y + statsRect.h, radius, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);
        roundedRectangleRGBA(renderer, scoreRect.x, scoreRect.y, scoreRect.x + scoreRect.w, scoreRect.y + scoreRect.h, radius, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);
        roundedRectangleRGBA(renderer, linesRect.x, linesRect.y, linesRect.x + linesRect.w, linesRect.y + linesRect.h, radius, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);
        roundedRectangleRGBA(renderer, nextRect.x, nextRect.y, nextRect.x + nextRect.w, nextRect.y + nextRect.h, radius, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);

        // Textes
        if (modeTitleTex) SDL_RenderCopy(renderer, modeTitleTex, NULL, &titleRect);
        if (bestScoreTex) SDL_RenderCopy(renderer, bestScoreTex, NULL, &bestScoreRect);
        if (statsTex)     SDL_RenderCopy(renderer, statsTex, NULL, &statsLabelRect);
        if (scoreTex)     SDL_RenderCopy(renderer, scoreTex, NULL, &scoreTextRect);
        if (linesTex)     SDL_RenderCopy(renderer, linesTex, NULL, &linesTextRect);
        if (nextTex)      SDL_RenderCopy(renderer, nextTex, NULL, &nextTextRect);

        SDL_RenderCopy(renderer, musicActive ? musicOnTex : musicOffTex, NULL, &musicRect);     

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Nettoyage
    SDL_DestroyTexture(modeTitleTex);
    SDL_DestroyTexture(bestScoreTex);
    SDL_DestroyTexture(statsTex);
    SDL_DestroyTexture(scoreTex);
    SDL_DestroyTexture(linesTex);
    SDL_DestroyTexture(nextTex);
    SDL_DestroyTexture(bgTex);
    SDL_DestroyTexture(musicOnTex);
    SDL_DestroyTexture(musicOffTex);

    TTF_CloseFont(fontBig);
    TTF_CloseFont(fontSmall);
    TTF_Quit();
}
