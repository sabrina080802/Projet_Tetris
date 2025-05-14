#include "timer.h"

static const Uint32 BASE_DELAY       = 500;  
static const Uint32 SPEEDUP_PER_LVL  = 50;   
static const Uint32 MIN_DELAY        = 100;  

static Uint32 fallDelay;

void resetTimer(void) {
    fallDelay = BASE_DELAY;
}

void updateTimerFromLevel(int level) {
    Uint32 target = (level > 1)
        ? BASE_DELAY - (Uint32)(level - 1) * SPEEDUP_PER_LVL
        : BASE_DELAY;
    fallDelay = (target >= MIN_DELAY ? target : MIN_DELAY);
}

Uint32 getElapsedTime(void) {
    return fallDelay;
}
