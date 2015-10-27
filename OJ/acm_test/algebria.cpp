/**
God Kufeng is the God of Math. However, Kufeng is not so skilled with linear algebra, especially when dealing with matrixes.

One day, Captain Chen has a problem with matrix, here is the problem:

Given a n×n matrix A, what is the solution of n×n matrix X for the equation AX+XA=2A?

Captain Chen is a nice Captain, he wants to solve the equation only when A is a diagonal matrix, which means Aij=0 holds for all i≠j .

“That’s easy!” says Kufeng, “the answer is simply X=I, when I is the Identity Matrix.”

“But… is it the only solution for the equation above?” Captain Chen asks.

Kufeng cannot answer this question, can you help him?

Input
The first line of input is a number n, giving the size of matrix A and X. (1≤n≤1000)
Then comes a single line with n numbers, x1,x2,⋯,xn, where xi is the value of Aii. (−10000≤xi≤10000)
Output
If the answer is unique, output UNIQUE, otherwise output NOT UNIQUE
*/

#include <iostream>
using namespace std;

#define  MAX_INPUT 1000
int main(void)
{
	int n;
	int MATRIX[MAX_INPUT];
	cin>>n;
	for (int i=0;i<n;i++)
	{
		cin>>MATRIX[i];
	}

	for(int i=0;i<n;i++)
	{
		for (int j=i;j<n;j++)
		{
			if (MATRIX[i]+MATRIX[j]==0)
			{
				cout<<"NOT UNIQUE";
				return 0;
			}
		}
	}
	cout<<"UNIQUE";
	return 0;
}