#include "textures.h"
#include "ressources.h"
#include <SDL2/SDL_image.h>

SDL_Texture* blockTextures[7];
SDL_Texture* ghostTexture = NULL;  

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Surface* surf = IMG_Load(path);
    if (!surf) {
        fprintf(stderr, "Erreur chargement %s : %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    return tex;
}

void initPiecesTextures(SDL_Renderer* renderer) {
    blockTextures[0] = loadTexture(renderer, PIECE_I);
    blockTextures[1] = loadTexture(renderer, PIECE_O);
    blockTextures[2] = loadTexture(renderer, PIECE_J);
    blockTextures[3] = loadTexture(renderer, PIECE_S);
    blockTextures[4] = loadTexture(renderer, PIECE_Z);
    blockTextures[5] = loadTexture(renderer, PIECE_L);
    blockTextures[6] = loadTexture(renderer, PIECE_T);
    ghostTexture = loadTexture(renderer, GHOST_PIECE); 
    if (!ghostTexture)   fprintf(stderr, "Erreur chargement ghostTexture !\n");
   
}
