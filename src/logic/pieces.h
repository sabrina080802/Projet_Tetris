// pieces.h
#ifndef PIECES_H
#define PIECES_H

#include <stdbool.h>
#include "../logic/grid.h"

typedef struct {
    int type, x, y, rotation;
    int shape[4][4];
} Tetromino;

extern int pieceCount[7];

Tetromino createTetromino(int type);
void rotateTetromino(Tetromino* t);
bool collides(Tetromino* t, int grid[GRID_ROWS][GRID_COLS]);
void spawnPiece(Tetromino* t);

int getTotalPieces(void);

#endif 
