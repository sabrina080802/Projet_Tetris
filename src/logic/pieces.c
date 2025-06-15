#include "pieces.h"

int spawnedPieceCount[7] = {0};

Tetromino createTetromino(int type)
{
    Tetromino t = {.type = type, .x = 3, .y = 0, .rotation = 0};

    int shapes[7][4][4] = {
        {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // I
        {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // S
        {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // Z
        {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // O
        {{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // J
        {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // L
        {{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}  // T
    };

    memcpy(t.shape, shapes[type], sizeof(t.shape));
    return t;
}

bool rotateTetromino(Tetromino *t)
{
    if (t->type == 3)
        return false;

    t->rotation = (t->rotation + 1) % (t->type < 3 ? 2 : 4);

    int newShape[4][4] = {0};
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            newShape[col][3 - row] = t->shape[row][col];

    memcpy(t->shape, newShape, sizeof(newShape));
    return true;
}

bool collides(Tetromino *t, int grid[GRID_ROWS][GRID_COLS])
{
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (t->shape[row][col])
            {
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

void spawnPiece(Tetromino *t)
{
    t->x = (GRID_COLS - 4) / 2;
    t->y = -1;
    t->rotation = 0;
    spawnedPieceCount[t->type]++;
}

int getTotalPieces(void)
{
    int total = 0;
    for (int i = 0; i < 7; i++)
        total += spawnedPieceCount[i];
    return total;
}