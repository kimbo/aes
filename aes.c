#include "aes.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t ffAdd(uint8_t ff1, uint8_t ff2)
{
	return ff1 ^ ff2;
}

uint8_t xtime(uint8_t in)
{
	uint16_t tmp = in << 1;
	if (tmp <= 0xff) {
		return (uint8_t)tmp;
	}
	return tmp ^ 0x011b;	
}

uint8_t ffMultiply(uint8_t ff1, uint8_t ff2)
{
	uint8_t prev;
	uint8_t ans = 0;
	uint8_t curr = ff1;
	int mask;
	for (mask = 1; mask <= ff2; mask <<= 1) {
		prev = curr;
		curr = xtime(prev);
		if (ff2 & mask) {
			ans ^= prev;
		}
	}
	return ans;
}

void keyExpansion(uint8_t *key, uint32_t *buf)
{

}

uint32_t subWord(uint32_t word)
{
	uint32_t result = 0;
	uint8_t byteNo;
	for (byteNo = 0; byteNo < 4; byteNo++) {
		uint8_t byte = (word >> (8 * byteNo) & 0xff);
		uint8_t row = byte >> 4;
		uint8_t col = byte & 0x0F;
		uint8_t newByte = sbox[row][col];
		result |= (newByte << (8 * byteNo));
	}
	return result;
}

uint32_t rotWord(uint32_t word)
{
	return 1;
}

void mixColumns(uint8_t state[4][4])
{
	
}

void subBytes(uint8_t state[4][4])
{

}
void shiftRows(uint8_t state[4][4])
{

}
void addRoundKey(uint8_t state[4][4], uint32_t *w, uint8_t something)
{

}
void cipher(uint8_t *in, uint8_t *out, uint32_t *w)
{

}