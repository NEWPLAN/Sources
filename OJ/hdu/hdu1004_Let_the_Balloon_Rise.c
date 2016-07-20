#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Vals
{
	int counts;
	char str[17];
};

int main(int argc, char const *argv[])
{
	/* code */
	char from[17];
	int Nlen=0;
	int before=0;
	int index=0,allnums=0;
	struct Vals matrix[1024];
	while(scanf("%d",&Nlen)&&Nlen)
	{
		memset(matrix,0,sizeof(matrix));
		while(Nlen--)
		{
			memset(from,0,sizeof(from));
			scanf("%s",from);
			for(index=0;index<allnums;index++)
			{
				if(strcmp(matrix[index].str,from)==0)
				{
					matrix[index].counts++;
					break;
				}
			}
			if(index==allnums)
			{
				strcpy(matrix[index].str,from);
				allnums++;
				matrix[index].counts++;
			}	
			if(matrix[index].counts>matrix[before].counts)
				before=index;

		}
		Nlen=0;
		printf("%s\n",matrix[before].str);
	}
	return 0;
}
