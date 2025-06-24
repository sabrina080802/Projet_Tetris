#include "gridRenderer.h"

GridRenderer createGridRenderer(SDL_Renderer *renderer, int rows, int cols)
{
    GridRenderer grid = {
        .rows = rows,
        .cols = cols,
        .r = renderer,
        .grid = createGrid(rows, cols)};
    return grid;
}
void renderGridRenderer(GridRenderer *gridRenderer, char **grid)
{
    for (int r = 0; r < gridRenderer->rows; r++)
    {
        for (int c = 0; c < gridRenderer->cols; c++)
        {
            if (grid[r][c] > 0)
            {
                SDL_Rect rect = {
                    gridRenderer->layout_x + c * gridRenderer->cellSize,
                    gridRenderer->layout_y + r * gridRenderer->cellSize,
                    gridRenderer->cellSize,
                    gridRenderer->cellSize};

                SDL_RenderCopy(
                    gridRenderer->r,
                    blockTextures[(int)grid[r][c] - 1],
                    NULL, &rect);
            }
        }
    }
}
void destroyGridRenderer(GridRenderer *grid)
{
    freeGrid(grid->grid, grid->rows, grid->cols);
}