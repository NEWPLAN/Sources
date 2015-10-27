#include <iostream>
#include <cstdio>

using namespace std;

#define  MAX_INPUT 50000
long long x[MAX_INPUT],y[MAX_INPUT];

int main(void)
{
	int n;
	double k1,k2,k3;
	cin>>n;
	for(int i=0;i<n;i++)
		cin>>x[i]>>y[i];
	k1=((double)y[1]-y[0])/(x[1]-x[0]);
	k2=((double)y[2]-y[0])/(x[2]-x[0]);
	k3=((double)y[3]-y[0])/(x[3]-x[0]);

	if (k1==k2)
	{
		for(int j=3;j<n;j++)
		{
			k3=((double)y[j]-y[0])/(x[j]-x[0]);
			if (k1!=k3)
			{
				cout<<x[j]<<" "<<y[j];
				return 0;
			}
		}
		
	}
	else
	{
		if (k1==k3)
		{
			cout<<x[2]<< " "<<y[2];
			return 0;
		}
		else if (k2==k3)
		{
			cout<<x[1]<< " "<<y[1];
			return 0;
		}
		else
			cout<<x[0]<< " "<<y[0];
	}
	return 0;
}
