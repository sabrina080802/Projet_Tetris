#pragma once
#include "gameInstance.h"
#include "../values/gameValues.h"

typedef struct
{
    int userAction;
    char **grid;
    char **renderingGrid;
    char **pieceShape;
    char **ghostShape;
    char **nextPieceShape;
} GameState;

int playState(GameInstance *game, GameState *state);
void prepareStateToPlay(GameInstance *game, GameState *state, char **grid);