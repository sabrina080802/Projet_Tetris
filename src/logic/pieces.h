#ifndef PIECES_H
#define PIECES_H
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "grid.h"


#define NUM_TETROMINOS 7

typedef struct {
    int x, y;
    int type;
    int rotation; 
    int shape[4][4];
} Tetromino;

bool collides(Tetromino* t, int grid[GRID_ROWS][GRID_COLS]);

Tetromino createTetromino(int type);
void drawTetromino(SDL_Renderer* renderer, Tetromino* t, SDL_Texture* textures[], int blockSize, int offsetX, int offsetY);
void rotateTetromino(Tetromino* t);
void drawGrid(SDL_Renderer* renderer, SDL_Texture* textures[], int blockSize, int offsetX, int offsetY);

#endif
