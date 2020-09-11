#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes.h"
#include "util.h"

extern int Nk;
extern int Nr;
extern const int Nb;

enum action {
    ENCRYPT, DECRYPT,
};

int main(int argc, char **argv)
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <key> <decrypt/encrypt> <msg>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *key = argv[1];
    enum action act = strcmp(argv[2], "decrypt") == 0 ? DECRYPT : ENCRYPT;
    char *msg = argv[3];

    int keyLen;
    switch(strlen(key)) {
    case 32:
        keyLen = 16;
        Nr = 10;
        Nk = 4;
        break;
    case 48:
        keyLen = 24;
        Nr = 12;
        Nk = 6;
        break;
    case 64:
        keyLen = 32;
        Nr = 14;
        Nk = 8;
        break;
    default:
        fprintf(stderr, "Key length %lu is invalid\n", strlen(key) / 2);
        exit(1);
    }
    uint8_t keyBuf[keyLen];
    int i, j;
    for (i = 0, j = 0; i < keyLen; i++, j += 2) {
        sscanf(key + j, "%2x", (uint32_t *)&keyBuf[i]);
    }

    uint8_t input[16];
    memset(input, 0, 16);
    for (i = 0, j = 0; i < 16; i++, j += 2) {
        sscanf(msg + j, "%2x", (uint32_t *)&input[i]);
    }

    uint8_t output[16];
    memset(output, 0, 16);
	uint32_t w[Nb * (Nr + 1)];
    keyExpansion(keyBuf, w);
    if (act == ENCRYPT) {
        cipher(input, output, w);
    } else {
        invCipher(input, output, w);
    }
}
