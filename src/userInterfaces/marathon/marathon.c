#include "marathon.h"

void showMarathon(App *app)
{
    Marathon marathon = createMarathon(app);
    renderMarathon(&marathon);
    destroyMarathon(&marathon);
}
Marathon createMarathon(App *app)
{
    SDL_Renderer *r = app->renderer;
    Marathon marathon = {
        .app = app,
        .bgTex = loadTexture(r, BG_INGAME),
        .gridFrame = createFrame(r, VIOLET_FONCE, VIOLET_BORD, 25),
        .pieceStats = createPieceStats(r, app->smallFont),
        .nextPieceGridFrame = createFrame(r, VIOLET_FONCE, VIOLET_BORD, 15),
        .grid = createGridRenderer(r, GRID_ROWS, GRID_COLS),
        .nextPieceGrid = createGridRenderer(r, 3, 6),
        .btnPause = createButton(r, BTN_PAUSE)};

    initLabel(&marathon.gameMode, "Marathon", r, app->bigFont, WHITE);

    return marathon;
}
void updateMarathon(Marathon *marathon)
{
    const int spacing = 40;

    int winW, winH;
    SDL_GetRendererOutputSize(marathon->app->renderer, &winW, &winH);
    marathon->bgRect.w = winW;
    marathon->bgRect.h = winH;

    int gridH = winH - 200;
    int gridW = (gridH / marathon->grid.rows) * marathon->grid.cols;
    marathon->grid.cellSize = gridH / marathon->grid.rows;
    marathon->grid.layout_y = 100;
    marathon->grid.layout_x = winW / 2 - gridW / 2;

    marathon->gridFrame.rect.w = gridW;
    marathon->gridFrame.rect.h = gridH;
    marathon->gridFrame.rect.x = marathon->grid.layout_x;
    marathon->gridFrame.rect.y = winH - 100 - marathon->gridFrame.rect.h;

    int pCellSize = marathon->grid.cellSize / 1.75;
    marathon->pieceStats.grid.cellSize = pCellSize;

    marathon->pieceStats.rect.w = marathon->pieceStats.grid.cols * pCellSize + 50;
    marathon->pieceStats.rect.h = marathon->pieceStats.grid.rows * pCellSize;
    marathon->pieceStats.rect.x = marathon->grid.layout_x - marathon->pieceStats.rect.w - spacing;
    marathon->pieceStats.rect.y = marathon->gridFrame.rect.y + marathon->gridFrame.rect.h - marathon->pieceStats.rect.h;

    marathon->nextPieceGrid.cellSize = marathon->grid.cellSize;
    marathon->nextPieceGridFrame.rect.w = marathon->grid.cellSize * marathon->nextPieceGrid.cols;
    marathon->nextPieceGridFrame.rect.h = marathon->grid.cellSize * marathon->nextPieceGrid.rows;
    marathon->nextPieceGridFrame.rect.x = marathon->gridFrame.rect.x + gridW + spacing;
    marathon->nextPieceGridFrame.rect.y = marathon->gridFrame.rect.y;
    marathon->nextPieceGrid.layout_x = marathon->nextPieceGridFrame.rect.x;
    marathon->nextPieceGrid.layout_y = marathon->nextPieceGridFrame.rect.y + marathon->nextPieceGrid.cellSize / 2;

    SDL_Rect *soundRect = &marathon->app->sounds.rect;
    marathon->btnPause.rect.x = soundRect->x - marathon->btnPause.rect.w;
    marathon->btnPause.rect.y = soundRect->y;
    marathon->btnPause.rect.w = (marathon->btnPause.rect.w * soundRect->h) / marathon->btnPause.rect.h;
    marathon->btnPause.rect.h = soundRect->h;

    marathon->gameMode.rect.y = marathon->gridFrame.rect.y - marathon->gameMode.rect.h - 5;
    marathon->gameMode.rect.x = winW / 2 - marathon->gameMode.rect.w / 2;

    updateMarathonScoreLabels(marathon);
    marathon->time.rect.x = marathon->nextPieceGridFrame.rect.x;
    marathon->time.rect.y = marathon->nextPieceGridFrame.rect.y + marathon->nextPieceGridFrame.rect.h + 5;

    marathon->currentLevel.rect.x = marathon->nextPieceGridFrame.rect.x;
    marathon->currentLevel.rect.y = marathon->time.rect.y + marathon->time.rect.h + 5;
    marathon->currentLinesCount.rect.x = marathon->currentLevel.rect.x;
    marathon->currentLinesCount.rect.y = marathon->currentLevel.rect.y + marathon->currentLevel.rect.h + 5;
    marathon->currentScore.rect.x = marathon->currentLevel.rect.x;
    marathon->currentScore.rect.y = marathon->currentLinesCount.rect.y + marathon->currentLinesCount.rect.h + 5;
}
void updateMarathonScoreLabels(Marathon *marathon)
{
    initLabel(&marathon->time, msTimeToString(marathon->game.totalPlayTime), marathon->app->renderer, marathon->app->smallFont, WHITE);

    static char levelBuffer[32], scoreBuffer[32], linesCountBuffer[32];
    snprintf(levelBuffer, sizeof(levelBuffer), "Niveau %d", marathon->game.level);
    snprintf(scoreBuffer, sizeof(scoreBuffer), "%d points", marathon->game.score);
    snprintf(linesCountBuffer, sizeof(linesCountBuffer), "%d lignes", marathon->game.linesCount);

    initLabel(&marathon->currentLinesCount, linesCountBuffer, marathon->app->renderer, marathon->app->smallFont, WHITE);
    initLabel(&marathon->currentScore, scoreBuffer, marathon->app->renderer, marathon->app->smallFont, WHITE);
    initLabel(&marathon->currentLevel, levelBuffer, marathon->app->renderer, marathon->app->smallFont, WHITE);
}
void renderMarathon(Marathon *marathon)
{
    resetGame(&marathon->game);
    resetGrid(marathon->grid.grid, marathon->game.rows, marathon->game.cols);
    resetGrid(marathon->nextPieceGrid.grid, marathon->nextPieceGrid.rows, marathon->nextPieceGrid.cols);
    srand(time(NULL));

    SDL_Renderer *r = marathon->app->renderer;
    SDL_Event e;

    GameEnds ends = createGameEnds(marathon->app, &marathon->game);
    GamePaused pause = createGamePaused(marathon->app, &marathon->game);
    GameState state;
    spawnNextPiece(&marathon->game);
    state.nextPieceShape = getShape(marathon->game.nextPiece);
    applyShape(state.nextPieceShape, 0, 1, marathon->nextPieceGrid.grid, marathon->nextPieceGrid.rows, marathon->nextPieceGrid.cols);

    while (marathon->game.running || marathon->game.paused)
    {
        prepareStateToPlay(&marathon->game, &state, marathon->grid.grid);
        if (marathon->game.paused == false)
        {
            marathon->game.totalPlayTime += FRAME_TIME_MS;
            int nextAction = NO_ACTION;
            while (SDL_PollEvent(&e))
                handleUserAction(&marathon->game, &e, &nextAction, &marathon->btnPause, &marathon->app->sounds);

            if (nextAction != NO_ACTION)
                state.userAction = nextAction;

            if (nextAction >= 10 || isTimeToPlay(&marathon->game, FRAME_TIME_SEC))
            {
                marathon->game.playTimer = PLAY_TIMER;
                executeGameAction(&marathon->game, state.renderingGrid, state.userAction);
                if (state.userAction == USER_ROTATE)
                    state.userAction = NO_ACTION;
            }

            int completedLines = playState(&marathon->game, &state);
            if (completedLines > -1)
            {
                if (completedLines > 0 && marathon->app->sounds.isActive)
                    Mix_PlayChannel(-1, marathon->app->lineSound, 0);

                freeGrid(state.nextPieceShape, 4, 4);
                resetGrid(marathon->nextPieceGrid.grid, marathon->nextPieceGrid.rows, marathon->nextPieceGrid.cols);

                spawnNextPiece(&marathon->game);
                state.nextPieceShape = getShape(marathon->game.nextPiece);
                applyShape(state.nextPieceShape, 0, 1, marathon->nextPieceGrid.grid, marathon->nextPieceGrid.rows, marathon->nextPieceGrid.cols);
            }

            if (!marathon->game.running)
                marathon->game.paused = true;
        }

        showGhost(state.grid, state.renderingGrid, state.ghostShape, marathon->game.currentPiece.row, marathon->game.currentPiece.col, marathon->grid.rows, marathon->grid.cols);

        applyShape(state.pieceShape,
                   marathon->game.currentPiece.row,
                   marathon->game.currentPiece.col,
                   state.renderingGrid, marathon->grid.rows, marathon->grid.cols);

        updateMarathon(marathon);
        SDL_RenderClear(r);
        SDL_RenderCopy(r, marathon->bgTex, NULL, &marathon->bgRect);

        renderLabel(&marathon->time);
        renderLabel(&marathon->gameMode);
        renderLabel(&marathon->currentLevel);
        renderLabel(&marathon->currentLinesCount);
        renderLabel(&marathon->currentScore);

        renderPieceStats(&marathon->pieceStats, marathon->game.piecesCounts);
        renderFrame(&marathon->gridFrame);
        renderGridRenderer(&marathon->grid, state.renderingGrid);
        renderFrame(&marathon->nextPieceGridFrame);
        renderGridRenderer(&marathon->nextPieceGrid, marathon->nextPieceGrid.grid);

        renderButton(&marathon->btnPause);
        renderSound(&marathon->app->sounds);

        freeGrid(state.ghostShape, 4, 4);
        freeGrid(state.pieceShape, 4, 4);
        freeGrid(state.renderingGrid, marathon->grid.rows, marathon->grid.cols);

        if (marathon->game.paused)
        {
            if (marathon->game.running)
                renderGamePaused(&marathon->game, &pause);
            else
            {
                renderGameEnds(&marathon->game, &ends);
                if (marathon->game.running)
                {
                    renderMarathon(marathon);
                    return;
                }
            }
        }

        SDL_RenderPresent(r);
        SDL_Delay(FRAME_TIME_MS);
    }
}
void destroyMarathon(Marathon *marathon)
{
    destroyPieceStats(&marathon->pieceStats);
    destroyGridRenderer(&marathon->grid);
    destroyButton(&marathon->btnPause);

    destroyLabel(&marathon->gameMode);
    destroyLabel(&marathon->currentLinesCount);
    destroyLabel(&marathon->currentScore);
    destroyLabel(&marathon->currentLevel);
}