#include "render.h"
#include "../logic/pieces.h"  
#include "../ui/textures.h"   
#include "../logic/grid.h"
#include "text.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void drawTetromino(SDL_Renderer* renderer, Tetromino* t, SDL_Texture* blockTextures[], int blockSize, int offsetX, int offsetY) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (t->shape[row][col]) {
                SDL_Rect rect = {
                    offsetX + (t->x + col) * blockSize,
                    offsetY + (t->y + row) * blockSize,
                    blockSize,
                    blockSize
                };
                SDL_RenderCopy(renderer, blockTextures[t->type], NULL, &rect);
            }
        }
    }
}

void drawGrid(SDL_Renderer* renderer, SDL_Texture* blockTextures[], int blockSize, int offsetX, int offsetY) {
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            int cell = grid[row][col];
            if (cell > 0) {
                SDL_Rect rect = {
                    offsetX + col * blockSize,
                    offsetY + row * blockSize,
                    blockSize,
                    blockSize
                };
                SDL_RenderCopy(renderer, blockTextures[cell - 1], NULL, &rect);
            }
        }
    }
}
void drawPieceStats(SDL_Renderer* renderer, SDL_Texture* textures[], int blockSizeMax, int offsetX, int offsetY, TTF_Font* font) {
    SDL_Color white = {255, 255, 255, 255};

    int blockSize = blockSizeMax / 3;  
    int spacing = blockSize * 4 + 10;  

    for (int type = 0; type < 7; type++) {
        int y = offsetY + type * spacing;
        int x = offsetX;

        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                Tetromino t = createTetromino(type);
                if (t.shape[row][col]) {
                    SDL_Rect dest = {
                        x + col * blockSize,
                        y + row * blockSize,
                        blockSize,
                        blockSize
                    };
                    SDL_RenderCopy(renderer, textures[type], NULL, &dest);
                }
            }
        }

        char text[16];
        snprintf(text, sizeof(text), "x%d", pieceCount[type]);
        int tw, th;
        SDL_Texture* tex = renderText(renderer, font, text, white, &tw, &th);
        SDL_Rect texRect = {
            x + 4 * blockSize + 8,
            y + blockSize,
            tw, th
        };
        SDL_RenderCopy(renderer, tex, NULL, &texRect);
        SDL_DestroyTexture(tex);
    }
}