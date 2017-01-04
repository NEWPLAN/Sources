#include <stdio.h>
#include <string.h>

/*模式*/
typedef struct my_Pattern
{
	char data;
	char next_index;
}pattern;

void get_next(char* pt,int* next,int pt_size)
{
	int i=1;
	int j=0;
	next[1]=0;
	while(i<pt_size)
	{
		if(j==0 || pt[j]==pt[i])
		{
			i++;
			j++;
			next[i]=j;
		}
		else
		{
			j=next[j];
		}
	}
}

int Brute_Force(char* obj,char* Pat,int pos)
{
	
	return 0;
}

/*返回子串pat在主串obj第pos个字符串之后的位置，若不存在，则返回0*/
int KMP_index(char* obj,char* Pat,int pos,int obj_size,int Pat_size)
{
	int i=pos;
	int j=1;
	int next[255];
	
	get_next(Pat,next,Pat_size);
	
	while(i<obj_size && j<=Pat_size)
	{
		if(0==j || obj[i]==Pat[j])
		{
			i++;
			j++;
		}
		else
			j=next[j];
	}
	if(j>Pat_size)
		return i-Pat_size;
	else
		return 0;
}


int main(int argc,char** argv)
{
	int ppp;
	char p1[]=" asdasdasdfghj";
	char p2[]=" sdasdf";
	ppp=KMP_index(p1,p2,1,strlen(p1)-1,strlen(p2)-1);
	printf("%d\n",ppp);
	return 0;
}

