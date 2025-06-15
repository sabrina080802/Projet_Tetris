#include "gamemode.h"
#include "marathon.h"

int showMarathon(SDL_Renderer *renderer)
{
    GameModeInfo marathon = {
        .modeName = "Marathon",
        .nextText = "Suivant"};

    snprintf(marathon.scoreText, sizeof(marathon.scoreText), "Score\n000000");
    snprintf(marathon.linesText, sizeof(marathon.linesText), "Lignes\n00000");

    initPiecesTextures(renderer);
    Tetromino current = createTetromino(getNextPieceType());
    Tetromino next = createTetromino(getNextPieceType());
    spawnedPieceCount[current.type]++;
    current.y = 5;

    return showGameMode(renderer, marathon, &current, &next, blockTextures);
}

void initMarathonMode(void)
{
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
