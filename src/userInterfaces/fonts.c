#include "fonts.h"

TTF_Font *loadFont(const char *path, int size)
{
    TTF_Font *font = TTF_OpenFont(path, size);
    if (!font)
    {
        fprintf(stderr, "Erreur chargement police %s : %s\n", path, TTF_GetError());
    }
    return font;
}
