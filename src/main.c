#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "app.h"
#include "views/splash.h"
#include "views/home.h"

int main(int argc, char* argv[]) {
    App app;
    if (!initApp(&app, "Tetris SDL", 1871, 1000, SDL_WINDOW_FULLSCREEN))
        return 1;
    showSplash(app.renderer);
    showHome(app.renderer);
    cleanupApp(&app);
    return 0;
}
