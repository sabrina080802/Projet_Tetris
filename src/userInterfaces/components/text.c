#include "text.h"

void initLabel(Label *label, const char *text, SDL_Renderer *renderer, TTF_Font *font, SDL_Color color)
{
    if (label->texture)
        SDL_DestroyTexture(label->texture);
    if (!text || strlen(text) == 0)
        text = " ";

    SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(
        font, text, color, 400);

    label->renderer = renderer;
    label->text = text;
    label->texture = SDL_CreateTextureFromSurface(renderer, surface);
    label->rect.w = surface->w;
    label->rect.h = surface->h;
    SDL_FreeSurface(surface);
}
void renderLabel(Label *label)
{
    SDL_RenderCopy(label->renderer, label->texture, NULL, &label->rect);
}
void destroyLabel(Label *label)
{
    if (label->texture)
        SDL_DestroyTexture(label->texture);
}

const char *msTimeToString(float msTime)
{
    static char buffer[32];
    int totalSeconds = (int)(msTime / 1000.0f);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    snprintf(buffer, sizeof(buffer), "Temps : %d min %02ds", minutes, seconds);
    return buffer;
}