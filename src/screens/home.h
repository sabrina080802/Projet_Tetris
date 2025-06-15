#pragma once
#include "../app.h"
#include "../ui/button.h"
#include "../modes/marathon.h"
#include "../modes/duel.h"
#include "../ui/textures.h"
#include "../ui/ressources.h"
#include <SDL2/SDL.h>

typedef struct
{
    App *app;
    SDL_Texture *bgTex;
    SDL_Texture *logoTex;
    Button btnMarathon;
    Button btnDuel;
    Button btnExit;

    SDL_Rect bgRect;
    SDL_Rect logoRect;

    int choosedMode;
} Home;

int showHome(App *app);
Home createHome(App *app);
void renderHome(Home *home);
void destroyHome(Home home);