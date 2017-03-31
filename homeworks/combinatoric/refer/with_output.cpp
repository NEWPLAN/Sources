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

inline void swap(char *s, int i1, int i2, int *a, int *b, int &p)
{
	swap(s[i1], s[i2]);
	a[p] = i1 - 1;
	b[p] = i2 - 1;
	p++;
}

//neighbor 函数将模拟1到n的全排列
//并将每次交换的哪两个数这个信息记录到a数组和b数组中
int neighbor(int *a, int *b, int n)
{
	char x[20];
	bool is_to_left[20];
	int p = 0;
	swap(x, 1, 1, a, b, p);
	for (int i = 1; i <= n; i++)
	{
		x[i] = i + '0';
		is_to_left[i] = true;
	}
	while (true)
	{
		if (is_to_left[n])
		{
			for (int i = n; i > 1; i--)
			{
				swap(x, i, i - 1, a, b, p);
			}
		}
		else
		{
			for (int i = 1; i < n; i++)
			{
				swap(x, i, i + 1, a, b, p);
			}
		}
		int wh = -1;
		int l, r;
		if (is_to_left[n])
		{
			l = 2;
			r = n;
		}
		else
		{
			l = 1;
			r = n - 1;
		}
		for (int i = n - 1; i > 1; i--)
		{
			if (is_to_left[i])
			{
				if (i == x[l] - '0')
				{
					l++;
				}
				else
				{
					wh = i;
					break;
				}
			}
			else
			{
				if (i == x[r] - '0')
				{
					r--;
				}
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
		if (is_to_left[wh]) swap(x, q, q - 1, a, b, p);
		else swap(x, q, q + 1, a, b, p);
		is_to_left[n] = !is_to_left[n];
	}
	return p;
}

int fac(int n)
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

bool less_than(int a1, int a2, int b1, int b2)
{
	if (a1 < b1) return true;
	if (a1 == b1 && a2 < b2) return true;
	return false;
}
#endif
//dfs函数负责枚举所有这样的划分
//即将1到n这n个整数划分为左右两部分
//其中左边部分有n1个数字，右边有n2个数字
//n1=n/2; n2=n-n1;

int l1[20], r1[20];
int ll1[20], rr1[20];
#ifdef OUT
void out1(int l[], int r[])
{
	int x = 1;
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
void dfs1(int lp, int rp, int lv)
{
	if (lp < n1)
	{
		l1[lp] = lv;
		dfs1(lp + 1, rp, lv + 1);
	}
	if (rp < n2)
	{
		r1[rp] = lv;
		dfs1(lp, rp + 1, lv + 1);
	}
	if (lp == n1 && rp == n2)
	{
		for (int i = 0; i < lp; i++) ll1[i] = l1[i];
		for (int i = 0; i < rp; i++) rr1[i] = r1[i];
		for (int p1 = 0; p1 < m1; p1++)
		{
			swap(ll1[a1[p1]], ll1[b1[p1]]);
			for (int p2 = 0; p2 < m2; p2++)
			{
				swap(rr1[a2[p2]], rr1[b2[p2]]);
#ifdef OUT
				out1(ll1, rr1);
#endif
			}
		}
	}
}

int l2[20], r2[20];
int ll2[20], rr2[20];
#ifdef OUT
void out2(int l[], int r[])
{
	int x = 2;
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
void dfs2(int lp, int rp, int lv)
{
	if (lp < n1)
	{
		l2[lp] = lv;
		dfs2(lp + 1, rp, lv + 1);
	}
	if (rp < n2)
	{
		r2[rp] = lv;
		dfs2(lp, rp + 1, lv + 1);
	}
	if (lp == n1 && rp == n2)
	{
		for (int i = 0; i < lp; i++) ll2[i] = l2[i];
		for (int i = 0; i < rp; i++) rr2[i] = r2[i];
		for (int p1 = 0; p1 < m1; p1++)
		{
			swap(ll2[a1[p1]], ll2[b1[p1]]);
			for (int p2 = 0; p2 < m2; p2++)
			{
				swap(rr2[a2[p2]], rr2[b2[p2]]);
#ifdef OUT
				out2(ll2, rr2);
#endif
			}
		}
	}
}

int l3[20], r3[20];
int ll3[20], rr3[20];
#ifdef OUT
void out3(int l[], int r[])
{
	int x = 3;
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
void dfs3(int lp, int rp, int lv)
{
	if (lp < n1)
	{
		l3[lp] = lv;
		dfs3(lp + 1, rp, lv + 1);
	}
	if (rp < n2)
	{
		r3[rp] = lv;
		dfs3(lp, rp + 1, lv + 1);
	}
	if (lp == n1 && rp == n2)
	{
		for (int i = 0; i < lp; i++) ll3[i] = l3[i];
		for (int i = 0; i < rp; i++) rr3[i] = r3[i];
		for (int p1 = 0; p1 < m1; p1++)
		{
			swap(ll3[a1[p1]], ll3[b1[p1]]);
			for (int p2 = 0; p2 < m2; p2++)
			{
				swap(rr3[a2[p2]], rr3[b2[p2]]);
#ifdef OUT
				out3(ll3, rr3);
#endif
			}
		}
	}
}

int l4[20], r4[20];
int ll4[20], rr4[20];

#ifdef OUT
void out4(int l[], int r[])
{
	int x = 4;
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

void dfs4(int lp, int rp, int lv)
{
	if (lp < n1)
	{
		l4[lp] = lv;
		dfs4(lp + 1, rp, lv + 1);
	}
	if (rp < n2)
	{
		r4[rp] = lv;
		dfs4(lp, rp + 1, lv + 1);
	}
	if (lp == n1 && rp == n2)
	{
		for (int i = 0; i < lp; i++) ll4[i] = l4[i];
		for (int i = 0; i < rp; i++) rr4[i] = r4[i];
		for (int p1 = 0; p1 < m1; p1++)
		{
			swap(ll4[a1[p1]], ll4[b1[p1]]);
			for (int p2 = 0; p2 < m2; p2++)
			{
				swap(rr4[a2[p2]], rr4[b2[p2]]);
#ifdef OUT
				out4(ll4, rr4);
#endif
			}
		}
	}
}

bool is_end[5] = {false};

unsigned int __stdcall thread1(void* lpVoid)
{
	dfs1(2, 0, 3);
	is_end[1] = true;
}
unsigned int __stdcall thread2(void* lpVoid)
{
	dfs2(1, 1, 3);
	is_end[2] = true;
}
unsigned int __stdcall thread3(void* lpVoid)
{
	dfs3(1, 1, 3);
	is_end[3] = true;
}

int main(int argc, char** argv)
{
	scanf("%d", &n);
	long long begin_t = clock();

#ifdef OUT
	freopen("output.txt", "w", stdout);
#endif
	if (n == 1)
	{
#ifdef OUT
		printf("1\n");
#endif
	}
	else if (n == 2)
	{
#ifdef OUT
		printf("1 2\n2 1\n");
#endif
	}
	else if (n < 4)
	{
		n1 = n / 2;
		n2 = n - n1;
		a1 = new int[fac(n1) + 5];
		b1 = new int[fac(n1) + 5];
		a2 = new int[fac(n2) + 5];
		b2 = new int[fac(n2) + 5];
		m1 = neighbor(a1, b1, n1);
		m2 = neighbor(a2, b2, n2);
		dfs1(0, 0, 1);

		delete a1;
		delete b1;
		delete a2;
		delete b2;
	}
	else
	{
		n1 = n / 2;
		n2 = n - n1;
		a1 = new int[fac(n1) + 5];
		b1 = new int[fac(n1) + 5];
		a2 = new int[fac(n2) + 5];
		b2 = new int[fac(n2) + 5];
		m1 = neighbor(a1, b1, n1);
		m2 = neighbor(a2, b2, n2);
		for (int i = 0; i < 5; i++) choosing[i] = 0;
		for (int i = 0; i < 5; i++) number[i] = 0;

		l1[0] = 1; l1[1] = 2;
		_beginthreadex(NULL, 0, thread1, NULL, 0, NULL);

		l2[0] = 1; r2[0] = 2;
		_beginthreadex(NULL, 0, thread2, NULL, 0, NULL);

		l3[0] = 2; r3[0] = 1;
		_beginthreadex(NULL, 0, thread3, NULL, 0, NULL);

		r4[0] = 1; r4[1] = 2;
		dfs4(0, 2, 3);

		for (int i = 1; i <= 3; i++) while (!is_end[i]);

		delete a1;
		delete b1;
		delete a2;
		delete b2;

	}

	long long end_t = clock();
	double interval = end_t - begin_t;
	interval /= 1000;

#ifndef OUT
	printf("%lf s\n", interval);
#endif

	return 0;
}
