#include "grid.h"
#include <string.h>  // pour memcpy et memset
#include <stdbool.h>

int grid[GRID_ROWS][GRID_COLS] = { 0 };

int clearCompleteLines(int grid[GRID_ROWS][GRID_COLS]) {
    int linesCleared = 0;
    int writeRow = GRID_ROWS - 1;

    // On parcourt toutes les lignes du bas vers le haut
    for (int row = GRID_ROWS - 1; row >= 0; row--) {
        bool full = true;
        // Vérifier si la ligne est pleine
        for (int col = 0; col < GRID_COLS; col++) {
            if (grid[row][col] == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            // On compte la ligne pleine, mais on ne décale pas tout de suite
            linesCleared++;
        } else {
            // On recopie la ligne non-pleine vers la position writeRow
            if (writeRow != row) {
                memcpy(grid[writeRow], grid[row], sizeof(grid[row]));
            }
            writeRow--;
        }
    }

    // On remplit toutes les lignes restantes en haut avec des zéros
    for (int r = writeRow; r >= 0; r--) {
        memset(grid[r], 0, sizeof(grid[r]));
    }

    return linesCleared;
}
