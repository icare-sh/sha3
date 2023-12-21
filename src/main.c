#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sha3.h"

static int test_hexdigit(char ch)
{
    if (ch >= '0' && ch <= '9')
        return  ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return  ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return  ch - 'a' + 10;
    return -1;
}

static int test_readhex(uint8_t *buf, const char *str, int maxbytes)
{
    int i, h, l;

    for (i = 0; i < maxbytes; i++) {
        h = test_hexdigit(str[2 * i]);
        if (h < 0)
            return i;
        l = test_hexdigit(str[2 * i + 1]);
        if (l < 0)
            return i;
        buf[i] = (h << 4) + l; // transform to bytes
    }

    return i;
}

int main(int argc, char ** argv){
    char str[] = "9F2FCC7C90DE090D6B87CD7E9718C1EA6CB21118FC2D5DE9F97E5DB6AC1E9C10";
    uint8_t in [256];
    uint8_t * hash;
    int msg_len;
    uint8_t  out[64];
    memset(in, 0, sizeof(in));
    memset(out, 0, sizeof(out));
    msg_len = test_readhex(in , str, sizeof(in));
    hash = sha3_256(in, msg_len, out);
    
    for (int i = 0; i < 64; i++){
        printf("%02x", hash[i]);
    }

    return 0;
}