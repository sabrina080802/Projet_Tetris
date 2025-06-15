#include "gamemode.h"
#include "duel.h"

#define GRID_COLS 10
#define GRID_ROWS 20

static const SDL_Color BG = {37, 17, 66, 190};
static const SDL_Color BORD = {137, 64, 247, 255};
static const SDL_Color WHITE = {255, 255, 255, 255};

static const int BASE[5] = {0, 40, 100, 300, 1200};
#define IA_DELAY_BONUS 250
#define IA_STEP_MS 1000

extern SDL_Texture *blockTextures[8];
static void freePiecesTextures(void)
{
    for (int i = 0; i < 8; ++i)
    {
        if (blockTextures[i])
        {
            SDL_DestroyTexture(blockTextures[i]);
            blockTextures[i] = NULL;
        }
    }
}

static void addScore(Player *p, int clr)
{
    p->score += BASE[clr] * p->level;
    p->lines += clr;
    p->level = p->lines / 10 + 1;
    Uint32 t = (p->level > 1) ? 500 - (p->level - 1) * 50 : 500;
    p->delay = (t >= 100 ? t : 100) + ((p->delay > 500) ? IA_DELAY_BONUS : 0);
}

static void sendGarbage(Player *dst, int n)
{
    if (n < 1)
        return;
    if (n > 4)
        n = 4;
    for (int r = 0; r < GRID_ROWS - n; ++r)
        memcpy(dst->g[r], dst->g[r + n], sizeof dst->g[r]);
    for (int r = GRID_ROWS - n; r < GRID_ROWS; ++r)
    {
        int hole = rand() % GRID_COLS;
        for (int c = 0; c < GRID_COLS; ++c)
            dst->g[r][c] = (c == hole) ? 0 : 8;
    }
}

static void resetPlayer(Player *p, int isAI)
{
    memset(p->g, 0, sizeof p->g);
    p->cur = createTetromino(rand() % 7);
    p->nxt = createTetromino(rand() % 7);
    p->cur.x = 3;
    p->cur.y = 0;
    p->score = p->lines = 0;
    p->level = 1;
    p->delay = 500 + (isAI ? IA_DELAY_BONUS : 0);
    p->lastFall = SDL_GetTicks();
    /* IA */
    p->iaPlanned = 0;
}

static struct
{
    SDL_Texture *title;
    SDL_Texture *labScore, *labLines, *labNext;
    SDL_Texture *bg, *btnPause, *icoOn, *icoOff;
} Gfx;

static int loadStaticTextures(SDL_Renderer *r, TTF_Font *fBig, TTF_Font *fSm)
{
    Gfx.title = renderText(r, fBig, "DUEL", WHITE, NULL, NULL);
    Gfx.labScore = renderText(r, fSm, "Score", WHITE, NULL, NULL);
    Gfx.labLines = renderText(r, fSm, "Lignes", WHITE, NULL, NULL);
    Gfx.labNext = renderText(r, fSm, "Suivant", WHITE, NULL, NULL);
    Gfx.bg = loadTexture(r, BG_INGAME);
    Gfx.btnPause = loadTexture(r, BTN_PAUSE);
    Gfx.icoOn = loadTexture(r, ICON_MUSIC_ON);
    Gfx.icoOff = loadTexture(r, ICON_MUSIC_OFF);
    return Gfx.title && Gfx.labScore && Gfx.labLines && Gfx.labNext &&
           Gfx.bg && Gfx.btnPause && Gfx.icoOn && Gfx.icoOff;
}

static void destroyStaticTextures(void)
{
    SDL_DestroyTexture(Gfx.title);
    SDL_DestroyTexture(Gfx.labScore);
    SDL_DestroyTexture(Gfx.labLines);
    SDL_DestroyTexture(Gfx.labNext);
    SDL_DestroyTexture(Gfx.bg);
    SDL_DestroyTexture(Gfx.btnPause);
    SDL_DestroyTexture(Gfx.icoOn);
    SDL_DestroyTexture(Gfx.icoOff);
    memset(&Gfx, 0, sizeof Gfx);
}

