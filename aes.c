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
	return 1;
}

uint32_t rotWord(uint32_t word)
{
	return 1;
}

