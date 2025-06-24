#include "shapes.h"

const char tetrominoShapes[7][4][4] = {
    {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // I
    {{0, 2, 2, 0}, {2, 2, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // S
    {{3, 3, 0, 0}, {0, 3, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // Z
    {{0, 4, 4, 0}, {0, 4, 4, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // O
    {{5, 0, 0, 0}, {5, 5, 5, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // J
    {{0, 0, 6, 0}, {6, 6, 6, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // L
    {{0, 7, 0, 0}, {7, 7, 7, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}  // T
};

char **getShape(int shapeId)
{
    char **shape = malloc(4 * sizeof(char *));
    for (int i = 0; i < 4; i++)
    {
        shape[i] = malloc(4 * sizeof(char));
        for (int j = 0; j < 4; j++)
            shape[i][j] = tetrominoShapes[shapeId - 1][i][j];
    }

    return shape;
}
int getShapeHeight(char **shape)
{
    int d = 0;
    bool isCounting = false;
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (shape[r][c] != 0)
            {
                if (isCounting)
                    d++;
                else
                {
                    isCounting = true;
                    d = 1;
                }
                break;
            }
        }
    }
    return d;
}
int getShapeWidth(char **shape)
{
    int w = 0;
    for (int r = 0; r < 4; r++)
    {
        int firstCol = -1, lastCol = -1;
        for (int c = 0; c < 4; c++)
        {
            if (shape[r][c] != 0)
            {
                if (firstCol == -1)
                    firstCol = c;
                lastCol = c;
            }
        }

        if (firstCol != -1)
        {
            int width = lastCol - firstCol + 1;
            if (width > w)
                w = width;
        }
    }
    return w;
}
char **getShapeAsGhost(int shapeId)
{
    char **shape = malloc(4 * sizeof(char *));
    for (int i = 0; i < 4; i++)
    {
        shape[i] = malloc(4 * sizeof(char));
        for (int j = 0; j < 4; j++)
        {
            if (tetrominoShapes[shapeId - 1][i][j] > 0)
                shape[i][j] = 8;
            else
                shape[i][j] = 0;
        }
    }

    return shape;
}