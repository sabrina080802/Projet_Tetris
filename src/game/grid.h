#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../values/shapes.h"

char **createGrid(int rows, int cols);
char **copyGrid(char **grid, int rows, int cols);
void resetGrid(char **grid, int rows, int cols);
void freeGrid(char **grid, int rows, int cols);
void applyGrid(char **grid, char **newGrid, int rows, int cols);
void applyShape(char **shape, int shapeRow, int shapeCol, char **grid, int gridRows, int gridCols);

void showGhost(char **grid, char **renderingGrid, char **shape, int shapeRow, int shapeCol, int gridRows, int gridCols);
int getLowestNoCollidesRow(char **shape, int startRow, int startCol, char **grid, int rows, int cols);
bool collides(char **shape, int shapeRow, int shapeCol, char **grid, int gridRows, int gridCols);
void addGhostLine(char **grid, int rows, int cols);

void rotateShape(char **shape);
int getCompletedLinesCount(char **grid, int rows, int cols);
int getRoughness(char **grid, int rows, int cols);