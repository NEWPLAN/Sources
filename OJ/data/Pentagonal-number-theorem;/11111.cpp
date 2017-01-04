/*----------------------------------------------
 *        Author    :NEWPLAN
 *        Date    :2015-04-01
 *        Email    :xxxxxxx
 *        Copyright:NEWPLAN
-----------------------------------------------*/
#include <iostream>


#define MAXNUM 100            //最高次数
unsigned  long ww[MAXNUM*11][MAXNUM*11];
unsigned long dynamic_GetPartitionCount(int n, int max);

using namespace std;
int main(int argc, char **argv)
{
    int n;
    int m;
    unsigned long count;
	
	while(1)
	{
		cin>>n;
		cout<<dynamic_GetPartitionCount(n,n)<<endl;
	}
	
    return 0;
}

unsigned long dynamic_GetPartitionCount(int n, int max)
{
    if(n == 1 || max == 1)
    {
		ww[n][max]=1;
        return 1;
    }
    if(n < max)
    {
		ww[n][n]=ww[n][n]? ww[n][n] : dynamic_GetPartitionCount(n, n);
		return ww[n][n];
    }
    if(n == max)
    {
		ww[n][max]=ww[n][n-1]?1+ww[n][n-1]:1 + dynamic_GetPartitionCount(n, n - 1);
		return ww[n][max];
    }
    else
    {
		ww[n][max]=ww[n - max][max]? (ww[n - max][max]) : dynamic_GetPartitionCount(n - max, max);
		ww[n][max]+=ww[n][max-1]? (ww[n][max-1]): dynamic_GetPartitionCount(n, max - 1);	
        return ww[n][max];
    }
}