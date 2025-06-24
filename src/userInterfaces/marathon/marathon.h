#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "../../app.h"
#include "../textures.h"
#include "../ressources.h"
#include "../components/button.h"
#include "../components/text.h"
#include "../gameComponents/pieceStats.h"
#include "../gameComponents/gridRenderer.h"
#include "../../game/gameInstance.h"
#include "../../game/grid.h"
#include "../../game/gameState.h"
#include "../../values/userActions.h"
#include "../input.h"
#include "../modal/gamePaused.h"
#include "../modal/gameEnds.h"

typedef struct
{
    App *app;
    SDL_Texture *bgTex;
    SDL_Rect bgRect;

    Frame gridFrame;
    GridRenderer grid;
    GameInstance game;

    Frame nextPieceGridFrame;
    GridRenderer nextPieceGrid;
    PieceStats pieceStats;

    Button btnPause;
    Label gameMode;
    Label currentScore;
    Label currentLinesCount;
    Label currentLevel;
    Label time;
} Marathon;

void showMarathon(App *app);
Marathon createMarathon(App *app);
void updateMarathon(Marathon *marathon);
void updateMarathonScoreLabels(Marathon *marathon);
void renderMarathon(Marathon *marathon);
void destroyMarathon(Marathon *marathon);
