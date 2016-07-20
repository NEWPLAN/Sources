#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	/* code */
	int line=0,num=0;
	int curr=0;
	int Nbuf[100001];
	int index,from ,end,maxfrom,maxend;
	int sum=0;
	int maxval=-1001;
	scanf("%d",&line);
	while(curr<line)
	{
		curr++;
		sum=0;
		maxval=-1000;
		memset(Nbuf,0,sizeof(Nbuf));
		scanf("%d",&num);
		for(index=0,from=0,end=0,maxfrom=0,maxend=0;index<num;index++)
		{
			scanf("%d",&Nbuf[index]);
			if(maxval<0)
			{
				if(maxval<Nbuf[index])
				{
					maxval=Nbuf[index];
					maxfrom=index+1;
					maxend=index+1;
					if(Nbuf[index]>0)
					{
						sum+=Nbuf[index];
						from=index;
						end=index;
					}
				}
			}
			else
			{
				sum+=Nbuf[index];
				if(sum<0)
				{
					from=index+1;
					end=index+1;
					sum=0;
				}
				else
				{
					end=index;
				}

				if(maxval<sum)
				{
					maxval=sum;
					maxfrom=from+1;
					maxend=end+1;
				}
			}
		}
		if(curr==line)
		{
			printf("Case %d:\n%d %d %d\n",curr,maxval,maxfrom,maxend);
		}
		else
		{
			printf("Case %d:\n%d %d %d\n\n",curr,maxval,maxfrom,maxend);
		}

	}
	return 0;
}
