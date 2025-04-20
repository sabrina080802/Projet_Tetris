#include "gamemode.h"
#include "../logic/pieces.h"
#include "../views/gamemode_internal.h"
#include "../ui/textures.h" 

int showDuel(SDL_Renderer* renderer) {
    GameModeInfo duel = {
        .modeName = "Duel",
        .bestScore = "",
        .scoreText = "",
        .linesText = "",
        .nextText = "Suivant"
    };

    initPiecesTextures(renderer);                         
    Tetromino current = createTetromino(rand() % 7);
    Tetromino next = createTetromino(rand() % 7); 
    pieceCount[current.type]++;
    current.y = 5; 
    return showGameMode(renderer, duel, &current, &next, blockTextures);

}
