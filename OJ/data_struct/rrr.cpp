#include<iostream>
#include <vector>
#include <string>

using namespace std;

int N;
string outs;

vector<char> strs;

void back_trace(int n);


int main(void)
{
	char p;
	cin>>N;
	int currs=N;
	for(int i=0;i<N;i++)
	{
		cin>>p;
		strs.push_back(p);
	}
	
	back_trace(0);
	return 0;
}

void back_trace(int n)
{
	if(n>=N)
		cout<<outs<<endl;
	else
	{
		int tt=strs.size();
		for(int j=0;j<tt;j++)
		{
			outs.push_back(strs.back());
			strs.pop_back();
			back_trace(n+1);
			strs.push_back(outs[n+j]);
			outs.pop_back();
		}
	}
}