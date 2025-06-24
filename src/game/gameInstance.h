#pragma once
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#include "piece.h"
#include "grid.h"
#include "../values/gameValues.h"
#include "../values/shapes.h"
#include "../values/userActions.h"

typedef struct
{
    int rows;
    int cols;
    int score;
    int linesCount;
    int level;
    bool running;
    bool paused;

    Piece currentPiece;
    char nextPiece;
    float totalPlayTime;
    float downTimer;
    float playTimer;

    int piecesCounts[7];
} GameInstance;

void resetGame(GameInstance *game);
void executeGameAction(GameInstance *game, char **grid, int userActionId);
bool isTimeToDown(GameInstance *game, float deltaTime);
void spawnNextPiece(GameInstance *game);
float getTimeToFall(int level);
bool isTimeToPlay(GameInstance *game, float deltaTime);

void rotateShape(char **shape);

int dropCompletedLines(char **grid, int rows, int cols);