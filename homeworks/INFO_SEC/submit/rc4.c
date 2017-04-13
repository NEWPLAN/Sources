#include <stdio.h>
#include <string.h>

void InitS(unsigned char *s)
{
	int i;
	for (i = 0; i < 256; i += 8)
	{
		s[i] = i;
		s[i + 1] = i + 1;
		s[i + 2] = i + 2;
		s[i + 3] = i + 3;
		s[i + 4] = i + 4;
		s[i + 5] = i + 5;
		s[i + 6] = i + 6;
		s[i + 7] = i + 7;
	}
}

void InitT(unsigned char *t, char *key, int keylen)
{
	int i;
	for (i = 0; i < 256; i += 4)
	{
		t[i] = key[i % keylen];
		t[i + 1] = key[(i + 1) % keylen];
		t[i + 2] = key[(i + 2) % keylen];
		t[i + 3] = key[(i + 3) % keylen];
	}
}

inline void Swap(unsigned char *s, int first, int last)
{
	unsigned char temp;
	temp = s[first];
	s[first] = s[last];
	s[last] = temp;
}

void InitPofS(unsigned char *s, unsigned char *t)
{
	int i;
	int j = 0;
	for (i = 0; i < 256; i += 4)
	{
		j = (j + s[i] + t[i]) % 256;
		Swap(s, i, j);

		j = (j + s[i + 1] + t[i + 1]) % 256;
		Swap(s, i + 1, j);

		j = (j + s[i + 2] + t[i + 2]) % 256;
		Swap(s, i + 2, j);

		j = (j + s[i + 3] + t[i + 3]) % 256;
		Swap(s, i + 3, j);
	}
}


void RC4(unsigned char *input, int in_len, unsigned char *output, int*out_len, unsigned char *key, int keylen)
{
	unsigned char* out = output;
	unsigned char s[256] = { 0 };
	unsigned char t[256] = { 0 };
	int i = 0;
	int j = 0;
	int k = 0;
	int temp;

	InitS(s);
	InitT(t, (char*)key, keylen);
	InitPofS(s, t);

	for (int index = 0; index < in_len; index++)
	{
		i = (i + 1) % 256;
		j = (j + s[i]) % 256;
		Swap(s, i, j);
		temp = (s[i] + s[j]) % 256;
		out[index] = s[temp] ^ input[index];
	}
	*out_len = in_len;
}