#include "gamemode.h"
#include "../logic/pieces.h"
#include "gamemode_internal.h"  

SDL_Texture* blockTextures[7]; 

void initPiecesTextures(SDL_Renderer* renderer) {
    blockTextures[0] = loadTexture(renderer, "ressources/Pieces/barbie.png");     // I
    blockTextures[1] = loadTexture(renderer, "ressources/Pieces/bleu.png");       // O
    blockTextures[2] = loadTexture(renderer, "ressources/Pieces/rose.png");      // J
    blockTextures[3] = loadTexture(renderer, "ressources/Pieces/vert.png");       // S
    blockTextures[4] = loadTexture(renderer, "ressources/Pieces/jaune.png"); 
    blockTextures[5] = loadTexture(renderer, "ressources/Pieces/turquoise.png");      
    blockTextures[6] = loadTexture(renderer, "ressources/Pieces/violet.png");      

}

void showMarathon(SDL_Renderer* renderer) {
    GameModeInfo marathon = {
        .modeName = "Marathon",
        .bestScore = "Meilleur score\r1465 lignes\r52 min, 34s\r725646 pts",
        .scoreText = "Score\n001424",
        .linesText = "Nbre lignes\n00001",
        .nextText = "Suivant"
    };

    initPiecesTextures(renderer);                         // 🔁 Appel du chargement des textures
    Tetromino current = createTetromino(rand() % 7);      // 🔁 Création d'une pièce
    current.y = 5; 
    showGameMode(renderer, marathon, &current, blockTextures);  // 🔁 Appel complet
}
