#include <memory.h>
#include <stdio.h>

/*������ͺ���*/
#define  sums(b) b.a0+b.a1+b.a2+b.a3+b.a4+b.a5+b.a6+b.a7+\
	b.a8+b.a9+b.a10+b.a11+b.a12+b.a13+b.a14+b.a15

/*����λ�νṹ�壬����ai:1��ʾ�ñ���ֻռ1��bit��ʹ��λ����Ҫ���ǵ��ǲ���ϵͳ�ĸ�ʽ��������*/
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
		/*ע�⡣����ʹ��scanf������ȡ�ĸ�ʽ����u��ʾ�޷���*/
		/*d��ʾdigital��ʹ��h��ʾshort������������Ĭ�ϲ�д��ʾ32λ���Σ�l��ʾ������*/
		scanf("%hd",&b);
		/*�����ڴ濽��*/
		memcpy(&p,&b,sizeof(short));
		c=sums(p);
		printf("%d\n",c);
	}
	return 0;
}