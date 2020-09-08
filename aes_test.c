#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "aes.h"
#include "util.h"

int main(int argc, char **argv)
{
	assert(ffAdd(0x57, 0x83) == 0xd4);
	assert(xtime(0x57) == 0xae);
	assert(xtime(0xae) == 0x47);
	assert(xtime(0x47) == 0x8e);
	assert(xtime(0x8e) == 0x07);
	assert(ffMultiply(0x57, 0x13) == 0xfe);

	assert(subWord(0x00102030) == 0x63cab704);
	assert(subWord(0x40506070) == 0x0953d051);
	assert(subWord(0x8090a0b0) == 0xcd60e0e7);
	assert(subWord(0xc0d0e0f0) == 0xba70e18c);

	assert(rotWord(0x09cf4f3c) == 0xcf4f3c09);
	assert(rotWord(0x2a6c7605) == 0x6c76052a);

	uint8_t key[16] = {
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
	};
	uint32_t expanded[44] = {
		0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c,
		0xa0fafe17, 0x88542cb1, 0x23a33939, 0x2a6c7605,
		0xf2c295f2, 0x7a96b943, 0x5935807a, 0x7359f67f,
		0x3d80477d, 0x4716fe3e, 0x1e237e44, 0x6d7a883b,
		0xef44a541, 0xa8525b7f, 0xb671253b, 0xdb0bad00,
		0xd4d1c6f8, 0x7c839d87, 0xcaf2b8bc, 0x11f915bc,
		0x6d88a37a, 0x110b3efd, 0xdbf98641, 0xca0093fd,
		0x4e54f70e, 0x5f5fc9f3, 0x84a64fb2, 0x4ea6dc4f,
		0xead27321, 0xb58dbad2, 0x312bf560, 0x7f8d292f,
		0xac7766f3, 0x19fadc21, 0x28d12941, 0x575c006e,
		0xd014f9a8, 0xc9ee2589, 0xe13f0cc8, 0xb6630ca6
	};

	uint32_t w[44];
	keyExpansion(key, w);
	/* Test that w == expanded */
	if (w[0] == expanded[0]) {

	}

	/**************/
	/*cipher tests*/
	/**************/
	uint8_t state[4][4] =  { {0x19,0xa0,0x9a,0xe9},
                         {0x3d,0xf4,0xc6,0xf8},
                         {0xe3,0xe2,0x8d,0x48},
                         {0xbe,0x2b,0x2a,0x08}};

	uint8_t sub[4][4] =    { {0xd4,0xe0,0xb8,0x1e},
				 {0x27,0xbf,0xb4,0x41},
				 {0x11,0x98,0x5d,0x52},
				 {0xae,0xf1,0xe5,0x30}};

	uint8_t shift[4][4] =  { {0xd4, 0xe0, 0xb8, 0x1e},
				 {0xbf, 0xb4, 0x41, 0x27},
				 {0x5d, 0x52, 0x11, 0x98},
				 {0x30, 0xae, 0xf1, 0xe5}};

	uint8_t mix[4][4] =    { {0x04, 0xe0, 0x48, 0x28},
				 {0x66, 0xcb, 0xf8, 0x06},
				 {0x81, 0x19, 0xd3, 0x26},
				 {0xe5, 0x9a, 0x7a, 0x4c}};

	// uint8_t round[4][4] =   { {0xa4, 0x68, 0x6b, 0x02},
	// 			  {0x9c, 0x9f, 0x5b, 0x6a},
	// 			  {0x7f, 0x35, 0xea, 0x50},
	// 			  {0xf2, 0x2b, 0x43, 0x49}};

	subBytes(state);
	assertEqual(state, sub, "subBytes()");

	shiftRows(state);
	assertEqual(state, shift, "shiftRows()");

	mixColumns(state);
	assertEqual(state, mix, "mixColumns()");

	addRoundKey(state, w, 4);
	/* Test that state == round */

	uint8_t in[16]  = { 0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
			    0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34 };
	uint8_t out[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	//uint8_t result[16] = { 0x39, 0x25, 0x84, 0x1d, 0x02, 0xdc, 0x09, 0xfb,
	//		       0xdc, 0x11, 0x85, 0x97, 0x19, 0x6a, 0x0b, 0x32 };
	cipher(in,out,w);
	/* Test that out == result */
}
