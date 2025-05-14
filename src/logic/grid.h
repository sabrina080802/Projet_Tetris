#ifndef GRID_H
#define GRID_H

#define GRID_ROWS 20
#define GRID_COLS 10

extern int grid[GRID_ROWS][GRID_COLS];

int clearCompleteLines(int grid[GRID_ROWS][GRID_COLS]);
int getOccupiedBlockCount(void);


#endif
