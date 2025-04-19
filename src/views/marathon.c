#include "gamemode.h"
#include "../logic/pieces.h"
#include "gamemode_internal.h"  


void showMarathon(SDL_Renderer* renderer) {
    GameModeInfo marathon = {
        .modeName = "Marathon",
        .bestScore = "Meilleur score\r1465 lignes\r52 min, 34s\r725646 pts",
        .scoreText = "Score\n001424",
        .linesText = "Nbre lignes\n00001",
        .nextText = "Suivant"
    };

    initPiecesTextures(renderer);                         
    Tetromino current = createTetromino(getNextPieceType());
    Tetromino next = createTetromino(getNextPieceType());
    pieceCount[current.type]++;
    current.y = 5; 
    showGameMode(renderer, marathon, &current, &next, blockTextures);
}
