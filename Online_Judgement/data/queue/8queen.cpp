#include <stdio.h>
#include <stdlib.h>

#define MAX_IN 8
/*全局变量自动初始化为0*/
char matrix[MAX_IN][MAX_IN]={0};
int counts=0;


int not_danger(int row,int j,char(*mat)[MAX_IN])
{
	int i,k;
	
	/*判断列方向是否有危险*/
	for(i=0;i<MAX_IN;i++)
		if(*(*(mat+i)+j)!=0)
			return 0;
	
	/*判断左上方是否有危险*/
	for(i=row,k=j;i>=0 && k>=0;i--,k--)
		if(*(*(mat+i)+k)!=0)
			return 0;
	
	/*判断右下方是否有危险*/
	for(i=row,k=j;i<MAX_IN && k<MAX_IN;i++,k++)
		if(*(*(mat+i)+k)!=0)
			return 0;
	
	/*判断右上方是否有危险*/
	for(i=row,k=j;i>=0&&k<MAX_IN;i--,k++)
		if(*(*(mat+i)+k)!=0)
			return 0;
	
	/*判断右上方是否有危险*/
	for(i=row,k=j;i<MAX_IN && k>=0;i++,k--)
		if(*(*(mat+i)+k)!=0)
			return 0;
	return 1;
}



/*row 表示启始行,n表示列*/

void Eight_Queue(int row,int n,char(*mat)[MAX_IN])
{
	char chess2[MAX_IN][MAX_IN];
	int i,j;
	for(i=0;i<MAX_IN;i++)
		for(j=0;j<MAX_IN;j++)
			chess2[i][j]=mat[i][j];
		//memcpy(chess2,mat,8*8);
	
	if(MAX_IN==row)
	{
		printf("第%d种：\n",counts+1);
		for(i=0;i<MAX_IN;i++)
		{
			for(j=0;j<MAX_IN;j++)
				printf("%c ",*(*(chess2+i)+j));
			printf("\n");
		}
		printf("\n");
		counts++;
	}
	else
	{
		/*判断这个位置是否有危险，如果有危险，如过没有危险继续往下*/
		for(j=0;j<n;j++)
		{
			if(not_danger(row,j,mat))/*判断是否危险*/
			{
				for(i=0;i<MAX_IN;i++)
				{
					*(*(chess2+row)+i)=0;
				}
				*(*(chess2+row)+j)=1;
				Eight_Queue(row+1,n,chess2);
			}
		}
	}
}

int main(int argc,char**argv)
{
	Eight_Queue(0,8,matrix);
	printf("\n共有解决方法：%d\n",counts);
	return 0;
}