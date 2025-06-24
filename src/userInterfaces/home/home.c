#include "home.h"

int showHome(App *app)
{
    Home home = createHome(app);
    renderHome(&home);
    destroyHome(&home);

    return home.choosedMode;
}
Home createHome(App *app)
{
    SDL_Renderer *r = app->renderer;
    Home home = {
        .app = app,
        .bgTex = loadTexture(r, HOME_BG),
        .logoTex = loadTexture(r, HOME_LOGO),
        .btnMarathon = createButton(r, HOME_BTN_MARATHON),
        .btnDuel = createButton(r, HOME_BTN_DUEL),
        .btnExit = createButton(r, QUITTER),
        .bgRect = {0, 0, 0, 0},
        .logoRect = {0, 80, 0, 0},
        .choosedMode = -1};

    return home;
}
void updateHome(Home *home)
{
    const int spacing = 40;

    int winW, winH;
    SDL_GetRendererOutputSize(home->app->renderer, &winW, &winH);

    SDL_QueryTexture(home->logoTex, NULL, NULL, &home->logoRect.w, &home->logoRect.h);
    home->bgRect.w = winW;
    home->bgRect.h = winH;
    home->logoRect.x = (winW - home->logoRect.w) / 2;
    home->logoRect.y = spacing * 2;

    const int logoBottom = home->logoRect.y + home->logoRect.h;
    const int startY = logoBottom + 50;

    home->btnMarathon.rect.x = (winW - home->btnMarathon.rect.w) / 2;
    home->btnMarathon.rect.y = startY;

    home->btnDuel.rect.x = (winW - home->btnDuel.rect.w) / 2;
    home->btnDuel.rect.y = home->btnMarathon.rect.y + home->btnMarathon.rect.h + spacing;

    home->btnExit.rect.x = (winW - home->btnExit.rect.w) / 2;
    home->btnExit.rect.y = home->btnDuel.rect.y + home->btnDuel.rect.h + spacing;
}
void handleHomeEvent(Home *home, const SDL_Event *e)
{
    if (e->type == SDL_QUIT || (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE))
    {
        home->choosedMode = 0;
    }
    else if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        int mx = e->button.x, my = e->button.y;

        if (tryToggleSound(mx, my, &home->app->sounds))
            return;
        else if (containsPoint(mx, my, home->btnExit.rect))
            home->choosedMode = 0;
        else if (containsPoint(mx, my, home->btnMarathon.rect))
            home->choosedMode = 1;
        else if (containsPoint(mx, my, home->btnDuel.rect))
            home->choosedMode = 2;
    }
}
void renderHome(Home *home)
{
    SDL_Renderer *r = home->app->renderer;
    SDL_Event e;

    while (home->choosedMode == -1)
    {
        while (SDL_PollEvent(&e))
            handleHomeEvent(home, &e);

        updateHome(home);

        SDL_RenderClear(r);
        SDL_RenderCopy(r, home->bgTex, NULL, &home->bgRect);
        SDL_RenderCopy(r, home->logoTex, NULL, &home->logoRect);

        renderButton(&home->btnMarathon);
        renderButton(&home->btnDuel);
        renderButton(&home->btnExit);

        renderSound(&home->app->sounds);

        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
}

void destroyHome(Home *home)
{
    SDL_DestroyTexture(home->bgTex);
    SDL_DestroyTexture(home->logoTex);
    destroyButton(&home->btnMarathon);
    destroyButton(&home->btnDuel);
    destroyButton(&home->btnExit);
}