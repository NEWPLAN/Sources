// AESDllTest.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "AESDllTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CAESDllTestApp, CWinApp)
END_MESSAGE_MAP()


// CAESDllTestApp 构造

CAESDllTestApp::CAESDllTestApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAESDllTestApp 对象

CAESDllTestApp theApp;


// CAESDllTestApp 初始化

BOOL CAESDllTestApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


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


// The round constant word array.
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
//unsigned char* key;
// Points to the round keys.
//uint8_t round_key[60*8*2];
//uint8_t* round_key;

// Points to a state of currently processed data block.
//uint8_t state[BLOCK_SIZE];

// The number of 32 bit words in a key.
uint8_t Nk;
// The number of rounds in AES Cipher.
uint8_t Nr;



/*
___________________________________________________________

This function produces Nb(Nr+1) round keys.
The round keys are used in each round to decrypt the states.
___________________________________________________________
*/
void keyExpansion(unsigned char* round_key, unsigned char* key)
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
			tmp[0] = tmp[0] ^ rcon[i / Nk];
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
unsigned char pretable[256] =
{
	0x0, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e,
	0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e,
	0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e,
	0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x72, 0x74, 0x76, 0x78, 0x7a, 0x7c, 0x7e,
	0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c, 0x8e, 0x90, 0x92, 0x94, 0x96, 0x98, 0x9a, 0x9c, 0x9e,
	0xa0, 0xa2, 0xa4, 0xa6, 0xa8, 0xaa, 0xac, 0xae, 0xb0, 0xb2, 0xb4, 0xb6, 0xb8, 0xba, 0xbc, 0xbe,
	0xc0, 0xc2, 0xc4, 0xc6, 0xc8, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd6, 0xd8, 0xda, 0xdc, 0xde,
	0xe0, 0xe2, 0xe4, 0xe6, 0xe8, 0xea, 0xec, 0xee, 0xf0, 0xf2, 0xf4, 0xf6, 0xf8, 0xfa, 0xfc, 0xfe,
	0x1b, 0x19, 0x1f, 0x1d, 0x13, 0x11, 0x17, 0x15, 0xb, 0x9, 0xf, 0xd, 0x3, 0x1, 0x7, 0x5,
	0x3b, 0x39, 0x3f, 0x3d, 0x33, 0x31, 0x37, 0x35, 0x2b, 0x29, 0x2f, 0x2d, 0x23, 0x21, 0x27, 0x25,
	0x5b, 0x59, 0x5f, 0x5d, 0x53, 0x51, 0x57, 0x55, 0x4b, 0x49, 0x4f, 0x4d, 0x43, 0x41, 0x47, 0x45,
	0x7b, 0x79, 0x7f, 0x7d, 0x73, 0x71, 0x77, 0x75, 0x6b, 0x69, 0x6f, 0x6d, 0x63, 0x61, 0x67, 0x65,
	0x9b, 0x99, 0x9f, 0x9d, 0x93, 0x91, 0x97, 0x95, 0x8b, 0x89, 0x8f, 0x8d, 0x83, 0x81, 0x87, 0x85,
	0xbb, 0xb9, 0xbf, 0xbd, 0xb3, 0xb1, 0xb7, 0xb5, 0xab, 0xa9, 0xaf, 0xad, 0xa3, 0xa1, 0xa7, 0xa5,
	0xdb, 0xd9, 0xdf, 0xdd, 0xd3, 0xd1, 0xd7, 0xd5, 0xcb, 0xc9, 0xcf, 0xcd, 0xc3, 0xc1, 0xc7, 0xc5,
	0xfb, 0xf9, 0xff, 0xfd, 0xf3, 0xf1, 0xf7, 0xf5, 0xeb, 0xe9, 0xef, 0xed, 0xe3, 0xe1, 0xe7, 0xe5
};

/*
___________________________________________________________

The SubBytes Function substitutes the values in the
state matrix with values in an S-box.
___________________________________________________________
*/
inline void subBytes(unsigned char* state)
{
	//uint8_t i, j;

	//for (i = 0; i < 4; i++)
	//{
	//	state[0 + i] = sbox[state[0 + i]];
	//	state[4 + i] = sbox[state[4 + i]];
	//	state[8 + i] = sbox[state[8 + i]];
	//	state[12+ i] = sbox[state[12+ i]];
	//}
	state[0] = sbox[state[0]];
	state[1] = sbox[state[1]];
	state[2] = sbox[state[2]];
	state[3] = sbox[state[3]];
	state[4] = sbox[state[4]];
	state[5] = sbox[state[5]];
	state[6] = sbox[state[6]];
	state[7] = sbox[state[7]];
	state[8] = sbox[state[8]];
	state[9] = sbox[state[9]];
	state[10] = sbox[state[10]];
	state[11] = sbox[state[11]];
	state[12] = sbox[state[12]];
	state[13] = sbox[state[13]];
	state[14] = sbox[state[14]];
	state[15] = sbox[state[15]];
}

