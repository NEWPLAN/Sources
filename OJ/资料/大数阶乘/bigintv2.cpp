#include <cstdio>

void dgCal(int a[],int b,int i)
{
	if(i>a[0])
	{
		a[0]++;
	}
	int temp=a[i]+b;
	if(temp>=10)
	{
		a[i]=temp%10;
		dgCal(a,temp/10,i+1);
	}
	else
	{
		a[i]=temp;
	}

}

void calculator(int a[],int b[])
{
	int temp=0;
	int gw;
	int sw;
	for(int i=1;i<=b[0];i++)
	{
		temp=a[i]+b[i];
		if(temp>=10)
		{
			a[i]=temp%10;
			dgCal(a,temp/10,i+1);//使用递归进位
		}
		else
		{
			a[i]=temp;
		}
		
	}
}

void copyArray(int a[],int b[])
{
	for(int i=0;i<=a[0];i++)
	{
		b[i]=a[i];
	}
}

void printa(int a[])
{
	printf("位数：%d\n",a[0]);
	for(int i=a[0];i>=1;i--)
	{
		printf("%d",a[i]);
	}
	printf("\n"); 
}

void printb(int b[])
{
	printf("位数：%d\n",b[0]);
	for(int i=b[0];i>=1;i--)
	{
		printf("%d",b[i]);
	}
	printf("\n"); 
}

int main(int argc, char* argv[])
{
	int i,j;

	int a[200];
	int b[200];//辅助数组
	for(i=0;i<200;i++)
	{
		a[i]=0;
	}

	a[0]=1;
	a[1]=2;
	
	//printa(a);
	copyArray(a,b);
	//printb(b);

	for(i=3;i<=100;i++)//从3开始
	{
		copyArray(a,b);
		for(j=0;j<i-1;j++)//相加i-1次
		{
			calculator(a,b);
		}
	}
	printf("100的阶乘：\n"); 
	printa(a);
	printf("99的阶乘：\n"); 
	printb(b);

	return 0;
}

