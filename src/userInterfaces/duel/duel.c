#include "duel.h"

void showDuel(App *app)
{
    Duel duel = createDuel(app);
    renderDuel(&duel);
    destroyDuel(&duel);
}
Duel createDuel(App *app)
{
    SDL_Renderer *r = app->renderer;
    Duel duel = {
        .app = app,
        .bgTex = loadTexture(r, HOME_BG),
        .p1GridFrame = createFrame(r, VIOLET_FONCE, VIOLET_BORD, 25),
        .p2GridFrame = createFrame(r, VIOLET_FONCE, VIOLET_BORD, 25),
        .p1Grid = createGridRenderer(r, GRID_ROWS, GRID_COLS),
        .p2Grid = createGridRenderer(r, GRID_ROWS, GRID_COLS),

        .p1NextPieceFrame = createFrame(r, VIOLET_FONCE, VIOLET_BORD, 15),
        .p2NextPieceFrame = createFrame(r, VIOLET_FONCE, VIOLET_BORD, 15),
        .p1NextPieceGrid = createGridRenderer(r, 3, 6),
        .p2NextPieceGrid = createGridRenderer(r, 3, 6),

        .p1PieceStats = createPieceStats(r, app->smallFont),
        .p2PieceStats = createPieceStats(r, app->smallFont),

        .btnPause = createButton(r, BTN_PAUSE)};

    initLabel(&duel.gameMode, "Duel", r, app->bigFont, WHITE);

    return duel;
}
void updateDuel(Duel *duel)
{
    const int spacing = 20;
    int winW, winH;
    SDL_GetRendererOutputSize(duel->app->renderer, &winW, &winH);
    duel->bgRect.w = winW;
    duel->bgRect.h = winH;

    int gridH = winH - 200;
    int gridW = (gridH / duel->p1Grid.rows) * duel->p1Grid.cols;
    duel->p1Grid.cellSize = gridH / duel->p1Grid.rows;
    duel->p2Grid.cellSize = duel->p1Grid.cellSize;
    duel->p1Grid.layout_y = 100;
    duel->p2Grid.layout_y = duel->p1Grid.layout_y;
    duel->p1Grid.layout_x = winW / 2 - gridW - spacing * 2;
    duel->p2Grid.layout_x = winW / 2 + spacing * 2;

    updateDuelGridFrame(&duel->p1Grid, &duel->p1GridFrame);
    updateDuelGridFrame(&duel->p2Grid, &duel->p2GridFrame);

    int pCellSize = duel->p1Grid.cellSize / 1.75;
    duel->p1PieceStats.grid.cellSize = pCellSize;
    duel->p1PieceStats.rect.w = duel->p1PieceStats.grid.cols * pCellSize + 50;
    duel->p1PieceStats.rect.h = duel->p1PieceStats.grid.rows * pCellSize;
    duel->p1PieceStats.rect.x = duel->p1GridFrame.rect.x - duel->p1PieceStats.rect.w - spacing;
    duel->p1PieceStats.rect.y = duel->p1GridFrame.rect.y + duel->p1GridFrame.rect.h - duel->p1PieceStats.rect.h;

    duel->p2PieceStats.grid.cellSize = pCellSize;
    duel->p2PieceStats.rect.w = duel->p1PieceStats.rect.w;
    duel->p2PieceStats.rect.h = duel->p1PieceStats.rect.h;
    duel->p2PieceStats.rect.x = duel->p2GridFrame.rect.x + duel->p2GridFrame.rect.w + spacing;
    duel->p2PieceStats.rect.y = duel->p1PieceStats.rect.y;

    duel->p1NextPieceGrid.cellSize = duel->p1Grid.cellSize;
    duel->p1NextPieceFrame.rect.w = duel->p1NextPieceGrid.cellSize * duel->p1NextPieceGrid.cols;
    duel->p1NextPieceFrame.rect.h = duel->p1NextPieceGrid.cellSize * duel->p1NextPieceGrid.rows;
    duel->p1NextPieceFrame.rect.x = duel->p1GridFrame.rect.x - duel->p1NextPieceFrame.rect.w - spacing;
    duel->p1NextPieceFrame.rect.y = duel->p1GridFrame.rect.y;
    duel->p1NextPieceGrid.layout_x = duel->p1NextPieceFrame.rect.x;
    duel->p1NextPieceGrid.layout_y = duel->p1NextPieceFrame.rect.y + duel->p1NextPieceGrid.cellSize / 2;

    duel->p2NextPieceGrid.cellSize = duel->p2Grid.cellSize;
    duel->p2NextPieceFrame.rect.w = duel->p1NextPieceFrame.rect.w;
    duel->p2NextPieceFrame.rect.h = duel->p1NextPieceFrame.rect.h;
    duel->p2NextPieceFrame.rect.x = duel->p2GridFrame.rect.x + duel->p2GridFrame.rect.w + spacing;
    duel->p2NextPieceFrame.rect.y = duel->p2GridFrame.rect.y;
    duel->p2NextPieceGrid.layout_x = duel->p2NextPieceFrame.rect.x;
    duel->p2NextPieceGrid.layout_y = duel->p2NextPieceFrame.rect.y + duel->p2NextPieceGrid.cellSize / 2;

    SDL_Rect *soundRect = &duel->app->sounds.rect;
    duel->btnPause.rect.x = soundRect->x - duel->btnPause.rect.w;
    duel->btnPause.rect.y = soundRect->y;
    duel->btnPause.rect.w = (duel->btnPause.rect.w * soundRect->h) / duel->btnPause.rect.h;
    duel->btnPause.rect.h = soundRect->h;

    duel->gameMode.rect.y = duel->p1GridFrame.rect.y - duel->gameMode.rect.h - 5;
    duel->gameMode.rect.x = winW / 2 - duel->gameMode.rect.w / 2;

    updateDuelScoreLabels(duel);
    duel->time.rect.x = winW / 2 - duel->time.rect.w / 2;
    duel->time.rect.y = duel->p1GridFrame.rect.y + duel->p1GridFrame.rect.h + 5;

    duel->p1Level.rect.x = duel->p1NextPieceFrame.rect.x + duel->p1NextPieceFrame.rect.w - duel->p1Level.rect.w;
    duel->p1LinesCount.rect.x = duel->p1NextPieceFrame.rect.x + duel->p1NextPieceFrame.rect.w - duel->p1LinesCount.rect.w;
    duel->p1Score.rect.x = duel->p1NextPieceFrame.rect.x + duel->p1NextPieceFrame.rect.w - duel->p1Score.rect.w;

    duel->p1Level.rect.y = duel->p1NextPieceFrame.rect.y + duel->p1NextPieceFrame.rect.h + 5;
    duel->p1LinesCount.rect.y = duel->p1Level.rect.y + duel->p1Level.rect.h + 5;
    duel->p1Score.rect.y = duel->p1LinesCount.rect.y + duel->p1LinesCount.rect.h + 5;

    duel->p2Level.rect.x = duel->p2NextPieceFrame.rect.x;
    duel->p2Level.rect.y = duel->p1Level.rect.y;
    duel->p2LinesCount.rect.x = duel->p2NextPieceFrame.rect.x;
    duel->p2LinesCount.rect.y = duel->p1LinesCount.rect.y;
    duel->p2Score.rect.x = duel->p2NextPieceFrame.rect.x;
    duel->p2Score.rect.y = duel->p1Score.rect.y;
}
void updateDuelScoreLabels(Duel *duel)
{
    initLabel(&duel->time, msTimeToString(duel->p1Game.totalPlayTime), duel->app->renderer, duel->app->bigFont, WHITE);

    static char p1LvlBuffer[32], p1ScoreBuffer[32], p1LinesCountBuffer[32];
    snprintf(p1LvlBuffer, sizeof(p1LvlBuffer), "Niveau %d", duel->p1Game.level);
    snprintf(p1ScoreBuffer, sizeof(p1ScoreBuffer), "%d points", duel->p1Game.score);
    snprintf(p1LinesCountBuffer, sizeof(p1LinesCountBuffer), "%d lignes", duel->p1Game.linesCount);

    static char p2LvlBuffer[32], p2ScoreBuffer[32], p2LinesCountBuffer[32];
    snprintf(p2LvlBuffer, sizeof(p2LvlBuffer), "Niveau %d", duel->p2Game.level);
    snprintf(p2ScoreBuffer, sizeof(p2ScoreBuffer), "%d points", duel->p2Game.score);
    snprintf(p2LinesCountBuffer, sizeof(p2LinesCountBuffer), "%d lignes", duel->p2Game.linesCount);

    initLabel(&duel->p1Level, p1LvlBuffer, duel->app->renderer, duel->app->smallFont, WHITE);
    initLabel(&duel->p1Score, p1ScoreBuffer, duel->app->renderer, duel->app->smallFont, WHITE);
    initLabel(&duel->p1LinesCount, p1LinesCountBuffer, duel->app->renderer, duel->app->smallFont, WHITE);
    initLabel(&duel->p2Level, p2LvlBuffer, duel->app->renderer, duel->app->smallFont, WHITE);
    initLabel(&duel->p2Score, p2ScoreBuffer, duel->app->renderer, duel->app->smallFont, WHITE);
    initLabel(&duel->p2LinesCount, p2LinesCountBuffer, duel->app->renderer, duel->app->smallFont, WHITE);
}
void updateDuelGridFrame(GridRenderer *gridRenderer, Frame *frame)
{
    frame->rect.w = gridRenderer->cellSize * gridRenderer->cols;
    frame->rect.h = gridRenderer->cellSize * gridRenderer->rows;
    frame->rect.x = gridRenderer->layout_x;
    frame->rect.y = gridRenderer->layout_y;
}
void renderDuel(Duel *duel)
{
    resetGame(&duel->p1Game);
    resetGame(&duel->p2Game);
    resetGrid(duel->p1Grid.grid, duel->p1Game.rows, duel->p1Game.cols);
    resetGrid(duel->p2Grid.grid, duel->p2Game.rows, duel->p2Game.cols);
    resetGrid(duel->p1NextPieceGrid.grid, duel->p1NextPieceGrid.rows, duel->p1NextPieceGrid.cols);
    resetGrid(duel->p2NextPieceGrid.grid, duel->p2NextPieceGrid.rows, duel->p2NextPieceGrid.cols);
    srand(time(NULL));
    SDL_Renderer *r = duel->app->renderer;
    SDL_Event e;

    GameEnds ends = createGameEnds(duel->app, &duel->p1Game);
    GamePaused pause = createGamePaused(duel->app, &duel->p1Game);
    GameState p1State, p2State;
    spawnNextPiece(&duel->p1Game);
    p1State.nextPieceShape = getShape(duel->p1Game.nextPiece);
    resetGrid(duel->p1NextPieceGrid.grid, duel->p1NextPieceGrid.rows, duel->p1NextPieceGrid.cols);
    applyShape(p1State.nextPieceShape, 0, 1, duel->p1NextPieceGrid.grid, duel->p1NextPieceGrid.rows, duel->p1NextPieceGrid.cols);
    spawnNextPiece(&duel->p2Game);
    p2State.nextPieceShape = getShape(duel->p2Game.nextPiece);
    resetGrid(duel->p2NextPieceGrid.grid, duel->p2NextPieceGrid.rows, duel->p2NextPieceGrid.cols);
    applyShape(p2State.nextPieceShape, 0, 1, duel->p2NextPieceGrid.grid, duel->p2NextPieceGrid.rows, duel->p2NextPieceGrid.cols);

    while ((duel->p1Game.running && duel->p2Game.running) || duel->p1Game.paused)
    {
        prepareStateToPlay(&duel->p1Game, &p1State, duel->p1Grid.grid);
        prepareStateToPlay(&duel->p2Game, &p2State, duel->p2Grid.grid);

        if (duel->p1Game.paused == false)
        {
            duel->p1Game.totalPlayTime += FRAME_TIME_MS;
            int nextP1Action = NO_ACTION;
            while (SDL_PollEvent(&e))
                handleUserAction(&duel->p1Game, &e, &nextP1Action, &duel->btnPause, &duel->app->sounds);

            if (nextP1Action != NO_ACTION)
                p1State.userAction = nextP1Action;

            if (nextP1Action >= 10 || isTimeToPlay(&duel->p1Game, FRAME_TIME_SEC))
            {
                duel->p1Game.playTimer = PLAY_TIMER;
                executeGameAction(&duel->p1Game, p1State.renderingGrid, p1State.userAction);
                if (p1State.userAction == USER_ROTATE)
                    p1State.userAction = NO_ACTION;
            }
            if (isTimeToPlay(&duel->p2Game, FRAME_TIME_SEC))
            {
                int nextP2Action = getAINextUserAction(&duel->p2Game, &p2State);
                p2State.userAction = nextP2Action;

                duel->p2Game.playTimer = PLAY_TIMER;
                executeGameAction(&duel->p2Game, p2State.renderingGrid, p2State.userAction);
            }

            int p1CompletedLines = playState(&duel->p1Game, &p1State);
            if (p1CompletedLines > -1)
            {
                if (p1CompletedLines > 0 && duel->app->sounds.isActive)
                    Mix_PlayChannel(-1, duel->app->lineSound, 0);

                freeGrid(p1State.nextPieceShape, 4, 4);
                resetGrid(duel->p1NextPieceGrid.grid, duel->p1NextPieceGrid.rows, duel->p1NextPieceGrid.cols);

                spawnNextPiece(&duel->p1Game);
                p1State.nextPieceShape = getShape(duel->p1Game.nextPiece);
                applyShape(p1State.nextPieceShape, 0, 1, duel->p1NextPieceGrid.grid, duel->p1NextPieceGrid.rows, duel->p1NextPieceGrid.cols);

                for (int i = 0; i < p1CompletedLines; i++)
                    addGhostLine(p2State.grid, duel->p2Grid.rows, duel->p2Grid.cols);
            }

            int p2CompletedLines = playState(&duel->p2Game, &p2State);
            if (p2CompletedLines > -1)
            {
                if (p2CompletedLines > 0 && duel->app->sounds.isActive)
                    Mix_PlayChannel(-1, duel->app->lineSound, 0);

                freeGrid(p2State.nextPieceShape, 4, 4);
                resetGrid(duel->p2NextPieceGrid.grid, duel->p2NextPieceGrid.rows, duel->p2NextPieceGrid.cols);

                spawnNextPiece(&duel->p2Game);
                p2State.nextPieceShape = getShape(duel->p2Game.nextPiece);
                applyShape(p2State.nextPieceShape, 0, 1, duel->p2NextPieceGrid.grid, duel->p2NextPieceGrid.rows, duel->p2NextPieceGrid.cols);

                for (int i = 0; i < p2CompletedLines; i++)
                    addGhostLine(p1State.grid, duel->p1Grid.rows, duel->p1Grid.cols);
            }

            if (!duel->p1Game.running || !duel->p2Game.running)
            {
                duel->p1Game.running = false;
                duel->p1Game.paused = true;
            }
        }

        showGhost(p1State.grid, p1State.renderingGrid, p1State.ghostShape, duel->p1Game.currentPiece.row, duel->p1Game.currentPiece.col, duel->p1Grid.rows, duel->p1Grid.cols);
        showGhost(p2State.grid, p2State.renderingGrid, p2State.ghostShape, duel->p2Game.currentPiece.row, duel->p2Game.currentPiece.col, duel->p2Grid.rows, duel->p2Grid.cols);

        applyShape(p1State.pieceShape, duel->p1Game.currentPiece.row, duel->p1Game.currentPiece.col, p1State.renderingGrid, duel->p1Grid.rows, duel->p1Grid.cols);
        applyShape(p2State.pieceShape, duel->p2Game.currentPiece.row, duel->p2Game.currentPiece.col, p2State.renderingGrid, duel->p2Grid.rows, duel->p2Grid.cols);

        updateDuel(duel);

        SDL_RenderClear(r);
        SDL_RenderCopy(r, duel->bgTex, NULL, &duel->bgRect);

        renderLabel(&duel->time);
        renderLabel(&duel->gameMode);
        renderLabel(&duel->p1Level);
        renderLabel(&duel->p2Level);
        renderLabel(&duel->p1Score);
        renderLabel(&duel->p2Score);
        renderLabel(&duel->p1LinesCount);
        renderLabel(&duel->p2LinesCount);

        renderPieceStats(&duel->p1PieceStats, duel->p1Game.piecesCounts);
        renderPieceStats(&duel->p2PieceStats, duel->p2Game.piecesCounts);
        renderFrame(&duel->p1NextPieceFrame);
        renderGridRenderer(&duel->p1NextPieceGrid, duel->p1NextPieceGrid.grid);
        renderFrame(&duel->p2NextPieceFrame);
        renderGridRenderer(&duel->p2NextPieceGrid, duel->p2NextPieceGrid.grid);

        renderFrame(&duel->p1GridFrame);
        renderGridRenderer(&duel->p1Grid, p1State.renderingGrid);
        renderFrame(&duel->p2GridFrame);
        renderGridRenderer(&duel->p2Grid, p2State.renderingGrid);

        renderButton(&duel->btnPause);
        renderSound(&duel->app->sounds);

        freeGrid(p1State.ghostShape, 4, 4);
        freeGrid(p2State.ghostShape, 4, 4);
        freeGrid(p1State.pieceShape, 4, 4);
        freeGrid(p2State.pieceShape, 4, 4);
        freeGrid(p1State.renderingGrid, duel->p1Grid.rows, duel->p1Grid.cols);
        freeGrid(p2State.renderingGrid, duel->p2Grid.rows, duel->p2Grid.cols);

        if (duel->p1Game.paused)
        {
            if (duel->p1Game.running)
                renderGamePaused(&duel->p1Game, &pause);
            else
            {
                renderGameEnds(&duel->p1Game, &ends);
                if (duel->p1Game.running)
                {
                    renderDuel(duel);
                    return;
                }
            }
        }

        SDL_RenderPresent(r);
        SDL_Delay(FRAME_TIME_MS);
    }
}
void destroyDuel(Duel *duel)
{
    destroyPieceStats(&duel->p1PieceStats);
    destroyPieceStats(&duel->p2PieceStats);
    destroyGridRenderer(&duel->p1Grid);
    destroyGridRenderer(&duel->p2Grid);
    destroyButton(&duel->btnPause);

    destroyLabel(&duel->gameMode);
    destroyLabel(&duel->p1LinesCount);
    destroyLabel(&duel->p2LinesCount);
    destroyLabel(&duel->p1Score);
    destroyLabel(&duel->p2Score);
    destroyLabel(&duel->p1Level);
    destroyLabel(&duel->p2Level);
}
