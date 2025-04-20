#include "gamemode.h"
#include "../logic/pieces.h"
#include "../views/gamemode_internal.h"
#include "../ui/textures.h" 

int showClassique(SDL_Renderer* renderer) {
    GameModeInfo classique = {
        .modeName = "classique",
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
    return showGameMode(renderer, classique, &current, &next, blockTextures);

}
