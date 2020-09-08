#include <stdint.h>

void swap(uint8_t arr[4][4], uint8_t i_1, uint8_t j_1, uint8_t i_2, uint8_t j_2);

void printArr(uint8_t arr[4][4]);

void assertEqual(uint8_t actual[4][4], uint8_t expected[4][4], const char *msg);

uint32_t word(uint8_t i, uint8_t j, uint8_t k, uint8_t l);