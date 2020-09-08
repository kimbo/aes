#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint32_t word(uint8_t i, uint8_t j, uint8_t k, uint8_t l)
{
	uint32_t w = 0;
	w |= (i << (8 * 3));
	w |= (j << (8 * 2));
	w |= (k << (8 * 1));
	w |= (l << (8 * 0));
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