/*��̤�����㷨����ʿ�������⣩
*N*N N>=5��ʱ����NΪż������һ�㶼�����н��
*/
#include <stdio.h>
#include <time.h>

#define X 8
#define Y 8

int chess[X][Y];

void print(void);
int next_point(int *x ,int *y, int counts);
int TravelChessBoard(int x,int y,int tag);

/*��ȡ��һ��Ŀ���,�ҵ����ڣ�x,y���������һ�����ߵ�λ��*/
/*
 *
 0-------------------------------------Y
 |**************************************
 |**************************************
 |**************************************
 |**************************************
 |****************1*2*******************
 |***************8***3******************
 |*****************P********************
 |***************7***4******************
 |****************6*5*******************
 |**************************************
 |**************************************
 |**************************************
 |**************************************
 X**************************************
 */
int next_point(int *x ,int *y, int counts)
{
	switch(counts)
	{
		case 0:
		if(*x-1>=0 && *y-2>=0 && chess[*x-1][*y-2]==0)
		{
			*x-=1;
			*y-=2;
			return 1;
		}
		break;
		
		case 7:
		if(*x+1<X && *y-2>=0 && chess[*x+1][*y-2]==0)
		{
			*x+=1;
			*y-=2;
			return 1;
		}
		break;
		
		case 6:
		if(*x+2<X && *y-1>=0 && chess[*x+2][*y-1]==0)
		{
			*x+=2;
			*y-=1;
			return 1;
		}
		break;
		
		case 5:
		if(*x+2<X && *y+1< Y && chess[*x+2][*y+1]==0)
		{
			*x+=2;
			*y+=1;
			return 1;
		}
		break;
		
		case 4:
		if(*x+1<X && *y+2<Y && chess[*x+1][*y+2]==0)
		{
			*x+=1;
			*y+=2;
			return 1;
		}
		break;
		
		case 3:
		if(*x-1>=0 && *y+2<Y && chess[*x-1][*y+2]==0)
		{
			*x-=1;
			*y+=2;
			return 1;
		}
		break;
		
		case 2:
		if(*x-2>=0 && *y+1<Y && chess[*x-2][*y+1]==0)
		{
			*x-=2;
			*y+=1;
			return 1;
		}
		break;
		
		case 1:
		if(*x-2>=0 && *y-1>=0 && chess[*x-2][*y-1]==0)
		{
			*x-=2;
			*y-=1;
			return 1;
		}
		break;
		
		default :
		break;
	}
	return 0;
}

/*������ȱ�������
 *��x,y��Ϊ��ʵλ������
 *tagΪ��Ǳ�����ÿ��һ��tag+1
 */
int TravelChessBoard(int x,int y,int tag)
{
	int x1=x,y1=y,Flag=0,count=0;
	chess[x][y]=tag;
	if(X*Y==tag)
	{
		/*��ӡ����*/
		print();
		return 1;
	}
	/*�ҵ������һ�����ߵ����꣨x1,y1��,����ҵ�Flag=1������Ϊ0*/
	Flag=next_point(&x1,&y1,count);
	while(0==Flag && count<7)/*��һ������*/
	{
		count++;
		Flag=next_point(&x1,&y1,count);
	}
	
	while(Flag)
	{
		if(TravelChessBoard(x1,y1,tag+1))
		{
			return 1;/*�ҵ�����1�����ŷ���1*/
		}
		/*���еĻ����²���*/
		x1=x;
		y1=y;
		count++;
		/*ʧ�ܺ���������*/
		Flag=next_point(&x1,&y1,count);
		while(0==Flag && count<7)
		{
			count++;
			Flag=next_point(&x1,&y1,count);
		}
		/*�ҵ������һ�����ߵ����꣨x1,y1��,����ҵ�Flag=1������Ϊ0*/
	}
	if(Flag==0)
	{
		chess[x][y]=0;
	}
	return 0;
}

void print(void)
{
	int i,j;
	for(i=0;i<X;i++)
	{
		for(j=0;j<Y;j++)
			printf("%2d\t",chess[i][j]);
		printf("\n");
	}
	printf("\n");
}

int main(int argc,char** argv)
{
	clock_t start,finish;
	start=clock();
	if(!TravelChessBoard(2,0,1))
	{
		printf("sorry for failed search\n");
	}
	finish=clock();
	printf("\n���μ����ʱ��%f��",(double)(finish-start)/CLOCKS_PER_SEC);
	return 0;
}