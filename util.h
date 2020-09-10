#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

void swap(uint8_t arr[4][4], uint8_t i_1, uint8_t j_1, uint8_t i_2, uint8_t j_2);

void printArr(uint8_t arr[4][4]);

void assertEqual(uint8_t actual[4][4], uint8_t expected[4][4], const char *msg);
void assertEqualN(uint8_t actual[], uint8_t expected[], int n, const char *operation);

uint32_t word(uint8_t i, uint8_t j, uint8_t k, uint8_t l);

void assertStringEquals(uint8_t actual[4][4], const char *expected, const char *msg);

void printState(char *msg, uint8_t state[4][4], int round);
void arrToStr(char buf[], uint8_t arr[4][4]);
void strToArr(uint8_t arr[4][4], char buf[], int n);

void printKeySchedule(int round, uint32_t key[], int start);
void assertEqual32(uint32_t actual[], uint32_t expected[], int n, const char *operation);
#endif /* UTIL_H */