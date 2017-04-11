#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// The number of columns in a state in AES. This is a constant value = 4 in AES. Also marked as Nb.
#define NUMBER_OF_COLUMNS 4
// Size of state in AES. Also constant value = 16.
#define BLOCK_SIZE 16

/*
______________________________________________________________________________________________________________________
______________________________________________________________________________________________________________________

Global variables and constants
______________________________________________________________________________________________________________________
______________________________________________________________________________________________________________________
*/

// Substitution box
// https://en.wikipedia.org/wiki/Rijndael_S-box
static const uint8_t sbox[256] =
{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

// Inverse substitution box
// https://en.wikipedia.org/wiki/Rijndael_S-box
static const uint8_t inv_sbox[256] =
{
	0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
	0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
	0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
	0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
	0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
	0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
	0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
	0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
	0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
	0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
	0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
	0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
	0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
	0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
	0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};

// The round constant word array.
// https://en.wikipedia.org/wiki/Rijndael_key_schedule
static const uint8_t rcon[256] =
{
	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
	0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
	0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
	0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
	0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
	0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
	0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
	0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
	0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
	0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
	0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
	0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
	0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
	0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
	0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
	0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
};

// Points to inserted key
unsigned char* key;
// Points to the round keys.
uint8_t *round_key;
// Point to initialization vector. Used in CBC mode.
uint8_t iv[16];
// Points to a state of currently processed data block.
uint8_t* state;

// The number of 32 bit words in a key.
uint8_t Nk = 4;
// The number of rounds in AES Cipher.
uint8_t Nr = 10;

// Points to inserted mode (CBC|ECB)
char* mode;
// Points to inserted type (e = encryption|d = decryption)
char* type;
// Points to input file
FILE* input_file;
// Points to output file
FILE* output_file;

/*
______________________________________________________________________________________________________________________
______________________________________________________________________________________________________________________

AES implementation
______________________________________________________________________________________________________________________
______________________________________________________________________________________________________________________
*/

/*
___________________________________________________________

This function produces Nb(Nr+1) round keys.
The round keys are used in each round to decrypt the states.
https://en.wikipedia.org/wiki/Rijndael_key_schedule
___________________________________________________________
*/
void keyExpansion()
{
	uint32_t i, j, k;
	uint8_t tmp[4]; // Used for the column/row operations

	// The first round key is the key itself.
	for (i = 0; i < Nk; i++)
	{
		round_key[i * 4 + 0] = key[i * 4 + 0];
		round_key[i * 4 + 1] = key[i * 4 + 1];
		round_key[i * 4 + 2] = key[i * 4 + 2];
		round_key[i * 4 + 3] = key[i * 4 + 3];
	}

	// All other round keys are found from the previous round keys.
	for (; i < (NUMBER_OF_COLUMNS * (Nr + 1)); i++)
	{
		for (j = 0; j < 4; j++) { tmp[j] = round_key[(i - 1) * 4 + j]; }
		if (i % Nk == 0)
		{
			// RotWord()
			k = tmp[0];
			tmp[0] = tmp[1];
			tmp[1] = tmp[2];
			tmp[2] = tmp[3];
			tmp[3] = k;
			// Subword()
			tmp[0] = sbox[tmp[0]];
			tmp[1] = sbox[tmp[1]];
			tmp[2] = sbox[tmp[2]];
			tmp[3] = sbox[tmp[3]];
			// The number of 32 bit words in a key.
			tmp[0] =  tmp[0] ^ rcon[i / Nk];
		}
		else if (Nk > 6 && i % Nk == 4)
		{
			// Subword()
			tmp[0] = sbox[tmp[0]];
			tmp[1] = sbox[tmp[1]];
			tmp[2] = sbox[tmp[2]];
			tmp[3] = sbox[tmp[3]];
		}
		round_key[i * 4 + 0] = round_key[(i - Nk) * 4 + 0] ^ tmp[0];
		round_key[i * 4 + 1] = round_key[(i - Nk) * 4 + 1] ^ tmp[1];
		round_key[i * 4 + 2] = round_key[(i - Nk) * 4 + 2] ^ tmp[2];
		round_key[i * 4 + 3] = round_key[(i - Nk) * 4 + 3] ^ tmp[3];
	}
}

/*
___________________________________________________________

Alternative to this function is to use precalculated tables
here: https://en.wikipedia.org/wiki/Rijndael_mix_columns
___________________________________________________________
*/
uint8_t xtime(uint8_t x)
{
	return ( (x << 1) ^ (((x >> 7) & 1) * 0x1b) );
}

/*
___________________________________________________________

Alternative to this function is to use precalculated tables
here: https://en.wikipedia.org/wiki/Rijndael_mix_columns
___________________________________________________________
*/
uint8_t multiply(uint8_t x, uint8_t y)
{
	return (((y & 1) * x) ^
	        ((y >> 1 & 1) * xtime(x)) ^
	        ((y >> 2 & 1) * xtime(xtime(x))) ^
	        ((y >> 3 & 1) * xtime(xtime(xtime(x)))) ^
	        ((y >> 4 & 1) * xtime(xtime(xtime(xtime(x))))));
}

/*
___________________________________________________________

The SubBytes Function substitutes the values in the
state matrix with values in an S-box.
___________________________________________________________
*/
void subBytes()
{
	uint8_t i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			state[j * NUMBER_OF_COLUMNS + i] = sbox[state[j * NUMBER_OF_COLUMNS + i]];
		}
	}
}

