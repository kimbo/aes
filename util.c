#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

uint32_t word(uint8_t i, uint8_t j, uint8_t k, uint8_t l)
{
	uint32_t w = 0;
	w |= (l << (8 * 0));
	// printf("%02x\n", w);
	w |= (k << (8 * 1));
	// printf("%02x\n", w);
	w |= (j << (8 * 2));
	// printf("%02x\n", w);
	w |= (i << (8 * 3));
	// printf("%02x\n", w);

	// printf("%02x\n", w >> 2);
	return w;
}

void swap(uint8_t arr[4][4], uint8_t i_1, uint8_t j_1, uint8_t i_2, uint8_t j_2)
{
	uint8_t tmp = arr[i_1][j_1];
	arr[i_1][j_1] = arr[i_2][j_2];
	arr[i_2][j_2] = tmp;
}

void printArr(uint8_t state[4][4])
{
	int i, j;
    printf("state = {\n");
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
            printf("0x%02x, ", state[i][j]);
		}
        printf("\n");
	}
    printf("}\n");
}

void assertEqual(uint8_t actual[4][4], uint8_t expected[4][4], const char *operation)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (actual[i][j] != expected[i][j]) {
				fprintf(stderr, "%s: ASSERTION FAILURE: actual[%d][%d] != expected[%d][%d], got 0x%02x, want 0x%02x\n", operation, i, j, i, j, actual[i][j], expected[i][j]);
				exit(EXIT_FAILURE);
			}
		}
	}
	fprintf(stdout, "operation \"%s\" succeeded\n", operation);
}

void assertStringEquals(uint8_t actual[4][4], const char *expected, const char *operation)
{
	char buf[64];
	arrToStr(buf, actual);
	if (strcmp((char *)actual, buf) != 0) {
		fprintf(stderr, "%s: ASSERTION FAILURE\ngot:  %s, want: %s\n", operation, buf, expected);
		exit(EXIT_FAILURE);
	}
	fprintf(stdout, "operation \"%s\" succeeded\n", operation);
}

void assertEqualN(uint8_t actual[], uint8_t expected[], int n, const char *operation)
{
	int i;
	for (i = 0; i < n; i++) {
		if (actual[i] != expected[i]) {
			fprintf(stderr, "%s: ASSERTION FAILURE: actual[%d] != expected[%d], got 0x%02x, want 0x%02x\n", operation, i, i, actual[i], expected[i]);
			exit(EXIT_FAILURE);
		}
	}
	fprintf(stdout, "operation \"%s\" succeeded\n", operation);
}

void assertEqual32(uint32_t actual[], uint32_t expected[], int n, const char *operation)
{
	int i;
	for (i = 0; i < n; i++) {
		if (actual[i] != expected[i]) {
			fprintf(stderr, "%s: ASSERTION FAILURE: actual[%d] != expected[%d], got 0x%02x, want 0x%02x\n", operation, i, i, actual[i], expected[i]);
			exit(EXIT_FAILURE);
		}
	}
	fprintf(stdout, "operation \"%s\" succeeded\n", operation);
}

void arrToStr(char buf[], uint8_t arr[4][4])
{
	memset(buf, 0, 64);
	int i, j;
	char tmp[16];
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			sprintf(tmp, "%02x", arr[j][i]);
			strcat(buf, tmp);
		}
	}
}

void strToArr(uint8_t arr[4][4], char buf[], int n)
{
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			// sscanf(&buf[i * 4 + j], "%02x", arr[0][0]);
		}
	}
}

void printState(char *msg, uint8_t state[4][4], int round)
{
	char buf[64];
	arrToStr(buf, state);
	printf("round[%d].%s\t%s\n", round, msg, buf);
}

void printKeySchedule(int round, uint32_t key[], int start)
{
	int i;
	printf("round[%d].k_sch\t", round);
	for (i = start; i < start + 4; i++) {
		// printf("(%d)", i - start);
		printf("%02x", key[i]);
	}
	printf("\n");
}