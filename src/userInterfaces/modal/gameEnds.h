#pragma once
#include "../textures.h"
#include "../components/button.h"
#include "../components/text.h"
#include "../../game/gameInstance.h"
#include "../../app.h"
#include <SDL2/SDL.h>

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Texture *bgTex;
    SDL_Rect bgRect;
    Button btnTryAgain;
    Button btnExit;
    GameInstance *game;
    TTF_Font *smallFont;

    Label score;
    Label linesCount;
    Label time;
    Label level;
} GameEnds;

GameEnds createGameEnds(App *app, GameInstance *gameInstance);
void renderGameEnds(GameInstance *game, GameEnds *gameEnds);
void destroyGameEnds(GameEnds *gameEnds);
void handleGameEndsUserAction(GameInstance *gameInstance, GameEnds *gameEnds, const SDL_Event *e);