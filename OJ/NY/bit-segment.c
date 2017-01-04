#include <memory.h>
#include <stdio.h>

/*定义求和函数*/
#define  sums(b) b.a0+b.a1+b.a2+b.a3+b.a4+b.a5+b.a6+b.a7+\
	b.a8+b.a9+b.a10+b.a11+b.a12+b.a13+b.a14+b.a15

/*定义位段结构体，其中ai:1表示该变量只占1个bit，使用位段需要考虑的是操作系统的格式对齐问题*/
typedef struct A
{
	unsigned short a0:1; 
	unsigned short a1:1; 
	unsigned short a2:1; 
	unsigned short a3:1; 
	unsigned short a4:1;  
	unsigned short a5:1; 
	unsigned short a6:1; 
	unsigned short a7:1; 

	unsigned short a8:1; 
	unsigned short a9:1; 
	unsigned short a10:1; 
	unsigned short a11:1; 
	unsigned short a12:1; 
	unsigned short a13:1; 
	unsigned short a14:1; 
	unsigned short a15:1; 
}DATATYPE;

int main(int argc ,char** argv)
{
	DATATYPE p;
	int c;
	int n;
	scanf("%d",&n);
	while(n--)
	{
		/*注意。这里使用scanf函数读取的格式问题u表示无符号*/
		/*d表示digital，使用h表示short（短整形数）默认不写表示32位整形，l表示长整形*/
		scanf("%hd",&b);
		/*基于内存拷贝*/
		memcpy(&p,&b,sizeof(short));
		c=sums(p);
		printf("%d\n",c);
	}
	return 0;
}