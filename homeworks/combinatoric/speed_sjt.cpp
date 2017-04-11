#include <stdio.h>
#include <time.h>
#include <iostream>
#include <process.h>
using namespace std;

//如果define了OUT，则程序会将结果输入到output.txt中
//如果没有define OUT，则程序不会输出结果，只会将程序的
//运行时间输入到标准输入流中

#define OUT

char x[20];
int n;

inline void exchange(char *s, int i1, int i2, int *a, int *b, int &p)
{
	swap(s[i1], s[i2]);
	a[p] = i1 - 1;
	b[p] = i2 - 1;
	p++;
}

//next 函数将模拟1到n的全排列
//并将每次交换的哪两个数这个信息记录到a数组和b数组中
int next(int *a, int *b, int n)
{
	char x[20];
	bool is_to_left[20];
	int p = 0;
	exchange(x, 1, 1, a, b, p);
	for (int i = 1; i <= n; i++)
	{
		x[i] = i + '0';
		is_to_left[i] = true;
	}
	while (true)
	{
		int wh = -1;
		int l, r;
		if (is_to_left[n])
		{
			for (int i = n; i > 1; i--)
				exchange(x, i, i - 1, a, b, p);
			l = 2;
			r = n;
		}
		else
		{
			for (int i = 1; i < n; i++)
				exchange(x, i, i + 1, a, b, p);
			l = 1;
			r = n - 1;
		}
		for (int i = n - 1; i > 1; i--)
		{
			if (is_to_left[i])
			{
				if (i == x[l] - '0')l++;
				else
				{
					wh = i;
					break;
				}
			}
			else
			{
				if (i == x[r] - '0')r--;
				else
				{
					wh = i;
					break;
				}
			}
			is_to_left[i] = !is_to_left[i];
		}
		if (wh == -1) break;
		int q;
		for (int i = l; i <= r; i++)
		{
			if (x[i] - '0' == wh)
			{
				q = i;
				break;
			}
		}
		if (is_to_left[wh]) exchange(x, q, q - 1, a, b, p);
		else exchange(x, q, q + 1, a, b, p);
		is_to_left[n] = !is_to_left[n];
	}
	return p;
}

int factorial(int n)
{
	int res = 1;
	for (int i = 2; i <= n; i++)
		res *= i;
	return res;
}

int n1, n2, *a1, *b1, *a2, *b2, m1, m2;
bool choosing[20];
int number[20];

#ifdef OUT
//out函数输出当前的一个排列
void out(int ll[], int rr[])
{
	for (int i = 0; i < n1; i++)
		printf("%d ", ll[i]);
	for (int i = 0; i < n2; i++)
		printf("%d%c", rr[i], i == n2 - 1 ? '\n' : ' ');
}

inline bool less_than(int a1, int a2, int b1, int b2)
{
	return ((a1 < b1) || (a1 == b1 && a2 < b2)) ? true : false;
}

void output(int l[], int r[], int index)
{
	int x = index;
	choosing[x] = true;
	int val = 1;
	for (int i = 1; i <= 4; i++)
		val = max(val, number[i] + 1);
	number[x] = val;
	choosing[x] = false;
	for (int i = 1; i <= n; i++)
	{
		while (choosing[i] != false);
		while (number[i] != 0 && less_than(number[i], i, number[x], x));
	}
	out(l, r);
	number[x] = 0;
}
#endif


//dfs函数负责枚举所有这样的划分
//即将1到n这n个整数划分为左右两部分
//其中左边部分有n1个数字，右边有n2个数字
//n1=n/2; n2=n-n1;
void dfs_search(int lpart, int rpart, int lvaule, int index, int *lPtr, int*rPtr, int*llPtr, int*rrPtr)
{
	if (lpart < n1)
	{
		lPtr[lpart] = lvaule;
		dfs_search(lpart + 1, rpart, lvaule + 1, index, lPtr, rPtr, llPtr, rrPtr);
	}
	if (rpart < n2)
	{
		rPtr[rpart] = lvaule;
		dfs_search(lpart, rpart + 1, lvaule + 1, index, lPtr, rPtr, llPtr, rrPtr);
	}
	if (lpart == n1 && rpart == n2)
	{
		for (int i = 0; i < lpart; i++) llPtr[i] = lPtr[i];
		for (int i = 0; i < rpart; i++) rrPtr[i] = rPtr[i];
		for (int p1 = 0; p1 < m1; p1++)
		{
			swap(llPtr[a1[p1]], llPtr[b1[p1]]);
			for (int p2 = 0; p2 < m2; p2++)
			{
				swap(rrPtr[a2[p2]], rrPtr[b2[p2]]);
#ifdef OUT
				output(llPtr, rrPtr, index);
#endif
			}
		}
	}
}

int l1[20], r1[20];
int ll1[20], rr1[20];
int l2[20], r2[20];
int ll2[20], rr2[20];
int l3[20], r3[20];
int ll3[20], rr3[20];
int l4[20], r4[20];
int ll4[20], rr4[20];
bool is_end[5] = {false};

unsigned int __stdcall thread1(void* lpVoid)
{
	l1[0] = 1;
	l1[1] = 2;
	dfs_search(2, 0, 3, 1, l1, r1, ll1, rr1);
	is_end[1] = true;
}
unsigned int __stdcall thread2(void* lpVoid)
{
	l2[0] = 1;
	r2[0] = 2;
	dfs_search(1, 1, 3, 2, l2, r2, ll2, rr2);
	is_end[2] = true;
}
unsigned int __stdcall thread3(void* lpVoid)
{
	l3[0] = 2;
	r3[0] = 1;
	dfs_search(1, 1, 3, 3, l3, r3, ll3, rr3);
	is_end[3] = true;
}

int main(int argc, char** argv)
{
	scanf("%d", &n);
	long long begin_t = clock();
#ifdef OUT
	freopen("output.txt", "w", stdout);
#endif
	switch (n)
	{
	case 1:
#ifdef OUT
		printf("1\n");
#endif
		break;
	case 2:
#ifdef OUT
		printf("1 2\n2 1\n");
#endif
		break;
	case 3:
#ifdef OUT
		printf("1 2 3\n1 3 2\n3 1 2\n");
		printf("3 2 1\n2 3 1\n2 1 3\n");
#endif
	default:
		n1 = n / 2;
		n2 = n - n1;
		a1 = new int[factorial(n1) + 5];
		b1 = new int[factorial(n1) + 5];
		a2 = new int[factorial(n2) + 5];
		b2 = new int[factorial(n2) + 5];
		m1 = next(a1, b1, n1);
		m2 = next(a2, b2, n2);
		for (int i = 0; i < 5; i++) choosing[i] = 0;
		for (int i = 0; i < 5; i++) number[i] = 0;

		_beginthreadex(NULL, 0, thread1, NULL, 0, NULL);
		_beginthreadex(NULL, 0, thread2, NULL, 0, NULL);
		_beginthreadex(NULL, 0, thread3, NULL, 0, NULL);

		r4[0] = 1;
		r4[1] = 2;
		dfs_search(0, 2, 3, 4, l4, r4, ll4, rr4);

		for (int i = 1; i <= 3; i++) while (!is_end[i]);

		delete a1;
		delete b1;
		delete a2;
		delete b2;
		break;
	}
	long long end_t = clock();
	double interval = end_t - begin_t;
	interval /= 1000;

#ifndef OUT
	printf("%lf s\n", interval);
#endif

	return 0;
}
