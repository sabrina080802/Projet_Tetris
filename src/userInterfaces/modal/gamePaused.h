#pragma once
#include "../textures.h"
#include "../components/button.h"
#include "../../game/gameInstance.h"
#include "../../app.h"
#include <SDL2/SDL.h>

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Texture *bgTex;
    SDL_Rect bgRect;
    Button btnContinue;
    Button btnExit;
    GameInstance *game;
} GamePaused;

GamePaused createGamePaused(App *app, GameInstance *gameInstance);
void renderGamePaused(GameInstance *game, GamePaused *gamePaused);
void destroyGamePaused(GamePaused *gameEnds);
void handleGamePauseUserAction(GameInstance *gameInstance, GamePaused *gamePaused, const SDL_Event *e);