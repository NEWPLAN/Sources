#include <stdio.h>
#include <string.h>

int main(void)
{
	int a,b,c;
	char temm[6]={0},*pp;
	scanf("%d%d",&a,&b);
	a+=b;
	sprintf(temm,"%d",a);
	c=strlen(temm);
	pp=temm;
	if(a<0)
	{
		printf("%c",*pp++);
		c--;
	}
	switch(c)
	{
		case 7:
		printf("%c,",*pp++);
		case 6:
		printf("%c",*pp++);
		case 5:
		printf("%c",*pp++);
		case 4:		
		printf("%c,",*pp++);
		case 3:
		printf("%c",*pp++);
		case 2:
		printf("%c",*pp++);
		case 1:
		printf("%c",*pp++);
		break;
	}
	return 0;
}