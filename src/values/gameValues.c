#include "gameValues.h"

const float FPS = 60.0f;
const float FRAME_TIME_MS = 1000.0f / 60;
const float FRAME_TIME_SEC = (1000.0f / 60) / 1000.0f;
const float PLAY_TIMER = ((1000.0f / 60) / 1000.0f) * 10;

const int framesToFall[30] = {
    48, 43, 38, 33, 28, 23, 18, 13, 8, 6,
    5, 5, 5, 4, 4, 4, 3, 3, 3, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 1};