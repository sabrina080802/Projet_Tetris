#include "timer.h"

Uint32 startTime = 0;

void resetTimer(void) {
    startTime = SDL_GetTicks();
}

Uint32 getElapsedTime(void) {
    return SDL_GetTicks() - startTime;
}
