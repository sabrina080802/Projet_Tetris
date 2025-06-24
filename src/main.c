#include <stdio.h>
#include "app.h"
#include "userInterfaces/splash/splash.h"
#include "userInterfaces/home/home.h"
#include "userInterfaces/marathon/marathon.h"
#include "userInterfaces/duel/duel.h"

int main(int argc, char *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    App app;
    if (!initApp(&app, "Tetris SDL", 1920, 1080, SDL_WINDOW_FULLSCREEN))
        return 1;

    initPiecesTextures(app.renderer);
    showSplash(app.renderer);

    int running = 1;
    while (running)
    {
        int mode = showHome(&app);
        switch (mode)
        {
        case 0:
            running = 0;
            break;
        case 1:
            showMarathon(&app);
            break;
        case 2:
            showDuel(&app);
            break;
        }
    }

    cleanupApp(&app);
    return 0;
}
