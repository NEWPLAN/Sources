#include <iostream>
#include <algorithm>

#define MAX 101
using namespace std;

int D[MAX][MAX];
int n;

int MaxSum(int i, int j)
{
	if(i==n)
		return D[i][j];
	else
		return max(MaxSum(i+1,j),MaxSum(i+1,j+1))+D[i][j];
}

int main(void)
{
	int j ,i;
	cin>>n;
	for(i=1;i<=n;i++)
		for(j=1;j<=i;j++)
			cin>>D[i][j];
		
	cout<<MaxSum(1,1)<<endl;
	return 0;
}