#include "grid.h"
#include <string.h>  
#include <stdbool.h>

int grid[GRID_ROWS][GRID_COLS] = { 0 };

int clearCompleteLines(int grid[GRID_ROWS][GRID_COLS]) {
    int linesCleared = 0;
    int writeRow = GRID_ROWS - 1;

    for (int row = GRID_ROWS - 1; row >= 0; row--) {
        bool full = true;
        for (int col = 0; col < GRID_COLS; col++) {
            if (grid[row][col] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            linesCleared++;
        } else {
            if (writeRow != row) {
                memcpy(grid[writeRow], grid[row], sizeof(grid[row]));
            }
            writeRow--;
        }
    }

    for (int r = writeRow; r >= 0; r--) {
        memset(grid[r], 0, sizeof(grid[r]));
    }

    return linesCleared;
}

int getOccupiedBlockCount(void) {
    int count = 0;
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
            if (grid[r][c] != 0) count++;
        }
    }
    return count;
}
