#pragma once
#include <stdlib.h>
#include <stdbool.h>

extern const char tetrominoShapes[7][4][4];

char **getShape(int shapeId);
int getShapeWidth(char **shape);
int getShapeHeight(char **shape);
char **getShapeAsGhost(int shapeId);