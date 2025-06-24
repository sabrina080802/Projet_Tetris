#include "gameEnds.h"

GameEnds createGameEnds(App *app, GameInstance *game)
{
    GameEnds gameEnds = {
        .renderer = app->renderer,
        .bgTex = loadTexture(app->renderer, BG_GAMEOVER),
        .btnTryAgain = createButton(app->renderer, BTN_RECOMMENCER),
        .btnExit = createButton(app->renderer, BTN_QUITTER),
        .smallFont = app->smallFont};

    return gameEnds;
}
void handleGameEndsUserAction(GameInstance *gameInstance, GameEnds *gameEnds, const SDL_Event *e)
{
    if (e->type != SDL_MOUSEBUTTONUP)
        return;

    int mx = e->button.x;
    int my = e->button.y;
    if (containsPoint(mx, my, gameEnds->btnTryAgain.rect))
    {
        gameInstance->paused = true;
        gameInstance->running = true;
    }
    else if (containsPoint(mx, my, gameEnds->btnExit.rect))
    {
        gameInstance->paused = false;
        gameInstance->running = false;
    }
}
void renderGameEnds(GameInstance *game, GameEnds *gameEnds)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
        handleGameEndsUserAction(game, gameEnds, &e);

    int winW, winH;
    SDL_GetRendererOutputSize(gameEnds->renderer, &winW, &winH);
    gameEnds->bgRect.x = 0;
    gameEnds->bgRect.y = 0;
    gameEnds->bgRect.w = winW;
    gameEnds->bgRect.h = winH;

    const int btnDesiredSize = 60;
    gameEnds->btnExit.rect.w = (gameEnds->btnExit.rect.w * btnDesiredSize) / gameEnds->btnExit.rect.h;
    gameEnds->btnExit.rect.h = btnDesiredSize;
    gameEnds->btnTryAgain.rect.w = (gameEnds->btnTryAgain.rect.w * btnDesiredSize) / gameEnds->btnTryAgain.rect.h;
    gameEnds->btnTryAgain.rect.h = btnDesiredSize;

    gameEnds->btnExit.rect.x = winW / 2 - gameEnds->btnTryAgain.rect.w / 2;
    gameEnds->btnExit.rect.y = winH / 2 + 110;
    gameEnds->btnTryAgain.rect.x = winW / 2 - gameEnds->btnTryAgain.rect.w / 2;
    gameEnds->btnTryAgain.rect.y = winH / 2 + 100 - gameEnds->btnTryAgain.rect.h;

    static char levelBuffer[32], scoreBuffer[32], linesCountBuffer[32];
    snprintf(levelBuffer, sizeof(levelBuffer), "Niveau %d", game->level);
    snprintf(scoreBuffer, sizeof(scoreBuffer), "%d points", game->score);
    snprintf(linesCountBuffer, sizeof(linesCountBuffer), "%d lignes", game->linesCount);
    initLabel(&gameEnds->time, msTimeToString(game->totalPlayTime), gameEnds->renderer, gameEnds->smallFont, WHITE);
    initLabel(&gameEnds->score, scoreBuffer, gameEnds->renderer, gameEnds->smallFont, WHITE);
    initLabel(&gameEnds->level, levelBuffer, gameEnds->renderer, gameEnds->smallFont, WHITE);
    initLabel(&gameEnds->linesCount, linesCountBuffer, gameEnds->renderer, gameEnds->smallFont, WHITE);

    gameEnds->time.rect.x = winW / 2 - gameEnds->time.rect.w / 2;
    gameEnds->level.rect.x = winW / 2 - gameEnds->level.rect.w / 2;
    gameEnds->score.rect.x = winW / 2 - gameEnds->score.rect.w / 2;
    gameEnds->linesCount.rect.x = winW / 2 - gameEnds->linesCount.rect.w / 2;

    gameEnds->score.rect.y = gameEnds->btnTryAgain.rect.y - gameEnds->score.rect.h - 15;
    gameEnds->linesCount.rect.y = gameEnds->score.rect.y - gameEnds->linesCount.rect.h - 5;
    gameEnds->level.rect.y = gameEnds->linesCount.rect.y - gameEnds->level.rect.h - 5;
    gameEnds->time.rect.y = gameEnds->level.rect.y - gameEnds->time.rect.h - 5;

    SDL_RenderCopy(gameEnds->renderer, gameEnds->bgTex, NULL, &gameEnds->bgRect);
    renderButton(&gameEnds->btnTryAgain);
    renderButton(&gameEnds->btnExit);
    renderLabel(&gameEnds->score);
    renderLabel(&gameEnds->level);
    renderLabel(&gameEnds->linesCount);
    renderLabel(&gameEnds->time);
}
void destroyGameEnds(GameEnds *gameEnds)
{
    SDL_DestroyTexture(gameEnds->bgTex);
    destroyButton(&gameEnds->btnTryAgain);
    destroyButton(&gameEnds->btnExit);
}