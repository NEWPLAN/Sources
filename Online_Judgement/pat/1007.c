#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int arrs[10000];

int main()
{
	int j,length,max,sum;
	int start1 = 0 ,start2 = 0;
	
	scanf("%d",&length);
	
	for(j = 0; j < length ; j ++)
		scanf("%d",arrs+j);

	max = INT_MIN;
	for(sum = j = 0; j < length; j ++)
	{
		sum += *(arrs+j);
		if(max < sum)
		{
			start1 = start2;
			max = sum;
		}
		if(sum < 0)
		{
			start2 = j+1;
			sum = 0;
		}
	}
	for(j = start1,sum = 0; sum != max; j ++)
		sum += *(arrs+j);
	printf("%d %d %d",sum,start1,j-1);
	 return 0;
}