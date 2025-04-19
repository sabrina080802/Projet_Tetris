#include <stdlib.h>
#include <time.h>

static int bag[7];
static int indexInBag = 0;

void shuffleBag() {
    for (int i = 0; i < 7; i++) {
        int j = rand() % 7;
        int temp = bag[i];
        bag[i] = bag[j];
        bag[j] = temp;
    }
}

void initPieceBag() {
    for (int i = 0; i < 7; i++) {
        bag[i] = i;
    }
    shuffleBag();
    indexInBag = 0;
}

int getNextPieceType() {
    if (indexInBag >= 7) {
        shuffleBag();
        indexInBag = 0;
    }
    return bag[indexInBag++];
}
