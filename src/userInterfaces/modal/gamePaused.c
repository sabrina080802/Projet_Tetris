#include "gamePaused.h"

GamePaused createGamePaused(App *app, GameInstance *game)
{
    GamePaused gamePaused = {
        .renderer = app->renderer,
        .bgTex = loadTexture(app->renderer, BG_PAUSE),
        .btnContinue = createButton(app->renderer, BTN_CONTINUER),
        .btnExit = createButton(app->renderer, BTN_QUITTER),
    };

    return gamePaused;
}
void handleGamePauseUserAction(GameInstance *gameInstance, GamePaused *gamePaused, const SDL_Event *e)
{
    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE)
    {
        gameInstance->paused = false;
        return;
    }

    if (e->type != SDL_MOUSEBUTTONUP)
        return;

    int mx = e->button.x;
    int my = e->button.y;
    if (containsPoint(mx, my, gamePaused->btnExit.rect))
    {
        gameInstance->running = false;
        gameInstance->paused = false;
    }
    else if (containsPoint(mx, my, gamePaused->btnContinue.rect))
    {
        gameInstance->paused = false;
    }
}
void renderGamePaused(GameInstance *game, GamePaused *gamePaused)
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
        handleGamePauseUserAction(game, gamePaused, &e);

    int winW, winH;
    SDL_GetRendererOutputSize(gamePaused->renderer, &winW, &winH);
    gamePaused->bgRect.x = 0;
    gamePaused->bgRect.y = 0;
    gamePaused->bgRect.w = winW;
    gamePaused->bgRect.h = winH;

    const int btnDesiredSize = 60;
    gamePaused->btnExit.rect.w = (gamePaused->btnExit.rect.w * btnDesiredSize) / gamePaused->btnExit.rect.h;
    gamePaused->btnContinue.rect.w = (gamePaused->btnContinue.rect.w * btnDesiredSize) / gamePaused->btnContinue.rect.h;
    gamePaused->btnExit.rect.h = btnDesiredSize;
    gamePaused->btnContinue.rect.h = btnDesiredSize;

    const int marginTop = 40;
    gamePaused->btnExit.rect.x = winW / 2 - gamePaused->btnExit.rect.w / 2;
    gamePaused->btnContinue.rect.x = winW / 2 - gamePaused->btnContinue.rect.w / 2;
    gamePaused->btnExit.rect.y = winH / 2 + marginTop;
    gamePaused->btnContinue.rect.y = winH / 2 - gamePaused->btnContinue.rect.h + marginTop;

    SDL_RenderCopy(gamePaused->renderer, gamePaused->bgTex, NULL, &gamePaused->bgRect);
    renderButton(&gamePaused->btnExit);
    renderButton(&gamePaused->btnContinue);
}
void destroyGamePaused(GamePaused *gamePaused)
{
    SDL_DestroyTexture(gamePaused->bgTex);
    destroyButton(&gamePaused->btnContinue);
    destroyButton(&gamePaused->btnExit);
}