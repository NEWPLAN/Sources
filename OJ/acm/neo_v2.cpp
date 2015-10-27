#include <iostream>
#include <algorithm>

#define MAX 101
using namespace std;

int D[MAX][MAX];
int n;
int maxSum[MAX][MAX];

int MaxSum(int i, int j)
{
	if(maxSum[i][j]!=-1)
		return maxSum[i][j];
	
	if(i==n)
		maxSum[i][j]= D[i][j];
	else
		maxSum[i][j]=  max(MaxSum(i+1,j),MaxSum(i+1,j+1))+D[i][j];
	return maxSum[i][j];
}

int main(void)
{
	int j ,i;
	cin>>n;
	for(i=1;i<=n;i++)
		for(j=1;j<=i;j++)
		{
			cin>>D[i][j];
			maxSum[i][j]=-1;
		}
		
	cout<<MaxSum(1,1)<<endl;
	return 0;
}