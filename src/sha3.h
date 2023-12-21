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


/*
    * keccak_init
    * sha3: The object to be initialized
    * mdlen: The length of the message digest
    * 
    * This function initializes the internal state of a Keccak object by filling it with zeros
*/
void keccak_init(keccak_t * sha3, int mdlen);

/*
    * keccak_absorb
    * sha3: The object to be initialized
    * in: The input message
    * inlen: The length of the input message
    * rsiz: The rate of the sponge function
    * 
    * This function absorbs the input message into the sponge function
*/
int keccak_absorb(keccak_t * sha3, const void *in, size_t inlen, int rsiz);

/*
    * keccak_squeeze
    * sha3: The object to be initialized
    * out: The output message
    * outlen: The length of the output message
    * del: The delimiter
    * rsiz: The rate of the sponge function
    * i_empty: The number of bytes left in the sponge function
    * 
    * This function squeezes the output message from the sponge function
*/
void keccak_squeeze(keccak_t * sha3, void *out, size_t outlen, unsigned char del, int rsiz, int i_empty);

/*
    * keccak
    * in: The input message
    * inlen: The length of the input message
    * del: The delimiter
    * out: The output message
    * outlen: The length of the output message
    * 
    * This function performs the Keccak hash function on the input message
*/
void * keccak(const void *in, size_t inlen, unsigned char del ,void *out, int outlen);


void * sha3_224(const void *in, size_t inlen, void *out);
void * sha3_256(const void *in, size_t inlen, void *out);
void * sha3_384(const void *in, size_t inlen, void *out);
void * sha3_512(const void *in, size_t inlen, void *out);




#endif // SHA3_H