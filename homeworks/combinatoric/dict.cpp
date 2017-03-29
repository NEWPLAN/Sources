/*************************************************************************
	> File Name: dict.cpp
	> Author:
	> Mail:
	> Created Time: Thu 23 Mar 2017 09:09:52 AM CST
 ************************************************************************/
#include <iostream>
using namespace std;

const int MaxNum = 9;
int iArr[MaxNum];
int count;
inline void printArr(int n)//打印数组，n为元素的总个数
{
	int i;
	for (i = 0; i < n; i++)cout << iArr[i];
	cout << endl;
	count++;
}
inline void Swap(int i, int j) //调换iArr[i]与iArr[j]的值
{
	int temp = iArr[i];
	iArr[i] = iArr[j];
	iArr[j] = temp;
}
void Transpose(int k, int m) //把数组下标为k~m的数转置
{
	int i, j;
	for (i = k, j = m; j > i; i++, j--)Swap(i, j);
}
int FullArray2(const int n)//对1~n进行全排列
{
	if (1 == n) {cout << "1" << endl; count++; return 1;} //特殊情况n=1
	int i, j;
	while (1)
	{
		printArr(n);
		for (i = n - 2; i >= 0; i--) //要求n>=2
		{
			if (iArr[i] < iArr[i + 1])break; //先求i
			if (0 == i)return 1; //函数出口：当i=0且iArr[0]>iArr[1]时，函数结束
		}
		for (j = n - 1; j > i; j--)
		{
			if (iArr[i] < iArr[j])break; //后求j
		}
		Swap(i, j); //调换iArr[i]与iArr[j]的值
		Transpose(i + 1, n - 1); //把i后面的数转置
	}
}
int  main(int argc, char** argv)
{
	int i, n;
	while (1)
	{
		//system("clear");
		count = 0; //测试新用例时,count重新置0
		cout << "请输入n（最大为9）：";
		cin >> n;
		if (n > MaxNum || n < 1) {cout << "Error: n的值在设定值范围之外" << endl; break;}
		for (i = 0; i < n; i++)iArr[i] = i + 1; //由于FullArray2上一次调用完，不会把调换的元素调整回来，故每次调用FullArray2前都要对数组进行重新初始化，即这条语句不能放在while循环外。
		FullArray2(n);
		cout << "1~" << n << "的全排列的个数：" << count << endl;
	}
    return 0;
}

