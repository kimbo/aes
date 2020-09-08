#include "aes.h"
#include "util.h"
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
	uint16_t mask;
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
		uint8_t col = byte >> 4;
		uint8_t row = byte & 0x0F;
		uint8_t newByte = sbox[col][row];
		result |= (newByte << (8 * byteNo));
	}
	return result;
}

uint32_t rotWord(uint32_t word)
{
	uint32_t result = 0;
	int8_t i;
	for (i = 3; i >= 0; i--) {
		uint8_t byte = (word >> (8 * i) & 0xff);
		uint8_t destIdx = i == 3 ? 0 : i + 1;
		result |= (byte << (8 * destIdx));
	}
	return result;
}

void subBytes(uint8_t state[4][4])
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			uint8_t byte = state[i][j];
			uint8_t col = byte >> 4;
			uint8_t row = byte & 0x0F;
			state[i][j] = sbox[col][row];
		}
	}
}

void shiftRows(uint8_t state[4][4])
{
	uint8_t tmp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = tmp;

	swap(state, 2, 0, 2, 2);
	swap(state, 2, 1, 2, 3);

	swap(state, 3, 0, 3, 3);
	swap(state, 3, 3, 3, 1);
	swap(state, 3, 3, 3, 2);
}

void mixColumns(uint8_t state[4][4])
{
	int col;
	for (col = 0; col < 4; col++) {
		uint8_t s_0 = ffMultiply(0x02, state[0][col]) ^ ffMultiply(0x03, state[1][col]) ^ state[2][col] ^ state[3][col];
		uint8_t s_1 = state[0][col] ^ ffMultiply(0x02, state[1][col]) ^ ffMultiply(0x03, state[2][col]) ^ state[3][col];
		uint8_t s_2 = state[0][col] ^ state[1][col] ^ ffMultiply(0x02, state[2][col]) ^ ffMultiply(0x03, state[3][col]);
		uint8_t s_3 = ffMultiply(0x03, state[0][col]) ^ state[1][col] ^ state[2][col] ^ ffMultiply(0x02, state[3][col]);

		state[0][col] = s_0;
		state[1][col] = s_1;
		state[2][col] = s_2;
		state[3][col] = s_3;
	}
}

void addRoundKey(uint8_t state[4][4], uint32_t *w, uint8_t something)
{

}

void cipher(uint8_t *in, uint8_t *out, uint32_t *w)
{

}