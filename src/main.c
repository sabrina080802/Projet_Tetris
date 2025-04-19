#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h> 

#include "app.h"
#include "views/splash.h"
#include "logic/pieces.h"
#include "views/home.h"
#include "views/gamemode_internal.h" 

int main(int argc, char* argv[]) {
    srand(time(NULL));     
    initPieceBag();         

    App app;
    if (!initApp(&app, "Tetris SDL", 1871, 1000, SDL_WINDOW_FULLSCREEN))
        return 1;

    showSplash(app.renderer);
    showHome(app.renderer);

    cleanupApp(&app);
    return 0;
}
