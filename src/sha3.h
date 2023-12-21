 #ifndef SHA3_H
 #define SHA3_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>



typedef struct {
    union {
        uint8_t b[200];
        uint64_t q[25]; 
    } state;    // matrix containing the 5x5 and 64-bit words 5*5*64 = 1600 bits (200 bytes)
} keccak_t;

/*
    * ROTL64
    * x: The 64-bit value to be rotated
    * y: The number of bits to rotate by
    * 
    * This function rotates the 64-bit value x by y bits to the left
*/
#define ROTL64(x, y) (((x) << (y)) | ((x) >> (64 - (y)))) 

/*
    * keccak_f
    * A: The state to be permuted
    * 
    * This function permutes the state A using the Keccak-f permutation , 24 rounds of bit manipulation
*/
void keccak_f(uint64_t A[25]);

void keccak_init(keccak_t * sha3, int mdlen);

int keccak_absorb(keccak_t * sha3, const void *in, size_t inlen, int rsiz);

void keccak_squeeze(keccak_t * sha3, void *out, size_t outlen, unsigned char del, int rsiz, int i_empty);

void * keccak(const void *in, size_t inlen, unsigned char del ,void *out, int outlen);

void * sha3_256(const void *in, size_t inlen, void *out);




#endif // SHA3_H