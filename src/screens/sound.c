#include "sound.h"

SoundComponent createSound(SDL_Renderer *r)
{
    SoundComponent sound = {
        r,
        {0, 0, 0, 0},
        loadTexture(r, ICON_MUSIC_ON),
        loadTexture(r, ICON_MUSIC_OFF),
        true,
    };

    SDL_QueryTexture(sound.textureOn, NULL, NULL, &sound.rect.w, &sound.rect.h);
    sound.rect.y = 20;
    return sound;
}
bool tryToggleSound(int x, int y, SoundComponent *sound)
{
    if (containsPoint(x, y, sound->rect))
    {
        sound->isActive = !sound->isActive;
        return true;
    }
    else
        return false;
}

void renderSound(SoundComponent *sound)
{
    int winW, winH;
    SDL_GetRendererOutputSize(sound->renderer, &winW, &winH);
    sound->rect.x = winW - sound->rect.w - 20;

    SDL_RenderCopy(sound->renderer, sound->isActive ? sound->textureOn : sound->textureOn, NULL, &sound->rect);
}