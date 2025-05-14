#include "score.h"

int score        = 0;
int level        = 1;
int linesCleared = 0;

static const int basePoints[5] = {
    0,    // 0 lignes
    40,   // 1 ligne
    100,  // 2 lignes
    300,  // 3 lignes
    1200  // 4 lignes (Tetris)
};

void resetScore(void) {
    score = 0;
}

void resetLevel(void) {
    level = 1;
}

void resetLinesCleared(void) {
    linesCleared = 0;
}

void addScore(int n) {
    if (n < 1 || n > 4) return;            
    score += basePoints[n] * level;
    linesCleared += n;
    level = (linesCleared / 10) + 1;
}
