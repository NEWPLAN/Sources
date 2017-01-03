#include <stdio.h>
//exceed times
int mypow(int a, int n)
{
	if (n == 0)
		return 1;
	int xx = mypow(a, n / 2) % 10;
	if (xx == 0)
		return 0;
	return n % 2 ? xx * xx * a : xx * xx;
}
int pow2(int n)
{
	int abs[10][10 * 10] = {0};
	int i = 1, j, k;
	for (i = 1; i < 10; i++)
	{
		abs[i][0] = 1;
		for (j = 1; j < 100; j++)
			abs[i][j] = (abs[i][j - 1] * i) % 10;
	}
	/*
	for (i = 0; i < 10; i++)
	{
		for (j = 1; j < 100; j++)
		{
			printf("%d ", abs[i][j]);
		}
		printf("\n");
	}*/
	return abs[n % 10][n % 100];
}

int main(int argc, char* argv[])
{
	int a, n;
	//while (1)
	//{
	scanf("%d", &n);
	while (n--)
	{
		scanf("%d", &a);
		printf("%d\n", pow2(a));
	}
	//}
	return 0;
}