/*
___________________________________________________________

This function adds the round key to state.
The round key is added to the state by an XOR function.
___________________________________________________________
*/
void addRoundKey(uint8_t round)
{
	uint8_t i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			state[i * NUMBER_OF_COLUMNS + j] ^= round_key[round  * NUMBER_OF_COLUMNS  * 4 + i * NUMBER_OF_COLUMNS + j];
		}
	}
}

/*
___________________________________________________________

The ShiftRows() function shifts the rows in the state to the left.
Each row is shifted with different offset.
___________________________________________________________
*/
void shiftRows()
{
	uint8_t tmp;

	// move second row 1 columns to the left
	tmp = state[1];
	state[1] = state[1 * NUMBER_OF_COLUMNS + 1];
	state[1 * NUMBER_OF_COLUMNS + 1] = state[2 * NUMBER_OF_COLUMNS + 1];
	state[2 * NUMBER_OF_COLUMNS + 1] = state[3 * NUMBER_OF_COLUMNS + 1];
	state[3 * NUMBER_OF_COLUMNS + 1] = tmp;

	// move third row 2 columns to the left
	tmp = state[2];
	state[2] = state[2 * NUMBER_OF_COLUMNS + 2];
	state[2 * NUMBER_OF_COLUMNS + 2] = tmp;

	tmp = state[1 * NUMBER_OF_COLUMNS + 2];
	state[1 * NUMBER_OF_COLUMNS + 2] = state[3 * NUMBER_OF_COLUMNS  + 2];
	state[3 * NUMBER_OF_COLUMNS + 2] = tmp;

	// move fourth row 3 columns to the left
	tmp = state[3];
	state[3] = state[3 * NUMBER_OF_COLUMNS + 3];
	state[3 * NUMBER_OF_COLUMNS + 3] = state[2 * NUMBER_OF_COLUMNS + 3];
	state[2 * NUMBER_OF_COLUMNS + 3] = state[1 * NUMBER_OF_COLUMNS + 3];
	state[1 * NUMBER_OF_COLUMNS + 3] = tmp;
}

/*
___________________________________________________________

MixColumns function mixes the columns of the state matrix
source: https://en.wikipedia.org/wiki/Rijndael_mix_columns
___________________________________________________________
*/

