#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <time.h> 
#include "app.h"
#include "screens/splash.h"
#include "logic/pieces.h"
#include "screens/home.h"
#include "views/gamemode_internal.h" 
#include "modes/marathon.h"
#include "modes/duel.h"
#include "modes/classique.h"
#include "ui/ressources.h"
#include "ui/textures.h" 

int main(int argc, char* argv[]) {
    srand(time(NULL));
    initPieceBag();

    App app;
    if (!initApp(&app, "Tetris SDL", 1871, 1000, SDL_WINDOW_FULLSCREEN))
        return 1;

    initPiecesTextures(app.renderer);
    ghostTexture = loadTexture(app.renderer, GHOST_PIECE);  

    showSplash(app.renderer);

    int running = 1;
    while (running) {
        int mode = showHome(app.renderer);
        switch (mode) {
            case 0: running = 0; break;
            case 1: showMarathon(app.renderer); break;
            case 2: showClassique(app.renderer); break;
            case 3: showDuel(app.renderer); break;
        }
    }
    cleanupApp(&app);
    return 0;
}
