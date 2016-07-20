#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int A,B,N;
	int matrix[8][8];
	int hdx[100];
	while(scanf("%d %d %d",&A,&B,&N)!=EOF && A+B+N)
	{
		memset(matrix,0,sizeof(matrix));
		memset(hdx,0,sizeof(hdx));
		int k=3,x,y;
		hdx[1]=hdx[2]=1;
		for(k=3;k<99;k++)
		{
			x=hdx[k-1];
			y=hdx[k-2];
			hdx[k]=(A*hdx[k-1]+B*hdx[k-2]) %7;
			printf("matrix[%d][%d]=%d, hdx[%d]=%d\n",x,y,k,k,hdx[k]);
			if(matrix[x][y]!=0)
				break;
			matrix[x][y]=k;			
		}
		printf("model=%d k=%d N=%d N-model=%d\n",k-3,k,N,N%(k-3));
		if(k>=N)
			printf("%d\n",hdx[N]);
		else
		{
			printf("%d\n",hdx[(N-3)%(k-3)+3]);
		}

	}
	return 0;
}