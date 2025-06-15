#pragma once

#include <stdbool.h>
#include "../logic/grid.h"
#include <string.h>
#include <stdio.h>

typedef struct
{
    int type, x, y, rotation;
    int shape[4][4];
} Tetromino;

extern int spawnedPieceCount[7];

Tetromino createTetromino(int type);
bool rotateTetromino(Tetromino *t);
bool collides(Tetromino *t, int grid[GRID_ROWS][GRID_COLS]);
void spawnPiece(Tetromino *t);
int getTotalPieces(void);
