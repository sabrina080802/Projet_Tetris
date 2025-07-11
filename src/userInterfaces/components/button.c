#include "button.h"

Button createButton(SDL_Renderer *r, const char *tex_path)
{
    Button btn = {
        .renderer = r,
        .texture = loadTexture(r, tex_path),
        .rect = {0, 0, 0, 0}};
    SDL_QueryTexture(btn.texture, NULL, NULL, &btn.rect.w, &btn.rect.h);
    return btn;
}

bool containsPoint(int x, int y, SDL_Rect rect)
{
    return (x >= rect.x && y >= rect.y &&
            x <= rect.x + rect.w && y <= rect.y + rect.h);
}
void renderButton(Button *btn)
{
    SDL_RenderCopy(btn->renderer, btn->texture, NULL, &btn->rect);
}
void destroyButton(Button *btn)
{
    SDL_DestroyTexture(btn->texture);
}