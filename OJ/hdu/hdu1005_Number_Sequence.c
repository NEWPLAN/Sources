#include <stdio.h>

int main(int argc, char const *argv[])
{
	
	int windows[3]={1,1,1};
	int A,B,n;
	int index=2;
	while(scanf("%d %d %d",&A,&B,&n)&&A!=0&&B!=0&&n!=0)
	{
		windows[0]=windows[1]=windows[2]=1;
		for(index=2;index<=n-1;index++)
		{
			windows[0]=windows[1];
			windows[1]=windows[2];
			windows[2]=(windows[0]*B+windows[1]*A)%7;
			if(windows[2]==windows[1] && windows[1]==windows[0])
				break;

		}
		printf("%d\n",windows[2]);
	}
	return 0;
}
