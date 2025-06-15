#include "app.h"
#include "screens/splash.h"
#include "logic/pieces.h"
#include "screens/home.h"
#include "views/gamemode_internal.h"
#include "modes/marathon.h"
#include "modes/duel.h"
#include <stdio.h>

int main()
{
    App app;
    if (!initApp(&app, "Tetris SDL", 1497, 1000, 0))
        return 1;

    showSplash(app.renderer);

    initPieceBag();
    initPiecesTextures(app.renderer);

    int running = 1;
    while (running)
    {
        int mode = showHome(app.renderer);
        switch (mode)
        {
        case 0:
            running = 0;
            break;
        case 1:
            showMarathon(app.renderer);
            break;
        case 2:
            showDuel(app.renderer);
            break;
        }
    }

    cleanupApp(&app);
    return 0;
}
