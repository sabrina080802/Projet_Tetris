#pragma once

extern int score;
extern int level;
extern int linesCleared;

void resetScore(void);
void resetLevel(void);
void resetLinesCleared(void);
void addScore(int n);
int getScore(void);
int getLinesCleared(void);
