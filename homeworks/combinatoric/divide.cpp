/*************************************************************************
	> File Name: DiCo.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 24 Mar 2017 12:35:23 PM CST
 ************************************************************************/
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;
#define N 4
 
char str[10];
void Perm(char *str, int k, int m);
void Swap(char &a, char &b);
 
int main()
{
	int n;
	while(scanf("%d", &n) != EOF)
	{
		for(int i=0; i<=n; ++i)
		{
			str[i] = i+'0';
		}
        Perm(str, 1, n);
	}
	return 0;
}
 
 
void Perm(char *str, int k, int m)
{
        int i;
        if(k == m)
        {
                for(i=1; i<=m; ++i)
                        cout<<str[i]<<" "<<flush;
                cout<<endl;
                return;
        }
 
        for(i=k; i<=m; ++i)
        {
                Swap(str[k], str[i]);
                Perm(str, k+1, m);
                Swap(str[k], str[i]);
        }
 
}
 
 
void Swap(char &a, char &b)
{
        char tmp = a;
        a = b;
        b = tmp;
}
