/*copyright @newplan001@com 2016.7.21*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define MY_DEBUG
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
#ifdef 		MY_DEBUG			
			printf("matrix[%d][%d]=%d, hdx[%d]=%d\n",x,y,k,k,hdx[k]);
#endif					
			if(matrix[x][y]!=0)
				break;
			matrix[x][y]=k;			
		}
#ifdef 		MY_DEBUG
		printf("model=%d k=%d N=%d N-model=%d\n",k-matrix[x][y],k,N,N%(k-matrix[x][y]));
#endif		
		if(k>=N)
			printf("%d\n",hdx[N]);
		else
		{
			printf("%d\n",hdx[(N-matrix[x][y])%(k-matrix[x][y])+matrix[x][y]]);
		}

	}
	return 0;
}
