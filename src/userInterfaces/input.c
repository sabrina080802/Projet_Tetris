#include "input.h"

void handleUserAction(GameInstance *game, const SDL_Event *e, int *actionId, Button *btnPause, SoundComponent *sound)
{
    if (e->type == SDL_QUIT)
    {
        game->running = false;
        *actionId = USER_QUIT;
    }
    else if (e->type == SDL_KEYDOWN)
    {
        switch (e->key.keysym.sym)
        {
        case SDLK_ESCAPE:
            *actionId = USER_PAUSED;
            game->paused = true;
            break;
        case SDLK_LEFT:
        case SDLK_q:
            *actionId = USER_GO_LEFT;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            *actionId = USER_GO_RIGHT;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            *actionId = USER_ACCELERATE;
            break;
        }
    }
    else if (e->type == SDL_KEYUP)
    {
        if (e->key.keysym.sym == SDLK_r || e->key.keysym.sym == SDLK_UP)
            *actionId = USER_ROTATE;
        else
            *actionId = STOP_ACTION;
    }
    else if (e->type == SDL_MOUSEBUTTONUP)
    {
        if (tryToggleSound(e->button.x, e->button.y, sound))
            return;
        else if (containsPoint(e->button.x, e->button.y, btnPause->rect))
        {
            *actionId = USER_PAUSED;
            game->paused = true;
        }
    }
}