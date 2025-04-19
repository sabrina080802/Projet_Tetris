#ifndef PIECES_H
#define PIECES_H
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "grid.h"


#define NUM_TETROMINOS 7

typedef struct {
    int x, y;
    int type;
    int rotation; 
    int shape[4][4];
} Tetromino;

extern int pieceCount[7];
extern const int PIECE_SHAPES[7][4][4];
extern SDL_Texture* blockTextures[7];  

void initPiecesTextures(SDL_Renderer* renderer);
bool collides(Tetromino* t, int grid[GRID_ROWS][GRID_COLS]);

Tetromino createTetromino(int type);
void drawTetromino(SDL_Renderer* renderer, Tetromino* t, SDL_Texture* textures[], int blockSize, int offsetX, int offsetY);
void rotateTetromino(Tetromino* t);
void drawGrid(SDL_Renderer* renderer, SDL_Texture* textures[], int blockSize, int offsetX, int offsetY);
void drawPieceStats(SDL_Renderer* renderer, SDL_Texture* textures[], int blockSize, int offsetX, int offsetY, TTF_Font* font);

#endif
