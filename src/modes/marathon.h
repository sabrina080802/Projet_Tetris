#pragma once

#include <SDL2/SDL.h>

#include "../logic/pieces.h"
#include "../views/gamemode_internal.h"
#include "../ui/textures.h"
#include "../logic/score.h"
#include "../logic/timer.h"

int showMarathon(SDL_Renderer *renderer);
void initMarathonMode(void);