void mixColumns()
{
	uint8_t i, tmp1, tmp2, tmp3;

	for (i = 0; i < 4; i++)
	{
		tmp3 = state[i * 4 + 0];
		tmp1 = state[i * 4 + 0] ^ state[i * 4 + 1] ^ state[i * 4 + 2] ^ state[i * 4 + 3];

		tmp2 = state[i * 4 + 0] ^ state[i * 4 + 1]; tmp2 = xtime(tmp2); state[i * 4 + 0] ^= tmp2 ^ tmp1;
		tmp2 = state[i * 4 + 1] ^ state[i * 4 + 2]; tmp2 = xtime(tmp2); state[i * 4 + 1] ^= tmp2 ^ tmp1;
		tmp2 = state[i * 4 + 2] ^ state[i * 4 + 3]; tmp2 = xtime(tmp2); state[i * 4 + 2] ^= tmp2 ^ tmp1;
		tmp2 = state[i * 4 + 3] ^ tmp3; tmp2 = xtime(tmp2); state[i * 4 + 3] ^= tmp2 ^ tmp1;
	}
}

/*
___________________________________________________________

Encipher given data block stored in state.
___________________________________________________________
*/
void encipher()
{
	uint8_t round = 0;

	addRoundKey(0);

	// The first Nr-1 rounds are identical.
	// These Nr-1 rounds are executed in the loop below.
	for (round = 1; round < Nr; round++)
	{
		subBytes();
		shiftRows();
		mixColumns();
		addRoundKey(round);
	}

	// The MixColumns function is not here in the last round.
	subBytes();
	shiftRows();
	addRoundKey(Nr);
}

/*
___________________________________________________________

SubBytes function substitutes values in the
state matrix with values in an S-box.
___________________________________________________________
*/
void invSubBytes()
{
	uint8_t i, j;

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			state[j * 4 + i] = inv_sbox[state[j * 4 + i]];
		}
	}
}

/*
___________________________________________________________

Inverse operation to mix columns operation.
___________________________________________________________
*/
void invMixColumns()
{
	uint8_t i, a, b, c, d;

	for (i = 0; i < 4; i++)
	{
		a = state[i * 4];
		b = state[i * 4 + 1];
		c = state[i * 4 + 2];
		d = state[i * 4 + 3];

		state[i * 4] = multiply(a, 0x0e) ^ multiply(b, 0x0b) ^ multiply(c, 0x0d) ^ multiply(d, 0x09);
		state[i * 4 + 1] = multiply(a, 0x09) ^ multiply(b, 0x0e) ^ multiply(c, 0x0b) ^ multiply(d, 0x0d);
		state[i * 4 + 2] = multiply(a, 0x0d) ^ multiply(b, 0x09) ^ multiply(c, 0x0e) ^ multiply(d, 0x0b);
		state[i * 4 + 3] = multiply(a, 0x0b) ^ multiply(b, 0x0d) ^ multiply(c, 0x09) ^ multiply(d, 0x0e);
	}
}

/*
___________________________________________________________

Inverse operation to shift rows operation.
___________________________________________________________
*/
void invShiftRows()
{
	uint8_t tmp;

	// Rotate second row 1 columns to right
	tmp = state[3 * 4 + 1];
	state[3 * 4 + 1] = state[2 * 4 + 1];
	state[2 * 4 + 1] = state[1 * 4 + 1];
	state[1 * 4 + 1] = state[1];
	state[1] = tmp;

	// Rotate third row 2 columns to right
	tmp = state[2];
	state[2] = state[2 * 4 + 2];
	state[2 * 4 + 2] = tmp;

	tmp = state[1 * 4 + 2];
	state[1 * 4 + 2] = state[3 * 4 + 2];
	state[3 * 4 + 2] = tmp;

	// Rotate fourth row 3 columns to right
	tmp = state[3];
	state[3] = state[1 * 4 + 3];
	state[1 * 4 + 3] = state[2 * 4 + 3];
	state[2 * 4 + 3] = state[3 * 4 + 3];
	state[3 * 4 + 3] = tmp;
}

