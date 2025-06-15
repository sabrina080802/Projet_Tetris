#include "home.h"

int showHome(App *app)
{
    Home home = createHome(app);
    renderHome(&home);
    destroyHome(home);

    return home.choosedMode;
}
Home createHome(App *app)
{
    int winW, winH;
    SDL_GetRendererOutputSize(app->renderer, &winW, &winH);

    SDL_Renderer *r = app->renderer;
    Home home = {
        app,
        loadTexture(r, HOME_BG),
        loadTexture(r, HOME_LOGO),

        createButton(r, HOME_BTN_MARATHON),
        createButton(r, HOME_BTN_DUEL),
        createButton(r, QUITTER),

        {0, 0, winW, winH},
        {0, 80, 0, 0},
        -1};

    SDL_QueryTexture(home.logoTex, NULL, NULL, &home.logoRect.w, &home.logoRect.h);
    home.logoRect.x = (winW - home.logoRect.w) / 2;

    const int spacing = 40;
    const int totalH = home.btnMarathon.rect.h + home.btnMarathon.rect.h + home.btnExit.rect.h + spacing * 3;
    const int startY = (winH - totalH) / 2 + 40;

    home.btnMarathon.rect.x = (winW - home.btnMarathon.rect.w) / 2;
    home.btnMarathon.rect.y = startY;

    home.btnDuel.rect.x = (winW - home.btnDuel.rect.w) / 2;
    home.btnDuel.rect.y = startY;

    home.btnExit.rect.x = (winW - home.btnExit.rect.w) / 2;
    home.btnExit.rect.y;

    return home;
}

void renderHome(Home *home)
{
    SDL_Renderer *r = home->app->renderer;
    SDL_Event e;

    while (home->choosedMode == -1)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
            {
                home->choosedMode = 0;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mx = e.button.x, my = e.button.y;

                if (tryToggleSound(mx, my, &home->app->sounds))
                    continue;
                else if (containsPoint(mx, my, home->btnMarathon.rect))
                    home->choosedMode = 1;
                else if (containsPoint(mx, my, home->btnDuel.rect))
                    home->choosedMode = 2;
                else if (containsPoint(mx, my, home->btnExit.rect))
                    home->choosedMode = 0;
            }
        }

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

void destroyHome(Home home)
{
    SDL_DestroyTexture(home->bgTex);
    SDL_DestroyTexture(home->logoTex);
    SDL_DestroyTexture(home->btnMarathon.texture);
    SDL_DestroyTexture(home->btnDuel.texture);
    SDL_DestroyTexture(home->btnExit.texture);
}