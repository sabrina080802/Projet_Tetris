#pragma once

#include <SDL2/SDL.h>
#include "../logic/grid.h"
#include "../logic/pieces.h"
#include "../ui/textures.h"
#include "../ui/text.h"
#include "../ui/render.h"
#include "../ui/ressources.h"
#include "../views/gamemode_internal.h"
#include "../screens/pause.h"
#include "../screens/duel_gameover.h"
#include "../logic/ia.h"
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int showDuel(SDL_Renderer *renderer);