/*
___________________________________________________________

Decipher
___________________________________________________________
*/
void decipher()
{
	uint8_t round = 0;

	// Adding first round key to the state before starting rounds.
	addRoundKey(Nr);

	for (round = Nr - 1; round > 0; round--)
	{
		invShiftRows();
		invSubBytes();
		addRoundKey(round);
		invMixColumns();
	}

	invShiftRows();
	invSubBytes();
	addRoundKey(0);
}

/*
___________________________________________________________

Decrypt CBC
___________________________________________________________
*/
void decryptCBC()
{
	uint8_t i = 0;
	uint8_t read = 0;

	state = (uint8_t *) malloc(BLOCK_SIZE);
	if (state == NULL) { printf("Failed to allocate memory."); exit(1); }

	// iterate over file
	while ((read = fread(state, 1, BLOCK_SIZE, input_file)) == BLOCK_SIZE)
	{

		keyExpansion();
		decipher();

		// xor state with key
		for (i = 0; i < BLOCK_SIZE; i++) { state[i] ^= iv[i]; }
		// write block to file
		fwrite(state, 1, BLOCK_SIZE, output_file);
		// store new vector
		for (i = 0; i < BLOCK_SIZE; i++) { iv[i] = state[i]; }
	}

	if (read > 0)   // there is one extra block
	{
		// fill remaining space with zeros
		memset(state + read, 0, BLOCK_SIZE - read);

		keyExpansion();
		decipher();

		// xor state with key
		for (i = 0; i < BLOCK_SIZE; i++) { state[i] ^= iv[i]; }

		// write block to file
		fwrite(state, 1, BLOCK_SIZE, output_file);
	}
	free(state);
}

/*
___________________________________________________________

Encrypt in CBC mode
___________________________________________________________
*/
void encryptCBC()
{
	uint8_t i = 0;
	uint8_t read = 0;

	state = (uint8_t *) malloc(BLOCK_SIZE);
	if (state == NULL) { printf("Failed to allocate memory."); exit(1); }

	while ((read = fread(state, 1, BLOCK_SIZE, input_file)) == BLOCK_SIZE)
	{
		// xor state with key
		for (i = 0; i < BLOCK_SIZE; i++) { state[i] ^= iv[i]; }

		keyExpansion();
		encipher();

		// write block to file
		fwrite(state, 1, BLOCK_SIZE, output_file);
		// store new vector
		for (i = 0; i < BLOCK_SIZE; i++) { iv[i] = state[i]; }
	}

	if (read > 0)   // there is one extra block
	{
		// fill remaining space with zeros
		memset(state + read, 0, BLOCK_SIZE - read);
		// xor state with key
		for (i = 0; i < BLOCK_SIZE; i++) { state[i] ^= iv[i]; }

		keyExpansion();
		encipher();

		// write block to file
		fwrite(state, 1, BLOCK_SIZE, output_file);
	}
	free(state);
}

/*
___________________________________________________________

Encrypt in ECB mode
___________________________________________________________
*/
void decryptECB()
{
	uint8_t read = 0;

	state = (uint8_t *) malloc(BLOCK_SIZE);
	if (state == NULL) { printf("Failed to allocate memory."); exit(1); }

	while ((read = fread(state, 1, BLOCK_SIZE, input_file)) == BLOCK_SIZE)
	{

		keyExpansion();
		decipher();

		fwrite(state, 1, BLOCK_SIZE, output_file);
	}

	if (read > 0)   // there is one extra block
	{
		// fill remaining space with zeros
		memset(state + read, 0, BLOCK_SIZE - read);

		keyExpansion();
		decipher();

		// write block to file
		fwrite(state, 1, BLOCK_SIZE, output_file);
	}
	free(state);
}

