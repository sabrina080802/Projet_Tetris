#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include "../ui/text.h"
#include "../ui/textures.h"
#include "../ui/ressources.h"

int showGameOver(SDL_Renderer *renderer, int score, int lines, int elapsedSeconds);
