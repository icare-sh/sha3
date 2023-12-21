#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sha3.h"

static int hexdigit(char ch)
{
    if (ch >= '0' && ch <= '9')
        return  ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return  ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return  ch - 'a' + 10;
    return -1;
}

static int readhex(uint8_t *buf, const char *str, int maxbytes)
{
    int i, h, l;

    for (i = 0; i < maxbytes; i++) {
        h = hexdigit(str[2 * i]);
        if (h < 0)
            return i;
        l = hexdigit(str[2 * i + 1]);
        if (l < 0)
            return i;
        buf[i] = (h << 4) + l; 
    }

    return i;
}

static char * processBinaryFile(const char *binaryFilePath) {
    FILE *file = fopen(binaryFilePath, "rb");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    // Find the size of the file
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Read the binary data
    char *binaryData = (char *)malloc(fileSize);
    fread(binaryData, 1, fileSize, file);
    fclose(file);

    // Convert binary data to hexadecimal
    char *hexData = (char *)malloc(fileSize * 2 + 1); // Each byte becomes two hex characters
    for (int i = 0; i < fileSize; ++i) {
        sprintf(hexData + i * 2, "%02X", (unsigned char)binaryData[i]);
    }
    hexData[fileSize * 2] = '\0';

    // Clean up
    free(binaryData);
    
    return hexData;
}

static void usage(const char *progname)
{
    fprintf(stderr, "Usage: %s [-m] <files>\n", progname);
    exit(1);
}

static void print_hash(char * path, int hashType, uint8_t * hash)
{
    printf("SHA3-256 %s = ", path);

    for (int i = 0; i < hashType / 8; i++)
    {
        printf("%02x",hash[i]);
    }
}

int main(int argc, char ** argv){
    if (argc < 2){
        usage(argv[0]);
        return 1;
    }

    char * hexData;
    char *path;
    size_t hexDataLen;

    int hashType = 256;
    uint8_t * data ;
    uint8_t * hash;

    if (strcmp(argv[1], "-m") != 0 )
    {
        path = malloc(strlen(argv[1]));
        memcpy(path, argv[3], strlen(argv[3]));
        hexData = processBinaryFile(argv[1]);
        hexDataLen = strlen(hexData);
        data = (uint8_t *)malloc(hexDataLen / 2);
        readhex(data, hexData, hexDataLen / 2);

        hash = (uint8_t *)malloc(32);
        sha3_256(data, hexDataLen / 2, hash);
    }
    else
    {
        hashType = atoi(argv[2]);
        path = malloc(strlen(argv[3]));
        memcpy(path, argv[3], strlen(argv[3]));
        hexData = processBinaryFile(argv[3]);
        hexDataLen = strlen(hexData);
        data = (uint8_t *)malloc(hexDataLen / 2);
        readhex(data, hexData, hexDataLen / 2);

        switch (hashType)
        {
        case 224:
            hash = (uint8_t *)malloc(28);
            sha3_224(data, hexDataLen / 2, hash);
            break;
        case 256:
            hash = (uint8_t *)malloc(32);
            sha3_256(data, hexDataLen / 2, hash);
            break;
        case 384:
            hash = (uint8_t *)malloc(48);
            sha3_384(data, hexDataLen / 2, hash);
            break;
        case 512:
            hash = (uint8_t *)malloc(64);
            sha3_512(data, hexDataLen / 2, hash);
            break;
        default:
            usage(argv[0]);
            return 1;
            break;
        }
    }

    print_hash(path, hashType, hash);


    free(data);
    free(hash);
    free(hexData);
    free(path);

    return 0;
}