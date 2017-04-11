#include <stdio.h>
#include <string.h>

void InitS(unsigned char *s)
{
	int i;
	for (i = 0; i < 256; i++)
		s[i] = i;
}

void InitT(unsigned char *t, char *key, int keylen)
{
	int i;
	for (i = 0; i < 256; i++)
	{
		t[i] = key[i % keylen];
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
	for (i = 0; i < 256; i++)
	{
		j = (j + s[i] + t[i]) % 256;
		Swap(s, i, j);
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