/*
___________________________________________________________

This function adds the round key to state.
The round key is added to the state by an XOR function.
___________________________________________________________
*/
inline void addRoundKey(uint8_t round, unsigned char* state, unsigned char* round_key)
{
	uint8_t i, j;

	for (i = 0; i < 4; i++)
	{
		state[i * NUMBER_OF_COLUMNS + 0] ^= round_key[round  * 16 + i * NUMBER_OF_COLUMNS + 0];
		state[i * NUMBER_OF_COLUMNS + 1] ^= round_key[round  * 16 + i * NUMBER_OF_COLUMNS + 1];
		state[i * NUMBER_OF_COLUMNS + 2] ^= round_key[round  * 16 + i * NUMBER_OF_COLUMNS + 2];
		state[i * NUMBER_OF_COLUMNS + 3] ^= round_key[round  * 16 + i * NUMBER_OF_COLUMNS + 3];
	}
}

/*
___________________________________________________________

The ShiftRows() function shifts the rows in the state to the left.
Each row is shifted with different offset.
___________________________________________________________
*/
inline void shiftRows(unsigned char* state)
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
	state[1 * NUMBER_OF_COLUMNS + 2] = state[3 * NUMBER_OF_COLUMNS + 2];
	state[3 * NUMBER_OF_COLUMNS + 2] = tmp;

	// move fourth row 3 columns to the left
	tmp = state[3];
	state[3] = state[3 * NUMBER_OF_COLUMNS + 3];
	state[3 * NUMBER_OF_COLUMNS + 3] = state[2 * NUMBER_OF_COLUMNS + 3];
	state[2 * NUMBER_OF_COLUMNS + 3] = state[1 * NUMBER_OF_COLUMNS + 3];
	state[1 * NUMBER_OF_COLUMNS + 3] = tmp;
}

inline void mixColumns(unsigned char* state)
{
	uint8_t i, tmp1, tmp2, tmp3;

	for (i = 0; i < 4; i++)
	{
		tmp3 = state[i * 4 + 0];
		tmp1 = state[i * 4 + 0] ^ state[i * 4 + 1] ^ state[i * 4 + 2] ^ state[i * 4 + 3];

		tmp2 = state[i * 4 + 0] ^ state[i * 4 + 1];
		tmp2 = pretable[tmp2];
		state[i * 4 + 0] ^= tmp2 ^ tmp1;
		tmp2 = state[i * 4 + 1] ^ state[i * 4 + 2];
		tmp2 = pretable[tmp2];
		state[i * 4 + 1] ^= tmp2 ^ tmp1;
		tmp2 = state[i * 4 + 2] ^ state[i * 4 + 3];
		tmp2 = pretable[tmp2];
		state[i * 4 + 2] ^= tmp2 ^ tmp1;
		tmp2 = state[i * 4 + 3] ^ tmp3;
		tmp2 = pretable[tmp2];
		state[i * 4 + 3] ^= tmp2 ^ tmp1;
	}
}

/*
___________________________________________________________

Encipher given data block stored in state.
___________________________________________________________
*/
inline void encipher(unsigned char* state, unsigned char* round_key)
{
	uint8_t round = 0;

	addRoundKey(0, state, round_key);

	// The first Nr-1 rounds are identical.
	// These Nr-1 rounds are executed in the loop below.
	for (round = 1; round < Nr; round++)
	{
		subBytes(state);
		shiftRows(state);
		mixColumns(state);
		addRoundKey(round, state, round_key);
	}

	// The MixColumns function is not here in the last round.
	subBytes(state);
	shiftRows(state);
	addRoundKey(Nr, state, round_key);
}

typedef struct ARGS
{
	unsigned char* inputs;//输入
	unsigned char* outputs;//输出
	//unsigned char* round_key;
	unsigned char* key;
	//unsigned char* state;
	int finfish;//运行结束
	int inputlen;
	//int outputlen;
	//int keylen;
	//int index;//第几个线程
} ArgsType;

unsigned int __stdcall thread(void* lpVoid)
{
	ArgsType* args = (ArgsType*) lpVoid;
	unsigned char state[BLOCK_SIZE] = { 0 };//每一轮的输入状态
	unsigned char Mkey[16 * 60] = { 0 };

	keyExpansion(Mkey, args->key);//密钥扩展

	int finished = 0;
	while (finished + BLOCK_SIZE <= args->inputlen)
	{
		for (int i = 0; i < BLOCK_SIZE; i++)
			state[i] = args->inputs[i];
		encipher(state, Mkey);
		for (int i = 0; i < BLOCK_SIZE; i++)
			args->outputs[i] = state[i];
		finished += BLOCK_SIZE;
		args->outputs += BLOCK_SIZE;
		args->inputs += BLOCK_SIZE;
	}
	args->finfish = 1;
	_endthreadex(0);
	return 0;
}

