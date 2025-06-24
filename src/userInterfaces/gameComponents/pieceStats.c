#include "pieceStats.h"

PieceStats createPieceStats(SDL_Renderer *r, TTF_Font *font)
{
    PieceStats pieceStats = {
        .renderer = r,
        .rect = {0, 0, 0, 0},
        .bgFrame = createFrame(r, VIOLET_FONCE, VIOLET_BORD, 15),
        .grid = createGridRenderer(r, 22, 6),
        .font = font};

    for (int i = 0; i < 7; i++)
    {
        char **shape = getShape(i + 1);
        applyShape(shape, 1 + (i * 3), 1, pieceStats.grid.grid, pieceStats.grid.rows, pieceStats.grid.cols);
        initLabel(&pieceStats.labels[i], "x0", r, pieceStats.font, WHITE);
    }

    return pieceStats;
}

void renderPieceStats(PieceStats *stats, int pieceCount[7])
{
    stats->bgFrame.rect.x = stats->rect.x;
    stats->bgFrame.rect.y = stats->rect.y;
    stats->bgFrame.rect.w = stats->rect.w;
    stats->bgFrame.rect.h = stats->rect.h;
    stats->grid.layout_x = stats->rect.x + 2;
    stats->grid.layout_y = stats->rect.y + 2;

    renderFrame(&stats->bgFrame);
    renderGridRenderer(&stats->grid, stats->grid.grid);

    for (int i = 0; i < 7; i++)
    {
        char str[20];
        sprintf(str, "x%d", pieceCount[i]);

        initLabel(&stats->labels[i], str, stats->renderer, stats->font, WHITE);

        stats->labels[i].rect.x = stats->rect.x + 6 * stats->grid.cellSize;
        stats->labels[i].rect.y = stats->rect.y + stats->grid.cellSize * (i * 3 + 1) + ((stats->grid.cellSize * 2) / 2 - stats->labels[i].rect.h / 2);
        SDL_RenderCopy(stats->renderer, stats->labels[i].texture, NULL, &stats->labels[i].rect);
    }
}
void destroyPieceStats(PieceStats *stats)
{
    for (int i = 0; i < 7; i++)
    {
        destroyLabel(&stats->labels[i]);
    }
}