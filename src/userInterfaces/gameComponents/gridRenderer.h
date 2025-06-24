#pragma once
#include <SDL2/SDL.h>
#include "../styles.h"
#include "../textures.h"
#include "../../values/gameValues.h"
#include "../../game/grid.h"

typedef struct
{
    char **grid;
    int layout_x, layout_y;
    int rows, cols;
    int cellSize;
    SDL_Renderer *r;
} GridRenderer;

GridRenderer createGridRenderer(SDL_Renderer *renderer, int rows, int cols);
void renderGridRenderer(GridRenderer *gridRenderer, char **grid);
void destroyGridRenderer(GridRenderer *grid);