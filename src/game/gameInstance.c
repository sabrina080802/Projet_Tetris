#include "gameInstance.h"

void resetGame(GameInstance *game)
{
    game->nextPiece = (rand() % 7) + 1;
    game->level = 1;
    game->totalPlayTime = 0.0f;
    game->downTimer = getTimeToFall(1);
    game->paused = false;
    game->running = true;
    game->rows = GRID_ROWS;
    game->cols = GRID_COLS;
    for (int i = 0; i < 7; i++)
        game->piecesCounts[i] = 0;
}

void executeGameAction(GameInstance *game, char **grid, int userActionId)
{
    if (userActionId < 10)
        return;

    char **shape = getShape(game->currentPiece.shape);
    for (int i = 0; i < game->currentPiece.rotation; i++)
        rotateShape(shape);

    char **gridCopy = copyGrid(grid, game->rows, game->cols);
    switch (userActionId)
    {
    case USER_GO_LEFT:
        if (!collides(shape, game->currentPiece.row, game->currentPiece.col - 1, gridCopy, game->rows, game->cols))
        {
            game->currentPiece.col--;
            applyGrid(grid, gridCopy, game->rows, game->cols);
        }
        break;
    case USER_GO_RIGHT:
        if (!collides(shape, game->currentPiece.row, game->currentPiece.col + 1, gridCopy, game->rows, game->cols))
        {
            game->currentPiece.col++;
            applyGrid(grid, gridCopy, game->rows, game->cols);
        }
        break;
    case USER_ROTATE:
        rotateShape(shape);
        if (collides(shape, game->currentPiece.row, game->currentPiece.col, gridCopy, game->rows, game->cols))
        {
            if (game->currentPiece.col < GRID_COLS / 2)
                game->currentPiece.col++;
            else
                game->currentPiece.col--;
        }

        game->currentPiece.rotation = (game->currentPiece.rotation + 1) % 4;
        applyGrid(grid, gridCopy, game->rows, game->cols);
        break;
    }

    freeGrid(gridCopy, game->rows, game->cols);
    freeGrid(shape, 4, 4);
}
void spawnNextPiece(GameInstance *game)
{
    game->currentPiece.shape = game->nextPiece;
    game->currentPiece.row = 0;
    game->currentPiece.col = (game->cols / 2) - 1;
    game->currentPiece.rotation = 0;
    game->piecesCounts[game->nextPiece - 1]++;

    game->nextPiece = (rand() % 7) + 1;
}

float getTimeToFall(int level)
{
    if (level > 30)
        return (framesToFall[29] / FPS);

    return framesToFall[level] / FPS;
}
bool isTimeToPlay(GameInstance *game, float deltaTime)
{
    game->playTimer -= deltaTime;
    return game->playTimer <= 0;
}
bool isTimeToDown(GameInstance *game, float deltaTime)
{
    game->downTimer -= deltaTime;
    if (game->downTimer <= 0)
    {
        game->downTimer = getTimeToFall(game->level);
        return true;
    }
    else
        return false;
}
int dropCompletedLines(char **grid, int rows, int cols)
{
    int lineIndex = 0;

    for (int r = rows - 1; r >= 0; r--)
    {
        bool isCompleteLine = true;
        for (int c = 0; c < cols && isCompleteLine; c++)
            isCompleteLine = grid[r][c] != 0;

        if (isCompleteLine)
        {
            for (int c = 0; c < cols; c++)
            {
                for (int r2 = r; r2 >= 1; r2--)
                    grid[r2][c] = grid[r2 - 1][c];

                grid[0][c] = 0;
            }

            r++;
            lineIndex++;
        }
    }

    return lineIndex;
}
