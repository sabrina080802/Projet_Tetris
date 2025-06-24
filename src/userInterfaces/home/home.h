#pragma once
#include "../../app.h"
#include "../ressources.h"
#include "../components/button.h"
#include "../textures.h"
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
void updateHome(Home *home);
void handleHomeEvent(Home *home, const SDL_Event *e);
void renderHome(Home *home);
void destroyHome(Home *home);