#include <iostream>

using namespace std;

int main(void)
{
	int a,b,timers,lasted;
	cin>>a>>b;
	int maxs , mins;
	maxs=a>b?a:b;
	mins=a<b?a:b;
	if(mins==maxs)
	{
		cout<<"No";
		return 0;
	}
	if(mins<0)
	{
		cout<<"No";
		return 0;
	}
	if (mins>=5&&(maxs-mins>1))
	{
		cout<<"No";
		return 0;
	}
	if (mins<5&&maxs>5&&(maxs>mins+1))
	{
		cout<<"No";
		return 0;
	}

	if(maxs<=5)
	{
		lasted=5-maxs;
		int tt=maxs-mins;
		if(lasted<tt-1)
		{
			cout<<"No";
			return 0;
		}
	}
	
	cout<<"Yes";
	return 0;
};