#include <stdlib.h>
#include <time.h>
#include "../logic/grid.h"             
#include "../logic/pieces.h"           
#include "../ui/textures.h"            
#include <SDL2/SDL.h>                  

static int bag[7];
static int indexInBag = 0;

void shuffleBag() {
    for (int i = 0; i < 7; i++) {
        int j = rand() % 7;
        int temp = bag[i];
        bag[i] = bag[j];
        bag[j] = temp;
    }
}

void initPieceBag() {
    for (int i = 0; i < 7; i++) {
        bag[i] = i;
    }
    shuffleBag();
    indexInBag = 0;
}

int getNextPieceType() {
    if (indexInBag >= 7) {
        shuffleBag();
        indexInBag = 0;
    }
    return bag[indexInBag++];
}

void drawGhostPiece(SDL_Renderer* renderer, Tetromino* t, int grid[GRID_ROWS][GRID_COLS], int blockSize, int offsetX, int offsetY) {

    if (!ghostTexture) printf("⚠️ ghostTexture n’est pas chargée !\n");
    
    Tetromino ghost = *t;
    while (!collides(&ghost, grid)) {
        ghost.y++;
    }
    ghost.y--; 

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (ghost.shape[row][col]) {
                int x = offsetX + (ghost.x + col) * blockSize;
                int y = offsetY + (ghost.y + row) * blockSize;
                SDL_Rect dest = { x, y, blockSize, blockSize };
                SDL_RenderCopy(renderer, ghostTexture, NULL, &dest);
            }
        }
    }
}