/*
___________________________________________________________

Decrypt in ECB mode
___________________________________________________________
*/
void encryptECB()
{
	uint8_t read = 0;

	state = (uint8_t *) malloc(BLOCK_SIZE);
	if (state == NULL) { printf("Failed to allocate memory."); exit(1); }

	while ((read = fread(state, 1, BLOCK_SIZE, input_file)) == BLOCK_SIZE)
	{

		keyExpansion();
		encipher();

		fwrite(state, 1, BLOCK_SIZE, output_file);
	}

	if (read > 0)   // there is one extra block
	{
		// fill remaining space with zeros
		memset(state + read, 0, BLOCK_SIZE - read);

		keyExpansion();
		encipher();

		// write block to file
		fwrite(state, 1, BLOCK_SIZE, output_file);
	}
	free(state);
}

/*
______________________________________________________________________________________________________________________
______________________________________________________________________________________________________________________

Program start
______________________________________________________________________________________________________________________
______________________________________________________________________________________________________________________
*/

/*
___________________________________________________________

Prints help guide
___________________________________________________________
*/
void print_help()
{
	printf("Usage: AES (-if|--input-file) <file name>          # file to encrypt/decrypt\n"
	       "           (-of|--output-file) <file name>         # file to output encrypted/decrypted input file\n"
	       "           (-m|--mode) <CBC|ECB>                   # encryption/decryption mode\n"
	       "           (-e|--encode|-d|--decode)               # pick encryption or decryption\n"
	       "           (-k|--key) <key to use>                 # Enter in hex format without spaces.\n"
	       "                                                   # Example: 2B7E151628AED2A6ABF7158809CF4F3C\n"
	       "           [-iv|--initialization-vector] <vector>  # required and used only with CBC mode. Enter in hex format.\n");
}


/*
___________________________________________________________

Helper function to convert hex input to byte
___________________________________________________________
*/
uint8_t hextobyte(const char *s)
{
	return
	    (((s[0] >= 'A' && s[0] <= 'Z') ? (10 + s[0] - 'A') :
	      (s[0] >= 'a' && s[0] <= 'z') ? (10 + s[0] - 'a') :
	      (s[0] >= '0' && s[0] <= '9') ? (s[0] - '0') : 0) << 4) |
	    ((s[1] >= 'A' && s[1] <= 'Z') ? (10 + s[1] - 'A') :
	     (s[1] >= 'a' && s[1] <= 'z') ? (10 + s[1] - 'a') :
	     (s[1] >= '0' && s[1] <= '9') ? (s[1] - '0') : 0);
}

