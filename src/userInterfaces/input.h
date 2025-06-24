#pragma once
#include "../game/gameInstance.h"
#include "components/button.h"
#include "components/sound.h"
#include <SDL2/SDL.h>

void handleUserAction(GameInstance *game, const SDL_Event *e, int *actionId, Button *btnPause, SoundComponent *sound);