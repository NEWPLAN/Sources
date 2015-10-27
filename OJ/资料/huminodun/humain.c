/*马踏棋盘算法（骑士周游问题）
*N*N N>=5的时候，且N为偶数，任一点都可以有解答
*/
#include <stdio.h>
#include <time.h>

#define X 8
#define Y 8

int chess[X][Y];

void print(void);
int next_point(int *x ,int *y, int counts);
int TravelChessBoard(int x,int y,int tag);

/*获取下一个目标点,找到基于（x,y）坐标的下一个可走的位置*/
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

/*深度优先遍历棋盘
 *（x,y）为其实位置坐标
 *tag为标记变量，每走一步tag+1
 */
int TravelChessBoard(int x,int y,int tag)
{
	int x1=x,y1=y,Flag=0,count=0;
	chess[x][y]=tag;
	if(X*Y==tag)
	{
		/*打印棋盘*/
		print();
		return 1;
	}
	/*找到马的下一个可走的坐标（x1,y1）,如果找到Flag=1，否则为0*/
	Flag=next_point(&x1,&y1,count);
	while(0==Flag && count<7)/*第一次搜索*/
	{
		count++;
		Flag=next_point(&x1,&y1,count);
	}
	
	while(Flag)
	{
		if(TravelChessBoard(x1,y1,tag+1))
		{
			return 1;/*找到返回1，跟着返回1*/
		}
		/*不行的话重新查找*/
		x1=x;
		y1=y;
		count++;
		/*失败后重新搜索*/
		Flag=next_point(&x1,&y1,count);
		while(0==Flag && count<7)
		{
			count++;
			Flag=next_point(&x1,&y1,count);
		}
		/*找到马的下一个可走的坐标（x1,y1）,如果找到Flag=1，否则为0*/
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
	printf("\n本次计算耗时：%f秒",(double)(finish-start)/CLOCKS_PER_SEC);
	return 0;
}