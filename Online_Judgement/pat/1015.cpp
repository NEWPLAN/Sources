#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/**16/20*/
char p[32]={0};

void change_num(int num,int radix)
{
	int i=0;
	for(i=0;i<31&&num;i++)
	{
		p[i+1]=0;
		p[i]=num%radix;
		num=num/radix;
	}
	return ;
}
int re2num(char* str,int radix)
{
	int s=0;
	while(*str)
	{
		s*=radix;
		s+=*str++;
	}
	return s;
}

int is_prime(int num)
{
	int i=0;
	int p=ceil(sqrt((double)num));
	if(p==num)
		return 1;
	for(i=2;i<=p;i++)
	{
		if(!(num%i))
			return 0;
	}
	return 1;
}

int main(int argc,char** argv)
{
	int radix,num;
	while(scanf("%d",&num)&&num>0)
	{
		scanf("%d",&radix);
		change_num(num,radix);
		if(is_prime(re2num(p,radix)))
			printf("Yes\n");
		else
			printf("No\n");
	}
	return 0;
}