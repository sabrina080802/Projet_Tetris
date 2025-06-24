#include "gameState.h"

void prepareStateToPlay(GameInstance *game, GameState *state, char **grid)
{
    Piece *piece = &game->currentPiece;
    state->pieceShape = getShape(piece->shape);
    state->ghostShape = getShapeAsGhost(piece->shape);
    state->grid = grid;

    for (int i = 0; i < piece->rotation; i++)
    {
        rotateShape(state->pieceShape);
        rotateShape(state->ghostShape);
    }

    state->renderingGrid = copyGrid(state->grid, game->rows, game->cols);
}
int playState(GameInstance *game, GameState *state)
{
    if (isTimeToDown(game, FRAME_TIME_SEC) || state->userAction == USER_ACCELERATE)
    {
        bool goNextPiece = false;
        if (!collides(state->pieceShape, game->currentPiece.row + DOWN_SPEED, game->currentPiece.col, state->grid, game->rows, game->cols))
        {
            game->currentPiece.row += DOWN_SPEED;
            goNextPiece = state->userAction != NO_ACTION && collides(state->pieceShape, game->currentPiece.row + DOWN_SPEED, game->currentPiece.col, state->grid, game->rows, game->cols);
        }
        else if (game->currentPiece.row == 0)
        {
            game->running = false;
        }
        else
            goNextPiece = true;

        if (goNextPiece)
        {
            state->userAction = NO_ACTION;
            applyShape(state->pieceShape, game->currentPiece.row, game->currentPiece.col, state->grid, game->rows, game->cols);
            applyShape(state->pieceShape, game->currentPiece.row, game->currentPiece.col, state->renderingGrid, game->rows, game->cols);

            int completedLines = dropCompletedLines(state->grid, game->rows, game->cols);

            game->linesCount += completedLines;
            game->level = (game->linesCount / 10) + 1;
            game->score += (800 * completedLines) * game->level;

            return completedLines;
        }
    }

    return -1;
}