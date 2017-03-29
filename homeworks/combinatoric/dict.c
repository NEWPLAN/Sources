/*************************************************************************
	> File Name: dict.c
	> Author:
	> Mail:
	> Created Time: Tue 21 Mar 2017 04:07:05 PM CST
 ************************************************************************/

#include <stdio.h>

#include <string.h>

void swap(char *p, char *q)
{
	char m;
	m = *p;
	*p = *q;
	*q = m;
}

void sort(char s[], int n)
{
	int i, j, flag = 1;
	char ch;
	for (i = 0; i < n - 1 && flag == 1; i++)
	{
		flag = 0;
		for (j = 0; j < n - i - 1; j++)
		{
			if (s[j] > s[j + 1])
			{
				ch = s[j];
				s[j] = s[j + 1];
				s[j + 1] = ch;
				flag = 1;
			}
		}
	}

}

int main(int argc, char** argv)
{
	char s[10] = {0};
	int len, i, j;
	scanf("%s", s);
	len = strlen(s);
	sort(s, len);
	int ccc = 0;
	while (1)
	{
		printf("%d,%s\n", ++ccc, s);
		/*if (!strcmp(s, "83674521"))
			break;
		if (ccc == 37314 - 2015)
			break;
*/
		for (i = len - 2; i >= 0 && s[i] >= s[i + 1]; i--);
		if (i < 0)
			break;
		for (j = len - 1; i < j && s[j] <= s[i]; j--);
		swap(&s[i], &s[j]);
		i++;
		j = len - 1;
		while (i < j)
		{
			swap(&s[i++], &s[j--]);
		}

	}
	return 0;

}
