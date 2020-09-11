#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "aes.h"
#include "util.h"

/**
 * AES Parameters
 *
 * Nb = 4
 * number of columns in the State
 *
 * Nk = 4, 6, or 8
 * number of 32-bit words in the Key
 *
 * Nr = 10, 12, or 14
 * number of rounds (function of Nb and Nk)
 */
const int Nb = 4;
int Nk = 4;
int Nr = 10;

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

void keyExpansion(uint8_t key[4*Nk], uint32_t w[Nb * (Nr + 1)])
{
	uint32_t tmp;
	int i;
	for (i = 0; i < Nk; i++) {
		w[i] = word(key[4 * i], key[4 * i + 1], key[4 * i + 2], key[4 * i + 3]);
	}

	assert(i == Nk);

	for (; i < Nb * (Nr + 1); i++) {
		tmp = w[i - 1];
		if (i % Nk == 0) {
			tmp = subWord(rotWord(tmp)) ^ rcon[i / Nk];
		} else if (Nk > 6 && i % Nk == 4) {
			tmp = subWord(tmp);
		}
		w[i] = w[i - Nk] ^ tmp;
	}
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

void invSubBytes(uint8_t state[4][Nb])
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			uint8_t byte = state[i][j];
			uint8_t col = byte >> 4;
			uint8_t row = byte & 0x0F;
			state[i][j] = invSbox[col][row];
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

void invShiftRows(uint8_t state[4][4])
{
	uint8_t tmp = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = tmp;

	swap(state, 2, 0, 2, 2);
	swap(state, 2, 1, 2, 3);

	tmp = state[3][0];
	state[3][0] = state[3][1];
	state[3][1] = state[3][2];
	state[3][2] = state[3][3];
	state[3][3] = tmp;
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

void invMixColumns(uint8_t state[4][4])
{
	int col;
	for (col = 0; col < 4; col++) {
		uint8_t s_0 = ffMultiply(0x0e, state[0][col]) ^ ffMultiply(0x0b, state[1][col]) ^ ffMultiply(0x0d, state[2][col]) ^ ffMultiply(0x09, state[3][col]);
		uint8_t s_1 = ffMultiply(0x09, state[0][col]) ^ ffMultiply(0x0e, state[1][col]) ^ ffMultiply(0x0b, state[2][col]) ^ ffMultiply(0x0d, state[3][col]);
		uint8_t s_2 = ffMultiply(0x0d, state[0][col]) ^ ffMultiply(0x09, state[1][col]) ^ ffMultiply(0x0e, state[2][col]) ^ ffMultiply(0x0b, state[3][col]);
		uint8_t s_3 = ffMultiply(0x0b, state[0][col]) ^ ffMultiply(0x0d, state[1][col]) ^ ffMultiply(0x09, state[2][col]) ^ ffMultiply(0x0e, state[3][col]);

		state[0][col] = s_0;
		state[1][col] = s_1;
		state[2][col] = s_2;
		state[3][col] = s_3;
	}
}

void addRoundKey(uint8_t state[4][4], uint32_t w[Nb * (Nr + 1)], uint8_t start)
{
	int i, j;
	for (j = 0, i = start; i < start + Nb; i++, j++) {
		uint8_t b1 = (w[i] >> (8 * 3) & 0xff);
		uint8_t b2 = (w[i] >> (8 * 2) & 0xff);
		uint8_t b3 = (w[i] >> (8 * 1) & 0xff);
		uint8_t b4 = (w[i] >> (8 * 0) & 0xff);

		state[0][j] ^= b1;
		state[1][j] ^= b2;
		state[2][j] ^= b3;
		state[3][j] ^= b4;
	}
}

void cipher(uint8_t in[4 * Nb], uint8_t out[4 * Nb], uint32_t w[Nb * (Nr + 1)])
{
	uint8_t state[4][Nb];
	int i = 0, row, col;
	for (col = 0; col < Nb; col++) {
		for (row = 0; row < 4; row++) {
			state[row][col] = in[i++];
		}
	}
	printState("input", state, 0);
	printKeySchedule(0, w, 0);

	addRoundKey(state, w, 0);

	for (i = 1; i < Nr; i++) {
		printState("start", state, i);
		subBytes(state);
		printState("s_box", state, i);
		shiftRows(state);
		printState("s_row", state, i);
		mixColumns(state);
		printState("m_col", state, i);
		addRoundKey(state, w, i * Nb); // w[i * Nb, (i + 1) * Nb - 1]
		printKeySchedule(i, w, i * Nb);
	}

	subBytes(state);
	printState("s_box", state, Nr);
	shiftRows(state);
	printState("s_row", state, Nr);
	addRoundKey(state, w, Nr * Nb); // w[Nr * Nb, (Nr + 1) * Nb - 1]
	printKeySchedule(Nr, w, Nr * Nb);
	printState("output", state, Nr);

	i = 0;
	for (col = 0; col < Nb; col++) {
		for (row = 0; row < 4; row++) {
			out[i++] = state[row][col];
		}
	}
}

void invCipher(uint8_t in[4 * Nb], uint8_t out[4 * Nb], uint32_t w[Nb * (Nr + 1)])
{
	uint8_t state[4][Nb];
	int i = 0, row, col;
	for (col = 0; col < Nb; col++) {
		for (row = 0; row < 4; row++) {
			state[row][col] = in[i++];
		}
	}
	printState("iinput", state, Nr);
	printKeySchedule(Nr, w, Nr * Nb);

	addRoundKey(state, w, Nr * Nb);

	for (i = Nr - 1; i > 0; i--) {
		printState("istart", state, i);
		invShiftRows(state);
		printState("is_row", state, i);
		invSubBytes(state);
		printState("is_box", state, i);
		addRoundKey(state, w, i * Nb);
		printKeySchedule(i, w, i * Nb);
		invMixColumns(state);
		printState("im_col", state, i);
	}

	invShiftRows(state);
	printState("is_row", state, 0);
	invSubBytes(state);
	printState("is_box", state, 0);
	addRoundKey(state, w, 0);
	printKeySchedule(i, w, 0);
	printState("ioutput", state, 0);

	i = 0;
	for (col = 0; col < Nb; col++) {
		for (row = 0; row < 4; row++) {
			out[i++] = state[row][col];
		}
	}
}