void func_entry(unsigned char *input, int in_len, unsigned char*output, int* out_len, unsigned char *key, int keylen)
{
	unsigned char state[BLOCK_SIZE] = { 0 };//压缩状态空间
	unsigned char Mkey[60 * 16] = { 0 }; //轮函数的K
	*out_len = (in_len + BLOCK_SIZE - 1) / BLOCK_SIZE * BLOCK_SIZE;
	ArgsType args[4];
	HANDLE hThread[4];
	int flags = 0;
	if (in_len >= 64)
	{
		flags = 1;
		for (int index = 0; index < 4; index++)
		{
			args[index].finfish = 0;
			args[index].inputs = input + ((in_len - in_len % 64) / 4) * index;
			args[index].outputs = output + ((in_len - in_len % 64) / 4) * index;
			args[index].inputlen = (in_len - in_len % 64) / 4;
			args[index].key = key;
			hThread[index] = (HANDLE)_beginthreadex(NULL, 0, thread, (void*)(&(args[index])), 0, NULL);
		}
		in_len %= 64;
		input += (in_len - in_len % 64);
		output += (in_len - in_len % 64);

	}
	if (in_len > 0)
	{
		MessageBox(NULL, TEXT("come here"), TEXT("notification"), 0);
		int read = 0;
		unsigned char* out = output;
		keyExpansion(Mkey, key);
		while (read + BLOCK_SIZE <= in_len)
		{
			for (int i = 0; i < BLOCK_SIZE; i++)
				state[i] = input[i];
			encipher(state, Mkey);
			for (int i = 0; i < BLOCK_SIZE; i++)
				out[i] = state[i];
			read += BLOCK_SIZE;
			out += BLOCK_SIZE;
			input += BLOCK_SIZE;
		}
		if (read < in_len)   // there is one extra block
		{
			// fill remaining space with zeros
			for (int i = 0; i < in_len - read; i++)
				state[i] = input[i];
			for (int i = in_len - read; i < BLOCK_SIZE; i++)
				state[i] = 0;
			encipher(state, Mkey);
			for (int i = 0; i < BLOCK_SIZE; i++)
				out[i] = state[i];
			read += BLOCK_SIZE;
		}
		//*out_len = read;
	}
	//for (int index = 0; index < 4; index++)
	//	while (!args[index].finfish);
	if (flags)
		for (int index = 0; index < 4; index++)
		{
			WaitForSingleObject(hThread[index], INFINITE);
			CloseHandle(hThread[index]);
		}
	if (*out_len > 128)
		MessageBox(NULL, TEXT("biger than 128BYTE"), TEXT("notification"), 0);
	return;
}

void Crypt_Enc_Block(unsigned char *input, int in_len, unsigned char*output, int *out_len, unsigned char *key, int keylen)
{
	//if (in_len == 32)
	//	MessageBox(NULL, TEXT("32BYTE"), TEXT("notification"), 0);
	//if (in_len == 16)
	//	MessageBox(NULL, TEXT("16BYTE"), TEXT("notification"), 0);
	//if (in_len == 128)
	//	MessageBox(NULL, TEXT("128BYTE"), TEXT("notification"), 0);
	Nk = keylen / 4;
	Nr = 10 + (Nk - 4);
	int outlen;
	//unsigned char* ooo = new unsigned char[100 * 1024 * 1024 + 20];
	//{
	//func_entry(input, in_len, ooo, &outlen, key, keylen);
	func_entry(input, in_len, output, out_len, key, keylen);
	//	*out_len = in_len;
	return;
	//}

	unsigned char state[BLOCK_SIZE] = { 0 };
	uint8_t Mkey[60 * 16] = { 0 };
	//round_key = Mkey;
	{
		int read = 0;
		unsigned char* out = output;
		keyExpansion(Mkey, key);
		while (read + BLOCK_SIZE <= in_len)
		{
			for (int i = 0; i < BLOCK_SIZE; i++)
				state[i] = input[i];
			encipher(state, Mkey);
			for (int i = 0; i < BLOCK_SIZE; i++)
				out[i] = state[i];
			read += BLOCK_SIZE;
			out += BLOCK_SIZE;
			input += BLOCK_SIZE;
		}
		if (read < in_len)   // there is one extra block
		{
			// fill remaining space with zeros
			for (int i = 0; i < in_len - read; i++)
				state[i] = input[i];
			for (int i = in_len - read; i < BLOCK_SIZE; i++)
				state[i] = 0;
			encipher(state, Mkey);
			for (int i = 0; i < BLOCK_SIZE; i++)
				out[i] = state[i];
			read += BLOCK_SIZE;
		}
		*out_len = read;
	}
	//if(outlen != *out_len)
	//	MessageBox(NULL, TEXT("length不一样！"), TEXT("notification"), 0);
	//if (outlen!=*out_len || !strncmp((const  char*)ooo, (const  char*)output,outlen))
	//	MessageBox(NULL, TEXT("不一样！"), TEXT("notification"), 0);
	//delete ooo;
	return;
}

