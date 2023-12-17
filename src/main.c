#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint64_t tKeccakLane;

void xorLane(uint8_t *state, size_t x, size_t y, tKeccakLane lane) {
    size_t index = x + 5 * y;
    tKeccakLane *element = (tKeccakLane*)(state + sizeof(tKeccakLane) * index);
    *element ^= lane;
}

static void xor64(uint8_t *x, uint64_t u)
{
    unsigned int i;

    for(i=0; i<8; ++i) {
        x[i] ^= u;
        u >>= 8;
    }
}

#define i(x, y) ((x)+5*(y))

#define XORLane(x, y, lane)        xor64((uint8_t*)state+sizeof(tKeccakLane)*i(x, y), lane)

int main() {
    // Exemple d'utilisation
    tKeccakLane state[5][5];  // Supposons que state soit un tableau bidimensionnel de type tKeccakLane
    size_t x = 1, y = 4;     // Coordonnées (2, 3)
    tKeccakLane lane = 0xFFFFFFFFFFFFFF;  // Valeur à XOR

    // Appel de la fonction simle
    //xorLane((uint8_t*)state, x, y, lane);

    //printf("%016lX\n", state[1][4]);

    // Appel de la macro
    XORLane(x, y, lane);

    printf("%016lX\n", state[1][4]);

    uint8_t *v = malloc(sizeof(uint64_t));
    xor64(v, lane);

    // print x
    printf("%016lX\n", v);

    // À ce stade, l'élément à la position (2, 3) dans le tableau a été XOR avec la valeur 'lane'.
    return 0;
}
