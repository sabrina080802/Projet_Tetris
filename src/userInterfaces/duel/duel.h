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
#include "../../game/ai.h"
#include "../../values/userActions.h"
#include "../input.h"
#include "../modal/gameEnds.h"
#include "../modal/gamePaused.h"

typedef struct
{
    App *app;
    SDL_Texture *bgTex;
    SDL_Rect bgRect;

    Frame p1GridFrame, p2GridFrame;
    GridRenderer p1Grid, p2Grid;
    GameInstance p1Game, p2Game;

    Frame p1NextPieceFrame, p2NextPieceFrame;
    GridRenderer p1NextPieceGrid, p2NextPieceGrid;
    PieceStats p1PieceStats, p2PieceStats;

    Button btnPause;
    Label gameMode;
    Label p1Score, p2Score;
    Label p1LinesCount, p2LinesCount;
    Label p1Level, p2Level;
    Label time;
} Duel;

void showDuel(App *app);
Duel createDuel(App *app);
void updateDuel(Duel *duel);
void updateDuelScoreLabels(Duel *duel);
void updateDuelGridFrame(GridRenderer *gridRenderer, Frame *frame);

void renderDuel(Duel *duel);
void destroyDuel(Duel *duel);
