#pragma once

#include <SDL2/SDL.h>
extern Uint32 startTime;

void resetTimer(void);
Uint32 getElapsedTime(void);
void updateTimerFromLevel(int level);
