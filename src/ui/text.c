#include "text.h"

SDL_Texture *renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color, int *w, int *h)
{
    if (!text || strlen(text) == 0)
    {
        text = " "; // Empêche les crashs liés à un texte vide
    }

    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(font, text, color, 400);
    if (!surface)
    {
        fprintf(stderr, "Erreur TTF : %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        fprintf(stderr, "Erreur texture : %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return NULL;
    }

    if (w)
        *w = surface->w;
    if (h)
        *h = surface->h;

    SDL_FreeSurface(surface);
    return texture;
}

TTF_Font *loadFont(const char *path, int size)
{
    TTF_Font *font = TTF_OpenFont(path, size);
    if (!font)
    {
        fprintf(stderr, "Erreur chargement police %s : %s\n", path, TTF_GetError());
    }
    return font;
}
