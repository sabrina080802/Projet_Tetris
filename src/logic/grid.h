#pragma once
#include <string.h>
#include <stdbool.h>

#define GRID_ROWS 20
#define GRID_COLS 10

extern int grid[GRID_ROWS][GRID_COLS];

int clearCompleteLines(int grid[GRID_ROWS][GRID_COLS]);
int getOccupiedBlockCount(void);
