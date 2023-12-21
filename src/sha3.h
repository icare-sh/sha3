 #ifndef SHA3_H
 #define SHA3_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>


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

void * keccak(const void *in, size_t inlen, unsigned char del ,void *out, int outlen);


void * sha3_256(const void *in, size_t inlen, void *out);




#endif // SHA3_H