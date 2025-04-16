#include "pieces.h"
#include <string.h>
#include "grid.h"


Tetromino createTetromino(int type) {
Tetromino t = { .type = type, .x = 3, .y = 0, .rotation = 0 };

    switch (type) {
        case 0: { int s[4][4] = {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}}; memcpy(t.shape, s, sizeof(s)); break; } // I
        case 1: { int s[4][4] = {{0,1,1,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}; memcpy(t.shape, s, sizeof(s)); break; } // O
        case 2: { int s[4][4] = {{1,0,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}; memcpy(t.shape, s, sizeof(s)); break; } // J
        case 3: { int s[4][4] = {{0,1,1,0},{1,1,0,0},{0,0,0,0},{0,0,0,0}}; memcpy(t.shape, s, sizeof(s)); break; } // S
        case 4: { int s[4][4] = {{1,1,0,0},{0,1,1,0},{0,0,0,0},{0,0,0,0}}; memcpy(t.shape, s, sizeof(s)); break; } // Z
        case 5: { int s[4][4] = {{0,0,1,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}; memcpy(t.shape, s, sizeof(s)); break; } // L
        case 6: { int s[4][4] = {{0,1,0,0},{1,1,1,0},{0,0,0,0},{0,0,0,0}}; memcpy(t.shape, s, sizeof(s)); break; } // T
    }
    return t;
}

void drawTetromino(SDL_Renderer* renderer, Tetromino* t, SDL_Texture* textures[], int blockSize, int offsetX, int offsetY) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (t->shape[row][col]) {
                SDL_Rect dest = {
                    offsetX + (t->x + col) * blockSize,
                    offsetY + (t->y + row) * blockSize,
                    blockSize, blockSize
                };
                SDL_RenderCopy(renderer, textures[t->type], NULL, &dest);
            }
        }
    }
}

void rotateTetromino(Tetromino* t) {
    if (t->type == 1) return;

    if ((t->type == 0 || t->type == 3 || t->type == 4) && t->rotation == 1) {
        t->rotation = 0;
        return;
    }

    t->rotation = (t->rotation + 1) % 4;

    int newShape[4][4] = {0};
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            newShape[col][3 - row] = t->shape[row][col];
        }
    }
    memcpy(t->shape, newShape, sizeof(newShape));
}
bool collides(Tetromino* t, int grid[GRID_ROWS][GRID_COLS]) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (t->shape[row][col]) {
                int gx = t->x + col;
                int gy = t->y + row;

                if (gx < 0 || gx >= GRID_COLS || gy >= GRID_ROWS)
                    return true; 

                if (gy >= 0 && grid[gy][gx])
                    return true; 
            }
        }
    }
    return false;
}

void drawGrid(SDL_Renderer* renderer, SDL_Texture* textures[], int blockSize, int offsetX, int offsetY) {
    for (int y = 0; y < GRID_ROWS; y++) {
        for (int x = 0; x < GRID_COLS; x++) {
            int cell = grid[y][x];
            if (cell > 0) {
                SDL_Rect dest = {
                    offsetX + x * blockSize,
                    offsetY + y * blockSize,
                    blockSize, blockSize
                };
                SDL_RenderCopy(renderer, textures[cell - 1], NULL, &dest);
            }
        }
    }
}



