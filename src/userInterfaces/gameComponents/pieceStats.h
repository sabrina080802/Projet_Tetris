#pragma once
#include <SDL2/SDL.h>
#include <string.h>
#include "../../values/shapes.h"
#include "../components/frame.h"
#include "../components/text.h"
#include "../styles.h"
#include "../textures.h"
#include "../fonts.h"
#include "gridRenderer.h"

typedef struct
{
    SDL_Renderer *renderer;
    SDL_Rect rect;
    Frame bgFrame;
    GridRenderer grid;
    Label labels[7];
    TTF_Font *font;
} PieceStats;

PieceStats createPieceStats(SDL_Renderer *r, TTF_Font *font);
void renderPieceStats(PieceStats *stats, int pieceCount[7]);
void destroyPieceStats(PieceStats *stats);