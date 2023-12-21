#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sha3.h"


/**
 * @brief Convertit un caractère hexadécimal en entier.
 *
 * Cette fonction prend en entrée un caractère et renvoie la valeur
 * numérique correspondante pour un chiffre hexadécimal.
 *
 * @param ch Le caractère hexadécimal en entrée.
 * @return La valeur numérique correspondante ou -1 si le caractère
 *         n'est pas un chiffre hexadécimal valide.
 */
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

/**
 * @brief Lit une chaîne hexadécimale et convertit en tableau d'octets.
 *
 * Cette fonction prend en entrée une chaîne hexadécimale et la convertit
 * en un tableau d'octets (uint8_t). Elle lit chaque paire de caractères
 * hexadécimaux de la chaîne et les convertit en une valeur numérique.
 *
 * @param buf      Le tableau d'octets en sortie où stocker les valeurs lues.
 * @param str      La chaîne hexadécimale en entrée.
 * @param maxbytes Le nombre maximal d'octets à lire et stocker dans buf.
 * @return Le nombre d'octets lus et stockés dans buf.
 */
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


/**
 * @brief Traite un fichier en le convertissant en une chaîne hexadécimale.
 *
 * Cette fonction prend en entrée le chemin d'un fichier, l'ouvre,
 * lit son contenu binaire, convertit les données en une chaîne hexadécimale
 * et retourne la chaîne résultante.
 *
 * @param binaryFilePath Le chemin du fichier à traiter.
 * @return Une chaîne hexadécimale représentant les données binaires du fichier.
 * @note Il est de la responsabilité de l'appelant de libérer la mémoire allouée
 *       pour la chaîne hexadécimale après utilisation.
 */
static char * processBinaryFile(const char *binaryFilePath) {
    // Ouvre le fichier en mode lecture binaire
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

    // Alloue de la mémoire pour stocker les données hexadécimales
    char *hexData = (char *)malloc(fileSize * 2 + 1); // Each byte becomes two hex characters
    for (int i = 0; i < fileSize; ++i) {
        // Convertit chaque octet en deux caractères hexadécimaux et les concatène
        sprintf(hexData + i * 2, "%02X", (unsigned char)binaryData[i]);
    }
    hexData[fileSize * 2] = '\0';// Ajoute le caractère de fin de chaîne

    // Libère la mémoire allouée pour les données binaires
    free(binaryData);
    
    return hexData;
}

/**
 * @brief Affiche un message d'utilisation pour le programme.
 *
 * Cette fonction prend en entrée le nom du programme (progname) et affiche
 * un message d'utilisation indiquant comment utiliser le programme correctement.
 *
 * @param progname Le nom du programme.
 * @note Cette fonction provoque la sortie du programme avec le code de sortie 1.
 */
static void usage(const char *progname)
{
    fprintf(stderr, "Usage: %s [-m] <files>\n", progname);
    exit(1);
}


/**
 * @brief Affiche le hash d'un fichier avec son chemin, le type de hash et la valeur du hash.
 *
 * Cette fonction prend en entrée le chemin d'un fichier, le type de hash
 * (hashType) et la valeur du hash (hash). Elle affiche ces informations
 * au format "SHA3-256 <chemin du fichier> = <valeur du hash>".
 *
 * @param path      Le chemin du fichier.
 * @param hashType  Le type de hash à afficher.
 * @param hash      La valeur du hash à afficher.
 */
static void print_hash(char * path, int hashType, uint8_t * hash)
{
    printf("SHA3-256 %s = ", path);

    for (int i = 0; i < hashType / 8; i++)
    {
        printf("%02x",hash[i]);
    }
}

/**
 * @brief Fonction principale du programme.
 *
 * Cette fonction prend en entrée les arguments de la ligne de commande
 * et affiche le hash de chaque fichier passé en argument.
 *
 * @param argc Le nombre d'arguments de la ligne de commande.
 * @param argv Les arguments de la ligne de commande.
 * @return 0 si le programme s'est exécuté correctement, 1 sinon.
 */
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
        for (int i = 1; i < argc; i++)
        {
            path = malloc(strlen(argv[i]));
            memcpy(path, argv[i], strlen(argv[i]));
            hexData = processBinaryFile(argv[i]);
            hexDataLen = strlen(hexData);
            data = (uint8_t *)malloc(hexDataLen / 2);
            readhex(data, hexData, hexDataLen / 2);
            hash = (uint8_t *)malloc(32);
            sha3_256(data, hexDataLen / 2, hash);
            print_hash(path, hashType, hash);
            printf("\n");
            free(data);
            free(hash);
            free(hexData);
            free(path); 
        }
        return 0;
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
        print_hash(path, hashType, hash);
    }

    free(data);
    free(hash);
    free(hexData);
    free(path);

    return 0;
}