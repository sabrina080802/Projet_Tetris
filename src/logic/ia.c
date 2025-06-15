#include "ia.h"

#define W_HEIGHT -1 /* malus */

bool collides(Tetromino *t, int g[GRID_ROWS][GRID_COLS]);
bool rotateTetromino(Tetromino *t);

enum Act
{
    ACT_ROT,
    ACT_LEFT,
    ACT_RIGHT,
    ACT_DROP
};
#define ACT_BUF_MAX 64
static enum Act actions[ACT_BUF_MAX];
static int actLen = 0, actIdx = 0;

static void rotate_times(Tetromino *t, int n)
{
    while (n--)
        rotateTetromino(t);
}
static void bounds(const Tetromino *t, int *minX, int *maxX)
{
    *minX = 4;
    *maxX = 0;
    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x)
            if (t->shape[y][x])
            {
                if (x < *minX)
                    *minX = x;
                if (x > *maxX)
                    *maxX = x;
            }
}

static int count_holes(int g[GRID_ROWS][GRID_COLS])
{
    int h = 0;
    for (int c = 0; c < GRID_COLS; ++c)
    {
        bool filled = false;
        for (int r = 0; r < GRID_ROWS; ++r)
        {
            if (g[r][c])
                filled = true;
            else if (filled)
                ++h;
        }
    }
    return h;
}
static int count_lines(int g[GRID_ROWS][GRID_COLS])
{
    int l = 0;
    for (int r = 0; r < GRID_ROWS; ++r)
    {
        bool full = true;
        for (int c = 0; c < GRID_COLS; ++c)
            if (!g[r][c])
            {
                full = false;
                break;
            }
        if (full)
            ++l;
    }
    return l;
}

/* ---- score struct ---- */
struct Score
{
    int lines, filled, y;
};
static bool better(struct Score a, struct Score b)
{
    if (a.lines != b.lines)
        return a.lines > b.lines;
    if (a.filled != b.filled)
        return a.filled > b.filled;
    return a.y > b.y;
}

/* ---- simulate placement, returns score (and y) or invalid ---- */
static bool simulate(Player *p, int wantX, int wantRot, struct Score *outSc)
{
    int tmpG[GRID_ROWS][GRID_COLS];
    memcpy(tmpG, p->g, sizeof tmpG);

    Tetromino t = p->cur;
    t.x = wantX;
    t.y = 0;
    rotate_times(&t, wantRot);

    int minX, maxX;
    bounds(&t, &minX, &maxX);
    if (t.x + minX < 0 || t.x + maxX >= GRID_COLS)
        return false;

    while (!collides(&t, tmpG))
        ++t.y;
    --t.y;
    if (t.y < 0)
        return false;

    for (int y = 0; y < 4; ++y)
        for (int x = 0; x < 4; ++x)
            if (t.shape[y][x])
            {
                if (tmpG[t.y + y][t.x + x])
                    return false;
                tmpG[t.y + y][t.x + x] = 1;
            }

    int beforeHoles = count_holes(p->g);
    int afterHoles = count_holes(tmpG);
    int filled = beforeHoles - afterHoles;

    *outSc = (struct Score){.lines = count_lines(tmpG), .filled = filled, .y = t.y};
    return true;
}

/* ---- planification ---- */
void ia_plan(Player *p)
{
    actLen = actIdx = 0;

    struct Score best = {-1, -1, -1};
    int bestX = 0;
    int bestRot = 0;

    /* Pièces I, S, Z : 2 rotations ; les autres : 4                     */
    int rotMax = (p->cur.type == 0 || p->cur.type == 3 || p->cur.type == 4)
                     ? 2
                     : 4;

    /* ------------------------------------------------------------------ */
    for (int rot = 0; rot < rotMax; ++rot)
    {
        Tetromino tmp = p->cur;
        rotate_times(&tmp, rot);

        /* Limites de la matrice 4×4 après rotation                       */
        int minX, maxX;
        bounds(&tmp, &minX, &maxX);

        /* ★ Nouveaux bords autorisés :                                   *
         *    left  = valeur minimale pour coller minX à la colonne 0     *
         *    right = valeur maximale pour coller maxX à la dernière col. */
        int left = -minX;
        int right = GRID_COLS - (maxX + 1);

        for (int x = left; x <= right; ++x)
        {
            struct Score sc;
            if (!simulate(p, x, rot, &sc))
                continue; /* placement invalide   */

            if (best.lines < 0 || better(sc, best))
            {
                best = sc;
                bestX = x;
                bestRot = rot;
            }
        }
    }
    /* ------------------------------------------------------------------ */
    if (best.lines < 0)
        return; /* Aucun coup possible   */

    /* Remplit le buffer d’actions                                        */
    int rotDiff = (bestRot - p->cur.rotation + 4) % 4;
    for (int i = 0; i < rotDiff && actLen < ACT_BUF_MAX; ++i)
        actions[actLen++] = ACT_ROT;

    int dx = bestX - p->cur.x;
    enum Act move = (dx > 0) ? ACT_RIGHT : ACT_LEFT;
    for (int i = 0; i < abs(dx) && actLen < ACT_BUF_MAX; ++i)
        actions[actLen++] = move;

    actions[actLen++] = ACT_DROP;
}

/* ---- exécution tick ---- */
void ia_move(Player *p)
{
    /* ré‑planifie si aucune action en attente */
    if (actIdx >= actLen)
    {
        ia_plan(p);
        if (actLen == 0)
            return; /* aucun coup possible */
    }

    enum Act a = actions[actIdx++];

    switch (a)
    {
    case ACT_ROT:
    {
        Tetromino preview = p->cur;
        rotateTetromino(&preview);
        if (!collides(&preview, p->g))
        {
            rotateTetromino(&p->cur);
            p->cur.rotation = (p->cur.rotation + 1) % 4;
        }
        else
        {
            actLen = actIdx = 0; /* plan fichu */
        }
        break;
    }

    case ACT_LEFT:
    case ACT_RIGHT:
    {
        int dir = (a == ACT_LEFT ? -1 : 1);
        Tetromino prev = p->cur;
        prev.x += dir;
        if (!collides(&prev, p->g))
        {
            p->cur.x += dir;
        }
        else
        {
            actLen = actIdx = 0; /* replanner */
        }
        break;
    }

    case ACT_DROP:
    {
        Tetromino t = p->cur;
        while (!collides(&t, p->g))
            ++t.y;
        --t.y;
        p->cur.y = t.y;
        actLen = actIdx = 0; /* pièce verrouillée au tick moteur suivant */
        break;
    }
    }
}
