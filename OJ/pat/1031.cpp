#include<iostream>
#include<string>
#include<cmath>

using namespace std;

int main(int argc, char** rgv)
{
	int N,N1/*,N2*/,N3;
	string inter;
	
	cin>>inter;
	
	N=inter.length();
	N1=(N+2)/3;
	N3=N-2*N1;
	for(int j=0;j<N1-1;j++)
	{
		cout<<inter[j];
		for(int i=0;i<N3;i++)
			cout<<" ";
		cout<<inter[N-1-j]<<endl;
	}
	
	for(int i=N1-1;i<=N-N1;i++)
		cout<<inter[i];
	return 0;
}