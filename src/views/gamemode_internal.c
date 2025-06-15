#include "gamemode_internal.h"

static int bag[7];
static int indexInBag = 0;

void initPieceBag()
{
    for (int i = 0; i < 7; i++)
        bag[i] = i;

    shuffleBag();
    indexInBag = 0;
}

int getNextPieceType()
{
    if (indexInBag >= 7)
    {
        shuffleBag();
        indexInBag = 0;
    }

    return bag[indexInBag++];
}

void shuffleBag()
{
    srand(time(NULL));
    for (int i = 0; i < 7; i++)
    {
        int j = rand() % 7;
        int temp = bag[i];
        bag[i] = bag[j];
        bag[j] = temp;
    }
}