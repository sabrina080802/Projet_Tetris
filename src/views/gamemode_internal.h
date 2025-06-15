#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <time.h>
#include "../logic/grid.h"
#include "../logic/pieces.h"
#include "../ui/textures.h"

void initPieceBag();
void shuffleBag();
int getNextPieceType();
