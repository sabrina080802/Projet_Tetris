#pragma once
#include <limits.h>
#include "grid.h"
#include "gameInstance.h"
#include "gameState.h"
#include "../values/userActions.h"

int getAINextUserAction(GameInstance *game, GameState *state);
int evaluateGrid(char **grid, int rows, int cols);