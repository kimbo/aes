#ifndef KIMBO_AES_H
#define KIMBO_AES_H

#include <stdint.h>

/***************************/
/* finite field arithmetic */
/***************************/

/* adds two finite fields (see Section 4.1) */
uint8_t ffAdd(uint8_t, uint8_t);

/* multiplies a finite field by x (see Section 4.2.1) */
uint8_t xtime(uint8_t);

/* uses xtime to multiply any finite field by any other finite field. (see Section 4.2.1) */
uint8_t ffMultiply(uint8_t, uint8_t);

/********************/
/*   key expansion  */
/********************/

void keyExpansion(uint8_t*, uint32_t*);

/* takes a four-byte input word and substitutes each byte in that word with its appropriate value from the S-Box. The S-box is provided (see Section 5.1.1). */
uint32_t subWord(uint32_t);

/* performs a cyclic permutation on its input word. */
uint32_t rotWord(uint32_t);

/*******************/
/* cipher function */
/*******************/

void subBytes();
void shiftRows();
void mixColumns();
void addRoundKey();

/***************************/
/* inverse cipher function */
/***************************/

void invSubBytes();
void invShiftRows();
void invMixColumns();

#endif /* KIMBO_AES_H */
