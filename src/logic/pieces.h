#ifndef PIECES_H
#define PIECES_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "../logic/grid.h"  // pour l'accès à GRID_ROWS, GRID_COLS

typedef struct {
    int shape[4][4];
    int x, y;
    int type;
    int rotation;
} Tetromino;

extern SDL_Texture* blockTextures[7];
extern int pieceCount[7];

Tetromino createTetromino(int type);
void rotateTetromino(Tetromino* t);
bool collides(Tetromino* t, int grid[GRID_ROWS][GRID_COLS]);
void spawnPiece(Tetromino* t);

#endif
