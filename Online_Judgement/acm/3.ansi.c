#include <stdio.h>
#include <stdlib.h>
typedef char (*mystruct)[3];
int main(void)
{
	mystruct pp;
	char p;
	int counts=0,index=0,j;
	scanf("%d",&counts);
	pp=(mystruct)malloc(counts*sizeof(char)*3);
	while(index<counts)
	{
		scanf("%s",pp[index]);
		index++;
	}
	
	for(index=0;index<counts;index++)
	{
		if(pp[index][0]>pp[index][1])
		{
			pp[index][0]=pp[index][0]+pp[index][1];
			pp[index][1]=pp[index][0]-pp[index][1];
			pp[index][0]=pp[index][0]-pp[index][1];
		}
		/*现在第一个小于第二个*/
		
		
		if(pp[index][1]>pp[index][2])
		{
			pp[index][1]=pp[index][1]+pp[index][2];
			pp[index][2]=pp[index][1]-pp[index][2];
			pp[index][1]=pp[index][1]-pp[index][2];
		}

		/*现在确保第三个最大*/
		
		if(pp[index][0]>pp[index][1])
		{
			pp[index][0]=pp[index][0]+pp[index][1];
			pp[index][1]=pp[index][0]-pp[index][1];
			pp[index][0]=pp[index][0]-pp[index][1];
		}
		/*现在第一个最小*/
	}
	
	for(index=0;index<counts;index++)
	{
		printf("%c %c %c\n",pp[index][0],pp[index][1],pp[index][2]);
	}
	
	return 0;
}