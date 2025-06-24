#include "styles.h"

const int STATS_PIECE_SIZE = 25;

const SDL_Color WHITE = {255, 255, 255, 255};
const SDL_Color VIOLET_FONCE = {37, 17, 66, 127};
const SDL_Color VIOLET_BORD = {137, 64, 247, 255};

float lerp(float a, float b, float delta)
{
    return a + (b - a) * delta;
}