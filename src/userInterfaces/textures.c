#include "textures.h"

SDL_Texture *blockTextures[8];
SDL_Texture *ghostTexture;

SDL_Texture *loadTexture(SDL_Renderer *renderer, const char *path)
{
    SDL_Surface *surface = IMG_Load(path);
    if (!surface)
    {
        fprintf(stderr, "Erreur chargement %s : %s\n", path, IMG_GetError());
        return NULL;
    }
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return tex;
}

void initPiecesTextures(SDL_Renderer *renderer)
{
    const char *pieces[] = {PIECE_I, PIECE_O, PIECE_J, PIECE_S, PIECE_Z, PIECE_L, PIECE_T, GHOST_PIECE};
    for (int i = 0; i < 8; i++)
        blockTextures[i] = loadTexture(renderer, pieces[i]);

    ghostTexture = blockTextures[7];
}
