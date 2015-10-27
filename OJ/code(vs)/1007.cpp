#include <iostream>

using namespace std;

int main(void)
{
	int n=1,k=0;
	double sums=1.0;
	cin>>k;
	while(1)
	{
		if(sums>k)
		{
			cout<<n;
			return 0;
		}
		n++;
		sums+=((double)1/n);
	}
	return 0;
}