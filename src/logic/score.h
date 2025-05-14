#ifndef SCORE_H
#define SCORE_H

extern int score;
extern int level;
extern int linesCleared;

void resetScore(void);
void resetLevel(void);
void resetLinesCleared(void);
void addScore(int n);

#endif
