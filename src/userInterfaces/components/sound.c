#include "sound.h"

SoundComponent createSound(SDL_Renderer *r)
{
    SoundComponent sound = {
        .renderer = r,
        .textureOn = loadTexture(r, ICON_MUSIC_ON),
        .textureOff = loadTexture(r, ICON_MUSIC_OFF),
        .isActive = true};

    SDL_QueryTexture(sound.textureOn, NULL, NULL, &sound.rect.w, &sound.rect.h);
    sound.rect.y = 20;
    sound.rect.w = (SOUND_HEIGHT * sound.rect.w) / sound.rect.h;
    sound.rect.h = SOUND_HEIGHT;
    return sound;
}
bool tryToggleSound(int x, int y, SoundComponent *sound)
{
    if (containsPoint(x, y, sound->rect))
    {
        sound->isActive = !sound->isActive;
        if (sound->isActive)
        {
            Mix_ResumeMusic();
        }
        else
        {
            Mix_PauseMusic();
        }

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
    SDL_RenderCopy(sound->renderer, sound->isActive ? sound->textureOn : sound->textureOff, NULL, &sound->rect);
}