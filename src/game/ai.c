#include "ai.h"

int getAINextUserAction(GameInstance *game, GameState *state)
{
    int bestScore = INT_MIN;
    char bestRotation = 0;
    int bestColumn = 0;

    char **shape = getShape(game->currentPiece.shape);
    for (char rotation = 0; rotation < 4; rotation++)
    {
        for (int col = 0; col < game->cols; col++)
        {
            int lowestRow = getLowestNoCollidesRow(shape, 0, col, state->grid, game->rows, game->cols);
            if (lowestRow < 0)
                continue;

            char **gridCopy = copyGrid(state->grid, game->rows, game->cols);
            applyShape(shape, lowestRow, col, gridCopy, game->rows, game->cols);
            int score = evaluateGrid(gridCopy, game->rows, game->cols);
            if (bestScore < score)
            {
                bestScore = score;
                bestColumn = col;
                bestRotation = rotation;
            }
            freeGrid(gridCopy, game->rows, game->cols);
        }

        rotateShape(shape);
    }

    freeGrid(shape, 4, 4);

    int nextAction = NO_ACTION;
    if (game->currentPiece.rotation != bestRotation)
        nextAction = USER_ROTATE;
    else
    {
        if (game->currentPiece.col < bestColumn)
            nextAction = USER_GO_RIGHT;
        else if (game->currentPiece.col > bestColumn)
            nextAction = USER_GO_LEFT;
        else
            nextAction = USER_ACCELERATE;
    }

    return nextAction;
}

int evaluateGrid(char **grid, int rows, int cols)
{
    int hMax = 0;
    int holes = 0;
    int completeLines = getCompletedLinesCount(grid, rows, cols);
    int roughness = getRoughness(grid, rows, cols);

    for (int c = 0; c < cols; c++)
    {
        int blockCount = 0;
        for (int r = 0; r < rows; r++)
        {
            if (grid[r][c])
            {
                if (!blockCount)
                {
                    blockCount = 1;
                    int h = rows - r;
                    if (h > hMax)
                        hMax = h;
                }
            }
            else if (blockCount)
                holes++;
        }
    }

    return -1 * hMax + 10 * completeLines - 5 * holes - 2 * roughness;
}