#include <iostream>
using namespace std;

int arrs[10001];

int Kadane(const int array[], size_t length,  int& left,  int& right)
{
	int i, cur_left, cur_right;
	int cur_max, max;
	
	cur_max = max = left = right = cur_left = cur_right = 0;
	max=-1;
	for(i = 0; i < length; ++i)
	{
		cur_max += array[i];

		if(cur_max > 0)
		{
			cur_right = i;

			if(max < cur_max)
			{
				max = cur_max;
				left = cur_left;
				right = cur_right;
			}
		}
		else
		{
			cur_max = 0;
			cur_left = cur_right = i + 1;
		}
	}

	return max;
}
int main(void)
{
	bool negative=false;
	int n;
	int left,right,maxs=-1;
	cin>>n;
	for(int i=0;i<n;i++)
	{
		cin>>arrs[i];
		maxs=max(maxs,arrs[i]);
	}
	if(maxs==0)
	{
		/*此部分存在的问题是全都是负数，存在一个为0*/
		cout<<0<<" "<<0<<" "<<0;
		return 0;
	}
	
	int sums=Kadane(arrs,n,left,right);
	if(sums>=0)
		cout<<sums<<" "<<arrs[left]<<" "<<arrs[right];
	else
		cout<<0<<" "<<arrs[0]<<" "<<arrs[n-1];
	return 0;
}