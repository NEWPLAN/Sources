#include <iostream>
#include <cstdio>

using namespace std;

int main(void)
{
	int n,i,cc=0;
	float coefficients[10001]={0},temps;
	cin>>n;
	for(int a=0;a<n;a++)
	{
		cin>>i>>temps;
		coefficients[i]+=temps;
	}
	cin>>n;
	for(int a=0;a<n;a++)
	{
		cin>>i>>temps;
		coefficients[i]+=temps;
	}
	for(int i=10000;i>=0;i--)
		if(coefficients[i])
			cc++;
	cout<<cc;
	for(i=10000;i>=0;i--)
		if(coefficients[i])
		{
			printf(" %d %.1f",i,coefficients[i]);
		}
	return 0;
}