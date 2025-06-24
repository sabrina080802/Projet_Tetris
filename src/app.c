#include "app.h"

int initApp(App *app, const char *title, int width, int height, Uint32 flags)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 0;
    }

    SDL_DisplayMode mode;
    if (SDL_GetCurrentDisplayMode(0, &mode) != 0)
    {
        fprintf(stderr, "GetDisplay error: %s\n", IMG_GetError());
        SDL_Quit();
        return 0;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        fprintf(stderr, "IMG_Init error: %s\n", IMG_GetError());
        SDL_Quit();
        return 0;
    }

    if (TTF_Init() != 0)
        return 0;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    app->music = Mix_LoadMUS(MUSIC);
    app->lineSound = Mix_LoadWAV(LINE_SOUND);

    Mix_VolumeChunk(app->lineSound, 64);
    Mix_VolumeMusic(64);
    Mix_PlayMusic(app->music, INT_MAX);
    flags |= SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    width = width <= 0 ? DEFAULT_W : width;
    height = height <= 0 ? DEFAULT_H : height;
    if (width > mode.w)
    {
        width = mode.w;
        height = mode.h;
    }

    app->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!app->window)
    {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        cleanupApp(app);
        return 0;
    }

    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!app->renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        cleanupApp(app);
        return 0;
    }

    app->smallFont = loadFont(FONT_PATH, 28);
    app->bigFont = loadFont(FONT_PATH, 48);
    app->sounds = createSound(app->renderer);

    SDL_SetWindowMinimumSize(app->window, width, height);
    return 1;
}

void cleanupApp(App *app)
{
    if (app->renderer)
        SDL_DestroyRenderer(app->renderer);
    if (app->window)
        SDL_DestroyWindow(app->window);

    Mix_FreeMusic(app->music);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}
