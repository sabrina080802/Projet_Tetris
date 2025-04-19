#include "gamemode.h"
#include "../logic/pieces.h"
#include "gamemode_internal.h"



void showDuel(SDL_Renderer* renderer) {
    GameModeInfo duel = {
        .modeName = "Duel",
        .bestScore = "Dernier duel :\rJoueur 1: 842 lignes\rJoueur 2: 670 lignes",
        .scoreText = "Score\n002450",
        .linesText = "Lignes\n00084",
        .nextText = "Prochain adversaire"
    };

    initPiecesTextures(renderer);                         
    Tetromino current = createTetromino(rand() % 7);
    Tetromino next = createTetromino(rand() % 7); 
    pieceCount[current.type]++;
    current.y = 5; 
    showGameMode(renderer, duel, &current, &next, blockTextures);
}