/*
___________________________________________________________

Program main
___________________________________________________________
*/
int main ( int argc, char *argv[] )
{
	bool input_file_inserted = false;
	bool output_file_inserted = false;
	bool key_inserted = false;
	bool mode_inserted = false;
	bool type_inserted = false;
	bool iv_inserted = false;

	if (argc > 1)
	{
		uint8_t i;
		for (i = 1; i < argc; i++)
		{
			if ((strcmp("-h", argv[i]) == 0) || (strcmp("--help", argv[i]) == 0))
			{
				print_help();
				exit(0);
			}
			else if ((strcmp("-if", argv[i]) == 0) || (strcmp("--input-file", argv[i]) == 0))
			{
				i++; input_file_inserted = true;
				input_file = fopen(argv[i], "rb");
				if (input_file == NULL)
				{
					printf("Input file does not exists. If you need help then run program with -h or --help.\n");
					exit(1);
				}
			}
			else if ((strcmp("-of", argv[i]) == 0) || (strcmp("--output-file", argv[i]) == 0))
			{
				i++; output_file_inserted = true;
				output_file = fopen(argv[i], "wb");
				// output_file = fopen(argv[i], "rb");
				// if(output_file == NULL) {
				// 	output_file = fopen(argv[i], "wb");
				// } else {
				//     printf("Output file %s already exists. If you need help then run program with -h or --help.\n", argv[i]);
				// 	exit(1);
				// }
			}
			else if ((strcmp("-k", argv[i]) == 0) || (strcmp("--key", argv[i]) == 0))
			{
				i++; key_inserted = true;
				if (strlen(argv[i]) == 32 || strlen(argv[i]) == 48 || strlen(argv[i]) == 64)
				{
					const char* hex_key = argv[i];
					uint8_t key_size = (strlen(hex_key) / 2);
					Nk = key_size / 4;
					Nr += (Nk - 4);

					round_key = malloc((Nr + 1) * key_size);
					if (round_key == NULL) { printf("Failed to allocate memory."); exit(1); }
					key = malloc(key_size);
					if (key == NULL) { printf("Failed to allocate memory."); exit(1); }

					uint8_t k = 0;
					// Easier would be use this, but 'h' is not supported in MinGW windows at the moment. sscanf(hex_key + (k * 2), "%2hhx", key + k);
					for (; k < key_size; k++) { key[k] = hextobyte(hex_key + (k * 2)); };

				}
				else
				{
					printf("Incorrect key size %d. Supported sizes are 32hex (128b) 48hex (192b) and 64hex (256b). If you need help then run program with -h or --help.\n", (int)strlen(argv[i]));
					exit(1);
				}
			}
			else if ((strcmp("-e", argv[i]) == 0) || (strcmp("--encode", argv[i]) == 0))
			{
				type_inserted = true;
				type = "e";
			}
			else if ((strcmp("-d", argv[i]) == 0) || (strcmp("--decode", argv[i]) == 0))
			{
				type_inserted = true;
				type = "d";
			}
			else if ((strcmp("-m", argv[i]) == 0) || (strcmp("--mode", argv[i]) == 0))
			{
				i++; mode_inserted = true;
				if ((strcmp("CBC", argv[i]) == 0) || (strcmp("ECB", argv[i]) == 0))
				{
					mode = argv[i];
				}
				else
				{
					printf("Unknown mode type. If you need help then run program with -h or --help.\n");
					exit(1);
				}
			}
			else if ((strcmp("-iv", argv[i]) == 0) || (strcmp("--initialization-vector", argv[i]) == 0))
			{
				i++; iv_inserted = true;
				if (strlen(argv[i]) == 32)
				{
					char* hex_iv = argv[i];

					uint8_t k;
					// sscanf(hex_iv + (k * 2), "%2hhx", &iv[k]);
					for (k = 0; k < 16; k++) { iv[k] = hextobyte(hex_iv + (k * 2)); };
				}
				else
				{
					printf("Incorrect initialization vector size %d. Supported size is 32hex (128b). If you need help then run program with -h or --help.\n", (int)strlen(argv[i]));
					exit(1);
				}
			}
			else
			{
				printf("Unknown argument. If you need help then run program with -h or --help.\n");
				exit(1);
			}
		}
	}

	// arguments parsed checked that all arguments are inserted
	if (input_file_inserted && output_file_inserted && key_inserted && mode_inserted && type_inserted)
	{
		if ((strcmp("CBC", mode) == 0) && !iv_inserted)
		{
			printf("Missing initialization vector. If you need help then run program with -h or --help.\n");
			exit(1);
		}
	}
	else
	{
		printf("Missing argument/s. If you need help then run program with -h or --help.\n");
		exit(1);
	}

	if ((strcmp("CBC", mode) == 0))
	{
		if ((strcmp("d", type) == 0))
		{
			printf("Decryption in CBC mode\n");
			decryptCBC();
		}
		else
		{
			printf("Encryption in CBC mode\n");
			encryptCBC();
		}
	}
	else
	{
		if ((strcmp("d", type) == 0))
		{
			printf("Decryption in ECB mode\n");
			decryptECB();
		}
		else
		{
			printf("Encryption in ECB mode\n");
			encryptECB();
		}
	}

	free(round_key);
	free(key);
	fclose(input_file);
	fclose(output_file);
}
