#include "gamemode_ui.h"

void initGameModeUI(SDL_Renderer *renderer, GameModeUI *ui)
{
    int winW, winH;
    SDL_GetRendererOutputSize(renderer, &winW, &winH);

    ui->bgRect = (SDL_Rect){0, 0, winW, winH};

    int gridWidth = BLOCK_SIZE * GRID_COLS;
    int gridHeight = BLOCK_SIZE * GRID_ROWS;
    int gridX = (winW - gridWidth) / 2;
    int gridY = (winH - gridHeight) / 2;
    ui->wellRect = (SDL_Rect){gridX, gridY, gridWidth, gridHeight};

    ui->scoreRect = (SDL_Rect){gridX + gridWidth + UI_MARGIN, gridY, UI_WIDTH, 100};
    ui->linesRect = (SDL_Rect){ui->scoreRect.x, ui->scoreRect.y + ui->scoreRect.h + UI_SPACING, UI_WIDTH, 100};
    ui->nextRect = (SDL_Rect){ui->scoreRect.x, ui->linesRect.y + ui->linesRect.h + UI_SPACING, UI_WIDTH, 150};
    ui->statsRect = (SDL_Rect){gridX - UI_WIDTH - UI_MARGIN, gridY, UI_WIDTH, 450};

    ui->titleRect = (SDL_Rect){gridX, gridY - 60, 0, 0};
    ui->bestScoreRect = (SDL_Rect){ui->statsRect.x, ui->statsRect.y - 30, 0, 0};
    ui->statsLabelRect = (SDL_Rect){ui->statsRect.x + 10, ui->statsRect.y + 10, 0, 0};
    ui->scoreTextRect = (SDL_Rect){ui->scoreRect.x + 10, ui->scoreRect.y + 10, 0, 0};
    ui->linesTextRect = (SDL_Rect){ui->linesRect.x + 10, ui->linesRect.y + 10, 0, 0};
    ui->nextLabelRect = (SDL_Rect){ui->nextRect.x + 10, ui->nextRect.y + 10, 0, 0};

    ui->musicRect = (SDL_Rect){winW - MUSIC_BUTTON_SIZE - UI_MARGIN, UI_MARGIN, MUSIC_BUTTON_SIZE, MUSIC_BUTTON_SIZE};
    ui->pauseRect = (SDL_Rect){winW - (MUSIC_BUTTON_SIZE + UI_MARGIN) * 2, UI_MARGIN, MUSIC_BUTTON_SIZE, MUSIC_BUTTON_SIZE};
}
