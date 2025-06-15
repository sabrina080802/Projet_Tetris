#pragma once

#include "../logic/grid.h"
#include "../logic/pieces.h"
#include <SDL2/SDL.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct
{
    int g[GRID_ROWS][GRID_COLS];
    Tetromino cur, nxt;
    Uint32 lastFall, delay;
    int score, lines, level;
    int iaPlanned, iaTargetX, iaTargetRot;
} Player;

void ia_plan(Player *p);
void ia_move(Player *p);
