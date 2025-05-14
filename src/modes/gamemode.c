#include "gamemode.h"
#include "../logic/grid.h"
#include "../logic/pieces.h"
#include "../ui/ressources.h"
#include "../ui/textures.h"
#include "../ui/text.h"
#include "../ui/render.h"   
#include "../screens/pause.h"
#include "../logic/score.h" 
#include "../logic/timer.h" 
#include "../screens/gamemodeover.h"      
#include "../views/gamemode_internal.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#define GRID_COLS 10
#define GRID_ROWS 20

static const SDL_Color WHITE = {255, 255, 255, 255};
static const SDL_Color VIOLET_FONCE = {37, 17, 66, 127};
static const SDL_Color VIOLET_BORD = {137, 64, 247, 255};

int showGameMode(SDL_Renderer* renderer, GameModeInfo modeInfo, Tetromino* current, Tetromino* next, SDL_Texture* blockTextures[]){
     if (TTF_Init() != 0) return 0;
    TTF_Font* fontBig = loadFont(FONT_PATH, 48);
    TTF_Font* fontSmall = loadFont(FONT_PATH, 28);
    if (!fontBig || !fontSmall) return 0;

    resetScore();
    resetLinesCleared();
    resetLevel();
    resetTimer();

    char scoreTextBuffer[64];
    char lineTextBuffer[64];
    Uint32 startTime = SDL_GetTicks();

    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    SDL_Texture* bgTex = loadTexture(renderer, BG_INGAME);
    if (!bgTex) {
        TTF_CloseFont(fontBig);
        TTF_CloseFont(fontSmall);
        TTF_Quit();
        return 0;
    }

    SDL_Texture* musicOnTex = loadTexture(renderer, ICON_MUSIC_ON);
    SDL_Texture* musicOffTex = loadTexture(renderer, ICON_MUSIC_OFF);
    SDL_Texture* pauseTex = loadTexture(renderer, BTN_PAUSE);
    SDL_Texture* pauseBgTex = loadTexture(renderer, BG_PAUSE);
    SDL_Texture* btnContinuerTex = loadTexture(renderer, BTN_CONTINUER);
    SDL_Texture* btnQuitterTex = loadTexture(renderer, BTN_QUITTER);

    int wTitle = 0, hTitle = 0;
    int wBest, hBest, wStats, hStats, wScore, hScore, wLines, hLines, wNext, hNext;

    const char* bestScoreText = strlen(modeInfo.bestScore)    > 0 ? modeInfo.bestScore    : " ";
    const char* statsLabel    = "Statistiques";
    const char* scoreText     = strlen(modeInfo.scoreText)    > 0 ? modeInfo.scoreText    : " ";
    const char* linesText     = strlen(modeInfo.linesText)    > 0 ? modeInfo.linesText    : " ";
    const char* nextLabel = "Suivant";
    const char* titleText = strlen(modeInfo.modeName) > 0 ? modeInfo.modeName: " ";

    SDL_Texture* bestScoreTex = renderText(renderer, fontSmall, bestScoreText, WHITE, &wBest, &hBest);
    SDL_Texture* statsTex     = renderText(renderer, fontSmall, statsLabel,    WHITE, &wStats, &hStats);
    SDL_Texture* scoreTex     = renderText(renderer, fontSmall, scoreText,     WHITE, &wScore, &hScore);
    SDL_Texture* linesTex     = renderText(renderer, fontSmall, linesText,     WHITE, &wLines, &hLines);
    SDL_Texture* nextTex     = renderText(renderer, fontSmall, nextLabel,    WHITE,  &wNext, &hNext);
    SDL_Texture* titleTex = renderText(renderer, fontBig, titleText, WHITE, &wTitle, &hTitle);
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

    int blockSizeW = wellRect.w / GRID_COLS;
    int blockSizeH = wellRect.h / GRID_ROWS;
    int blockSize = blockSizeW < blockSizeH ? blockSizeW : blockSizeH;
    wellRect.w = blockSize * GRID_COLS;
    wellRect.h = blockSize * GRID_ROWS;
    wellRect.x = (winW - wellRect.w) / 2;
    wellRect.y = (winH - wellRect.h) / 2;

    Uint32 lastFallTime = SDL_GetTicks();
    Uint32 fallDelay = 500;

    SDL_Rect musicRect = {winW - 90, 20, 80, 80};       
    SDL_Rect pauseRect = {winW - 180, 20, 80, 80};  
    SDL_Rect continuerBtn = {0};
    SDL_Rect quitterBtn = {0};
    int running = 1, paused = 0, musicActive = 1;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                return 0;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mx = e.button.x, my = e.button.y;
                if (mx >= musicRect.x && mx <= musicRect.x + musicRect.w &&
                    my >= musicRect.y && my <= musicRect.y + musicRect.h) {
                    musicActive = !musicActive;
                }
                else if (mx >= pauseRect.x && mx <= pauseRect.x + pauseRect.w &&
                        my >= pauseRect.y && my <= pauseRect.y + pauseRect.h) {
                    paused = 1;
                }
            }

            if (paused) {
                int action = showPause(renderer); 
                if (action == 1) return 1;
                paused = 0;
                }
               if (e.type == SDL_KEYDOWN) {
                SDL_Keycode key = e.key.keysym.sym;
                if (key == SDLK_ESCAPE) {
                    return 0; 
                }
                Tetromino test = *current;
                switch (e.key.keysym.sym) {
                    case SDLK_LEFT:  test.x--; break;
                    case SDLK_RIGHT: test.x++; break;
                    case SDLK_DOWN:  test.y++; break;
                    case SDLK_z:
                    case SDLK_UP:    rotateTetromino(&test); break;
                }
                if (!collides(&test, grid)) *current = test;
            }
      }

       Uint32 now = SDL_GetTicks();
        if (now - lastFallTime > fallDelay) {
            Uint32 elapsed = 0;
            Uint32 currentElapsed = (now - startTime) / 1000;
            if ((int)currentElapsed != (int)elapsed){
                elapsed = currentElapsed;
                int minutes = elapsed / 60;
                int seconds = elapsed % 60;
                snprintf(modeInfo.bestScore, 64, "Temps :\r%02d min, %02d s", minutes, seconds);
            }
            Tetromino test = *current;
            test.y++;

            if (!collides(&test, grid)) {
                current->y++;
            } else {
                for (int row = 0; row < 4; row++) {
                    int cleared = clearCompleteLines(grid);
                    if (cleared > 0) {
                        addScore(cleared); 
                        updateTimerFromLevel(level);
                        fallDelay = getElapsedTime();
                        snprintf(scoreTextBuffer, sizeof(scoreTextBuffer), "Score\n%06d",   score);
                        snprintf(lineTextBuffer,  sizeof(lineTextBuffer),"Lignes\n%05d",  linesCleared);
                        SDL_DestroyTexture(scoreTex);
                        SDL_DestroyTexture(linesTex);
                        scoreTex = renderText(renderer, fontSmall, scoreTextBuffer, WHITE, &wScore, &hScore);
                        linesTex = renderText(renderer, fontSmall, lineTextBuffer, WHITE, &wLines, &hLines);
                    }

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
               *current = *next;
                pieceCount[current->type]++;
                *next = createTetromino(getNextPieceType());  
                if (collides(current, grid)) {
                    int result = showGameOver(renderer, score, linesCleared, (SDL_GetTicks() - startTime) / 1000);
                    if (result == 1) {
                        memset(grid, 0, sizeof(grid));
                        *current = createTetromino(getNextPieceType());
                        *next = createTetromino(getNextPieceType());
                        score = 0;
                        linesCleared = 0;
                        startTime = SDL_GetTicks();
                        continue; 
                    } else {
                        running = 0; 
                    }
                }
            }
            lastFallTime = now;
        }
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,bgTex, NULL, &bgRect);
        drawGrid(renderer, blockTextures, blockSize, wellRect.x, wellRect.y); 
        drawGhostPiece(renderer, current, grid, blockSize, wellRect.x, wellRect.y);
        drawTetromino(renderer, current, blockTextures, blockSize, wellRect.x, wellRect.y);
        drawPieceStats(renderer, blockTextures, blockSize, statsRect.x + 20, statsRect.y + 20, fontSmall);
        int radius = 25;
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        char timerText[64];
        int elapsed = (SDL_GetTicks() - startTime) / 1000;
        snprintf(timerText, sizeof(timerText), "Temps : %02d:%02d", elapsed / 60, elapsed % 60);

        SDL_Texture* timerTex = renderText(renderer, fontBig, timerText, WHITE, NULL, NULL);
        SDL_Rect timerRect = {0, 0, 0, 0};          
        SDL_QueryTexture(timerTex, NULL, NULL, &timerRect.w, &timerRect.h);
        timerRect.y = titleRect.y + (hTitle - timerRect.h) / 2;  
        timerRect.x = titleRect.x + titleRect.w + 20;        
        SDL_RenderCopy(renderer, timerTex, NULL, &timerRect);
        SDL_RenderCopy(renderer, titleTex,  NULL, &titleRect);
        SDL_DestroyTexture(timerTex);
        // Stats
        roundedBoxRGBA(renderer, statsRect.x, statsRect.y, statsRect.x + statsRect.w, statsRect.y + statsRect.h, 20, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);
        roundedRectangleRGBA(renderer, statsRect.x, statsRect.y, statsRect.x + statsRect.w, statsRect.y + statsRect.h, 20, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);
        drawPieceStats(renderer, blockTextures, blockSize, statsRect.x + 20, statsRect.y + 20, fontSmall);
        // Next
        roundedBoxRGBA(renderer, nextRect.x, nextRect.y, nextRect.x + nextRect.w, nextRect.y + nextRect.h, 20, VIOLET_FONCE.r, VIOLET_FONCE.g, VIOLET_FONCE.b, VIOLET_FONCE.a);
        roundedRectangleRGBA(renderer, nextRect.x, nextRect.y, nextRect.x + nextRect.w, nextRect.y + nextRect.h, 20, VIOLET_BORD.r, VIOLET_BORD.g, VIOLET_BORD.b, VIOLET_BORD.a);
        int nextBlockSize = blockSize / 2;
        int minX = 4, maxX = 0, minY = 4, maxY = 0;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (next->shape[row][col]) {
                    if (col < minX) minX = col;
                    if (col > maxX) maxX = col;
                    if (row < minY) minY = row;
                    if (row > maxY) maxY = row;
                }
            }
        }
        int pieceWidth = (maxX - minX + 1) * nextBlockSize;
        int pieceHeight = (maxY - minY + 1) * nextBlockSize;

        int offsetX = nextRect.x + (nextRect.w - pieceWidth) / 2 - minX * nextBlockSize;
        int offsetY = nextRect.y + (nextRect.h - pieceHeight) / 2 - minY * nextBlockSize;

        SDL_Rect nextLabelRect = {
        nextRect.x + 10,               
        nextRect.y - hNext + 35,        
        wNext, hNext
        };
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                if (next->shape[row][col]) {
                    SDL_Rect dest = {
                        offsetX + col * nextBlockSize,
                        offsetY + row * nextBlockSize,
                        nextBlockSize, nextBlockSize
                    };
                    SDL_RenderCopy(renderer, blockTextures[next->type], NULL, &dest);
                }
            }
        }

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
        SDL_RenderCopy(renderer, nextTex, NULL, &nextLabelRect);

        // Textes
        if (bestScoreTex) SDL_RenderCopy(renderer,bestScoreTex, NULL, &bestScoreRect);
        if (statsTex)     SDL_RenderCopy(renderer,statsTex, NULL, &statsLabelRect);
        if (scoreTex)     SDL_RenderCopy(renderer, scoreTex, NULL, &scoreTextRect);
        if (linesTex)     SDL_RenderCopy(renderer, linesTex, NULL, &linesTextRect);

        SDL_RenderCopy(renderer, musicActive ? musicOnTex : musicOffTex, NULL, &musicRect);     

        SDL_RenderCopy(renderer, pauseTex, NULL, &pauseRect);

        if (paused) {
        SDL_Rect pauseBgRect = {0, 0, winW, winH}; 
        SDL_RenderCopy(renderer, pauseBgTex, NULL, &pauseBgRect);

        continuerBtn = (SDL_Rect){winW / 2 - 160, winH / 2 + 20, 150, 50};
        quitterBtn   = (SDL_Rect){winW / 2 + 10,  winH / 2 + 20, 150, 50};

        SDL_RenderCopy(renderer, btnContinuerTex, NULL, &continuerBtn);
        SDL_RenderCopy(renderer, btnQuitterTex, NULL, &quitterBtn);
        SDL_RenderPresent(renderer);

        SDL_Event ev;
        int wait = 1;
        while (wait && SDL_WaitEvent(&ev)) {
            if (ev.type == SDL_QUIT) {
                running = 0;
                wait = 0;
            } else if (ev.type == SDL_MOUSEBUTTONDOWN) {
                int mx = ev.button.x;
                int my = ev.button.y;
               if (mx >= continuerBtn.x && mx <= continuerBtn.x + continuerBtn.w &&
                    my >= continuerBtn.y && my <= continuerBtn.y + continuerBtn.h) {
                    paused = 0;
                    wait = 0;
                } else if (mx >= quitterBtn.x && mx <= quitterBtn.x + quitterBtn.w &&
                        my >= quitterBtn.y && my <= quitterBtn.y + quitterBtn.h) {
                    running = 0;
                    wait = 0;
                }
            }
        }
        continue; 
    }
        // Musique & pause
        SDL_RenderCopy(renderer, musicActive ? musicOnTex : musicOffTex, NULL, &musicRect);
        SDL_RenderCopy(renderer, pauseTex, NULL, &pauseRect);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    // Nettoyage
    SDL_DestroyTexture(bestScoreTex);
    SDL_DestroyTexture(statsTex);
    SDL_DestroyTexture(scoreTex);
    SDL_DestroyTexture(linesTex);
    SDL_DestroyTexture(bgTex);
    SDL_DestroyTexture(musicOnTex);
    SDL_DestroyTexture(musicOffTex);
    SDL_DestroyTexture(pauseBgTex);
    SDL_DestroyTexture(btnContinuerTex);
    SDL_DestroyTexture(btnQuitterTex);
    SDL_DestroyTexture(nextTex);
    TTF_CloseFont(fontBig);
    TTF_CloseFont(fontSmall);
    TTF_Quit();

    return 1;
}

