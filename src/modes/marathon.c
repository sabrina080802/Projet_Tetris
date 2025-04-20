#include "gamemode.h"
#include "../logic/pieces.h"
#include "../views/gamemode_internal.h"
#include "../ui/textures.h"
#include "../logic/score.h"
#include "../logic/timer.h"



int showMarathon(SDL_Renderer* renderer) {
    GameModeInfo marathon = {
        .modeName = "Marathon",
        .nextText = "Suivant"
    };

    snprintf(marathon.scoreText, sizeof(marathon.scoreText), "Score\n000000");
    snprintf(marathon.linesText, sizeof(marathon.linesText), "Lignes\n00000");

    initPiecesTextures(renderer);                         
    Tetromino current = createTetromino(getNextPieceType());
    Tetromino next = createTetromino(getNextPieceType());
    pieceCount[current.type]++;
    current.y = 5;

    return showGameMode(renderer, marathon, &current, &next, blockTextures);
}

void initMarathonMode(void) {
    memset(grid, 0, sizeof(grid));

    initPieceBag();
    Tetromino first; 
    first.type = getNextPieceType();
    first = createTetromino(first.type);
    spawnPiece(&first);

    resetScore();
    resetLevel();
    resetLinesCleared();

    resetTimer();
}

