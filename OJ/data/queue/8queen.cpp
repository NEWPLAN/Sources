#include <stdio.h>
#include <stdlib.h>

#define MAX_IN 8
/*ȫ�ֱ����Զ���ʼ��Ϊ0*/
char matrix[MAX_IN][MAX_IN]={0};
int counts=0;


int not_danger(int row,int j,char(*mat)[MAX_IN])
{
	int i,k;
	
	/*�ж��з����Ƿ���Σ��*/
	for(i=0;i<MAX_IN;i++)
		if(*(*(mat+i)+j)!=0)
			return 0;
	
	/*�ж����Ϸ��Ƿ���Σ��*/
	for(i=row,k=j;i>=0 && k>=0;i--,k--)
		if(*(*(mat+i)+k)!=0)
			return 0;
	
	/*�ж����·��Ƿ���Σ��*/
	for(i=row,k=j;i<MAX_IN && k<MAX_IN;i++,k++)
		if(*(*(mat+i)+k)!=0)
			return 0;
	
	/*�ж����Ϸ��Ƿ���Σ��*/
	for(i=row,k=j;i>=0&&k<MAX_IN;i--,k++)
		if(*(*(mat+i)+k)!=0)
			return 0;
	
	/*�ж����Ϸ��Ƿ���Σ��*/
	for(i=row,k=j;i<MAX_IN && k>=0;i++,k--)
		if(*(*(mat+i)+k)!=0)
			return 0;
	return 1;
}



/*row ��ʾ��ʼ��,n��ʾ��*/

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
		printf("��%d�֣�\n",counts+1);
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
		/*�ж����λ���Ƿ���Σ�գ������Σ�գ����û��Σ�ռ�������*/
		for(j=0;j<n;j++)
		{
			if(not_danger(row,j,mat))/*�ж��Ƿ�Σ��*/
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
	printf("\n���н��������%d\n",counts);
	return 0;
}