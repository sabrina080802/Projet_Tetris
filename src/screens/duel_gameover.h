#pragma once
#include <SDL2/SDL.h>
#include "../ui/text.h"
#include "../ui/textures.h"
#include "../ui/ressources.h"
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

int showDuelGameOver(SDL_Renderer *renderer, int scoreP1, int linesP1, int scoreP2, int linesP2);
