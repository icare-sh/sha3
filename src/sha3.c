 #include "sha3.h"


// KeccakRhoOffsets


void keccak_f(uint64_t A[25])
{
    const unsigned int KeccakRhoOffsets[24] = {
        1, 3, 6, 10, 15, 21, 28, 36,
        45, 55, 2, 14, 27, 41, 56, 8,
        25, 43, 62, 18, 39, 61, 20, 44
    };

    const unsigned int KeccakPilnIndices[24] = {
        10, 7, 11, 17, 18, 3, 5, 16,
        8, 21, 24, 4, 15, 23, 19, 13,
        12, 2, 20, 14, 22, 9, 6, 1
    };

    const uint64_t KeccakRoundConstants[24] = {
        0x0000000000000001, 0x0000000000008082,
        0x800000000000808a, 0x8000000080008000,
        0x000000000000808b, 0x0000000080000001,
        0x8000000080008081, 0x8000000000008009,
        0x000000000000008a, 0x0000000000000088,
        0x0000000080008009, 0x000000008000000a,
        0x000000008000808b, 0x800000000000008b,
        0x8000000000008089, 0x8000000000008003,
        0x8000000000008002, 0x8000000000000080,
        0x000000000000800a, 0x800000008000000a,
        0x8000000080008081, 0x8000000000008080,
        0x0000000080000001, 0x8000000080008008
    };

    unsigned int x, y, round;

    uint64_t C[5], D;

    for (round = 0; round < 24; round++) {
        // θ step
        for (x = 0; x < 5; x++) {
            C[x] = A[x] ^ A[x + 5] ^ A[x + 10] ^ A[x + 15] ^ A[x + 20];
        }

        for (x = 0; x < 5; x++) {
            D = C[(x + 4) % 5] ^ ROTL64(C[(x + 1) % 5], 1);
            for (y = 0; y < 25; y += 5) {
                A[y + x] ^= D;
            }
        }

        // ρ and π steps
        D = A[1];
        for (x = 0; x < 24; x++) {
            y = KeccakPilnIndices[x];
            C[0] = A[y];
            A[y] = ROTL64(D, KeccakRhoOffsets[x]);
            D = C[0];
        }

        // χ step
        for (y = 0; y < 25; y += 5) {
            for (x = 0; x < 5; x++) {
                C[x] = A[y + x];
            }
            for (x = 0; x < 5; x++) {
                A[y + x] ^= (~C[(x + 1) % 5]) & C[(x + 2) % 5];
            }
        }

        // ι step
        A[0] ^= KeccakRoundConstants[round];
    }
}

void keccak_init(keccak_t * sha3, int mdlen)
{
    memset(sha3, 0, sizeof(keccak_t));
}

int keccak_absorb(keccak_t * sha3, const void *in, size_t inlen, int rsiz)
{
    size_t i;
    const uint8_t *pin = in;
    int j = 0;

    for (i = 0; i < inlen; i++) {
        sha3->state.b[j++] ^= pin[i];
        if (j >= rsiz) {
            keccak_f(sha3->state.q);
            j = 0;
        }
    }
    return j;
}

void keccak_squeeze(keccak_t * sha3, void *out, size_t outlen, unsigned char del, int rsiz, int i_empty)
{
    size_t i;
    uint8_t *pout = out;
    
    sha3->state.b[i_empty] ^= del;
    sha3->state.b[rsiz - 1] ^= 0x80;
    keccak_f(sha3->state.q);

    for (i = 0; i < outlen; i++) {
        pout[i] = sha3->state.b[i];
    }
}

void * keccak(const void *in, size_t inlen, unsigned char del ,void *out, int outlen)
{
    keccak_t sha3;
    int rsiz;
    int i_empty = 0;
    rsiz = 200 - 2 * outlen;

    keccak_init(&sha3, outlen);
    i_empty = keccak_absorb(&sha3, in, inlen, rsiz);
    keccak_squeeze(&sha3, out, outlen, del, rsiz, i_empty);

    return out;
}

void * sha3_256(const void *in, size_t inlen, void *out)
{
    uint8_t * h;
    h = keccak(in, inlen, 0x06, out, 256 / 8);

    return h;
}