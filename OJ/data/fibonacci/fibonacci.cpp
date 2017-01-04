/*poj3070*/

#include <iostream>

#define maxSize 5000000
using namespace std;
int F[maxSize];
int fibonacci(int n,int mod)
{
	if(n==0)
		return 0;
	if(n==1)
		return 1;
	if(n==2)
		return 1;
	if(n<maxSize&&F[n])
		return F[n];
	int temp;
	/*
	*使用fibonacci的卷积公式：
	*F[N]=F[N/2]*F[N/2+1] 	+ F[N/2-1]*F[N/2];当N为偶数的时候
	*F[N]=F[N/2+1]*F[N/2+1] + F[N/2]*F[N/2];当N为奇数的时候
	*/
	if(n%2==0)/*odd*/
		temp=(fibonacci(n/2,mod)*fibonacci(n/2+1,mod)+fibonacci(n/2-1,mod)*fibonacci(n/2,mod))%mod;
	else
		temp=(fibonacci(n/2+1,mod)*fibonacci(n/2+1,mod)+fibonacci(n/2,mod)*fibonacci(n/2,mod))%mod;	
	if(n<maxSize)
		F[n]=temp;
	return temp;
}

int main(int argc , char** argv)
{
	int n;
	F[1]=1;
	F[2]=1;
	F[3]=2;

	while(cin>>n&&n!=-1)
	{
		cout<<fibonacci(n,10000)<<endl;
	}
	return 1;
}