typedef struct
{
    Uint32 last;
    SDL_Texture *tex;
    char buf[16];
} CachedNum;
static void updNum(SDL_Renderer *r, TTF_Font *f, CachedNum *cn, Uint32 v, const char *fmt)
{
    if (cn->tex && v == cn->last)
        return;
    if (cn->tex)
        SDL_DestroyTexture(cn->tex);
    sprintf(cn->buf, fmt, v);
    cn->tex = renderText(r, f, cn->buf, WHITE, NULL, NULL);
    cn->last = v;
}

int showDuel(SDL_Renderer *r)
{
    if (TTF_Init() != 0)
        return 0;
    TTF_Font *fBig = loadFont(FONT_PATH, 48);
    TTF_Font *fSm = loadFont(FONT_PATH, 28);
    if (!fBig || !fSm)
        return 0;

    if (!loadStaticTextures(r, fBig, fSm))
    {
        TTF_CloseFont(fBig);
        TTF_CloseFont(fSm);
        return 0;
    }
    initPiecesTextures(r);

    int W, H;
    SDL_GetRendererOutputSize(r, &W, &H);
    SDL_Rect bgRect = {0, 0, W, H};
    int blk = (H - 80) / GRID_ROWS;
    int wellH = blk * GRID_ROWS;
    int wellW = blk * GRID_COLS;
    SDL_Rect well[2] = {{W / 2 - wellW - 100, (H - wellH) / 2, wellW, wellH},
                        {W / 2 + 100, (H - wellH) / 2, wellW, wellH}};
    SDL_Rect btnPause = {W - 180, 20, 80, 80}, btnMusic = {W - 90, 20, 80, 80};

    Uint32 lastClock = UINT_MAX;
    SDL_Texture *texClock = NULL;
    char clkBuf[8];
    while (1)
    {
        Player P[2];
        resetPlayer(&P[0], 0);
        resetPlayer(&P[1], 1);
        ia_plan(&P[1]);
        Uint32 start = SDL_GetTicks(), lastIa = start;
        int running = 1, lost = 0, music = 1;
        SDL_Event e;
        CachedNum scN[2] = {{UINT_MAX, NULL, ""}, {UINT_MAX, NULL, ""}}, lnN[2] = {{UINT_MAX, NULL, ""}, {UINT_MAX, NULL, ""}};
        while (running)
        {
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                    running = 0;
                if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    int mx = e.button.x, my = e.button.y;
                    if (mx >= btnMusic.x && mx <= btnMusic.x + btnMusic.w && my >= btnMusic.y && my <= btnMusic.y + btnMusic.h)
                        music = !music;
                    else if (mx >= btnPause.x && mx <= btnPause.x + btnPause.w && my >= btnPause.y && my <= btnPause.y + btnPause.h)
                    {
                        if (showPause(r) == 1)
                        {
                            running = 0;
                            lost = 0;
                        }
                    }
                }
                if (e.type == SDL_KEYDOWN)
                {
                    Tetromino t = P[0].cur;
                    switch (e.key.keysym.sym)
                    {
                    case SDLK_LEFT:
                        --t.x;
                        break;
                    case SDLK_RIGHT:
                        ++t.x;
                        break;
                    case SDLK_DOWN:
                        ++t.y;
                        break;
                    case SDLK_UP:
                    case SDLK_z:
                        rotateTetromino(&t);
                        break;
                    case SDLK_SPACE:
                        while (!collides(&t, P[0].g))
                        {
                            P[0].cur = t;
                            ++t.y;
                        }
                        break;
                    }
                    if (!collides(&t, P[0].g))
                        P[0].cur = t;
                }
            }

            Uint32 now = SDL_GetTicks();
            if (now - lastIa >= IA_STEP_MS)
            {
                ia_move(&P[1]);
                lastIa = now;
            }

            /* gravité */
            for (int i = 0; i < 2; ++i)
            {
                Player *p = &P[i];
                if (now - p->lastFall > p->delay)
                {
                    Tetromino test = p->cur;
                    ++test.y;
                    if (!collides(&test, p->g))
                        ++p->cur.y;
                    else
                    {
                        for (int y = 0; y < 4; ++y)
                            for (int x = 0; x < 4; ++x)
                                if (p->cur.shape[y][x])
                                {
                                    int gx = p->cur.x + x, gy = p->cur.y + y;
                                    if (gx >= 0 && gx < GRID_COLS && gy >= 0 && gy < GRID_ROWS)
                                        p->g[gy][gx] = p->cur.type + 1;
                                }
                        memcpy(grid, p->g, sizeof grid);
                        int clr = clearCompleteLines(grid);
                        if (clr)
                        {
                            memcpy(p->g, grid, sizeof grid);
                            addScore(p, clr);
                            sendGarbage(&P[1 - i], clr);
                        }
                        p->cur = p->nxt;
                        p->nxt = createTetromino(rand() % 7);
                        p->cur.x = 3;
                        p->cur.y = 0;
                        p->iaPlanned = 0;
                        if (i == 1)
                            ia_plan(p);
                        if (collides(&p->cur, p->g))
                        {
                            running = 0;
                            lost = 1;
                        }
                    }
                    p->lastFall = now;
                }
            }

            /* update textes */
            for (int i = 0; i < 2; ++i)
            {
                updNum(r, fSm, &scN[i], P[i].score, "%06d");
                updNum(r, fSm, &lnN[i], P[i].lines, "%04d");
            }
            Uint32 sec = (SDL_GetTicks() - start) / 1000;
            if (sec != lastClock)
            {
                lastClock = sec;
                if (texClock)
                    SDL_DestroyTexture(texClock);
                sprintf(clkBuf, "%02d:%02d", sec / 60, sec % 60);
                texClock = renderText(r, fBig, clkBuf, WHITE, NULL, NULL);
            }

            SDL_RenderClear(r);
            SDL_RenderCopy(r, Gfx.bg, NULL, &bgRect);
            SDL_Rect rt;
            SDL_QueryTexture(Gfx.title, NULL, NULL, &rt.w, &rt.h);
            rt.x = W / 2 - rt.w - 30;
            rt.y = 20;
            SDL_RenderCopy(r, Gfx.title, NULL, &rt);
            SDL_Rect rc;
            SDL_QueryTexture(texClock, NULL, NULL, &rc.w, &rc.h);
            rc.x = rt.x + rt.w + 20;
            rc.y = rt.y + (rt.h - rc.h) / 2;
            SDL_RenderCopy(r, texClock, NULL, &rc);
            for (int i = 0; i < 2; ++i)
            {
                SDL_Rect *w = &well[i];
                Player *p = &P[i];
                roundedBoxRGBA(r, w->x, w->y, w->x + w->w, w->y + w->h, 20, BG.r, BG.g, BG.b, BG.a);
                roundedRectangleRGBA(r, w->x, w->y, w->x + w->w, w->y + w->h, 20, BORD.r, BORD.g, BORD.b, BORD.a);
                memcpy(grid, p->g, sizeof grid);
                drawGrid(r, blockTextures, blk, w->x, w->y);
                drawGhostPiece(r, &p->cur, grid, blk, w->x, w->y);
                drawTetromino(r, &p->cur, blockTextures, blk, w->x, w->y);
                SDL_Rect pane = {(i ? w->x + w->w + 30 : w->x - 220), w->y + 100, 200, 180};
                roundedBoxRGBA(r, pane.x, pane.y, pane.x + pane.w, pane.y + pane.h, 16, BG.r, BG.g, BG.b, 200);
                roundedRectangleRGBA(r, pane.x, pane.y, pane.x + pane.w, pane.y + pane.h, 16, BORD.r, BORD.g, BORD.b, 255);
                SDL_Rect rs = {pane.x + 10, pane.y + 10, 0, 0}, rl = {pane.x + 10, pane.y + 60, 0, 0};
                SDL_QueryTexture(Gfx.labScore, NULL, NULL, &rs.w, &rs.h);
                SDL_QueryTexture(Gfx.labLines, NULL, NULL, &rl.w, &rl.h);
                SDL_RenderCopy(r, Gfx.labScore, NULL, &rs);
                SDL_RenderCopy(r, Gfx.labLines, NULL, &rl);
                SDL_Rect vs = {pane.x + pane.w - 10, rs.y, 0, 0}, vl = {pane.x + pane.w - 10, rl.y, 0, 0};
                SDL_QueryTexture(scN[i].tex, NULL, NULL, &vs.w, &vs.h);
                SDL_QueryTexture(lnN[i].tex, NULL, NULL, &vl.w, &vl.h);
                vs.x -= vs.w;
                vl.x -= vl.w;
                SDL_RenderCopy(r, scN[i].tex, NULL, &vs);
                SDL_RenderCopy(r, lnN[i].tex, NULL, &vl);
                SDL_Rect nxt = {pane.x, pane.y + pane.h + 10, pane.w, blk * 4 + 20};
                roundedBoxRGBA(r, nxt.x, nxt.y, nxt.x + nxt.w, nxt.y + nxt.h, 16, BG.r, BG.g, BG.b, 200);
                roundedRectangleRGBA(r, nxt.x, nxt.y, nxt.x + nxt.w, nxt.y + nxt.h, 16, BORD.r, BORD.g, BORD.b, 255);
                SDL_Rect rn = {nxt.x + 10, nxt.y + 5, 0, 0};
                SDL_QueryTexture(Gfx.labNext, NULL, NULL, &rn.w, &rn.h);
                SDL_RenderCopy(r, Gfx.labNext, NULL, &rn);
                int nb = blk / 2, minX = 4, maxX = 0, minY = 4, maxY = 0;
                for (int y = 0; y < 4; ++y)
                    for (int x = 0; x < 4; ++x)
                        if (p->nxt.shape[y][x])
                        {
                            if (x < minX)
                                minX = x;
                            if (x > maxX)
                                maxX = x;
                            if (y < minY)
                                minY = y;
                            if (y > maxY)
                                maxY = y;
                        }
                int pw = (maxX - minX + 1) * nb, ph = (maxY - minY + 1) * nb, offX = nxt.x + (nxt.w - pw) / 2 - minX * nb, offY = nxt.y + 30 + (nxt.h - 30 - ph) / 2 - minY * nb;
                for (int y = 0; y < 4; ++y)
                    for (int x = 0; x < 4; ++x)
                        if (p->nxt.shape[y][x])
                        {
                            SDL_Rect d = {offX + x * nb, offY + y * nb, nb, nb};
                            SDL_RenderCopy(r, blockTextures[p->nxt.type], NULL, &d);
                        }
            }
            SDL_RenderCopy(r, Gfx.btnPause, NULL, &btnPause);
            SDL_RenderCopy(r, music ? Gfx.icoOn : Gfx.icoOff, NULL, &btnMusic);
            SDL_RenderPresent(r);
            SDL_Delay(16);
        }

        if (texClock)
        {
            SDL_DestroyTexture(texClock);
            texClock = NULL;
        }
        for (int i = 0; i < 2; ++i)
        {
            if (scN[i].tex)
                SDL_DestroyTexture(scN[i].tex);
            if (lnN[i].tex)
                SDL_DestroyTexture(lnN[i].tex);
        }
        if (lost)
        {
            if (showDuelGameOver(r, P[0].score, P[0].lines, P[1].score, P[1].lines) == 1)
                continue;
        }
        break;
    }

    destroyStaticTextures();
    freePiecesTextures();
    TTF_CloseFont(fBig);
    TTF_CloseFont(fSm);
    TTF_Quit();
    return 0;
}
