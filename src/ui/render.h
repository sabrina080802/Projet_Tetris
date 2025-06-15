#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../logic/pieces.h"
#include "../ui/textures.h"
#include "../logic/grid.h"
#include "text.h"

void drawTetromino(SDL_Renderer *renderer, Tetromino *t, SDL_Texture *textures[], int blockSize, int offsetX, int offsetY);
void drawGrid(SDL_Renderer *renderer, SDL_Texture *textures[], int blockSize, int offsetX, int offsetY);
void drawGhostPiece(SDL_Renderer *renderer, Tetromino *t, int grid[GRID_ROWS][GRID_COLS], int blockSize, int offsetX, int offsetY);
void drawPieceStats(SDL_Renderer *renderer, SDL_Texture *textures[], int blockSizeMax, int offsetX, int offsetY, TTF_Font *font);
