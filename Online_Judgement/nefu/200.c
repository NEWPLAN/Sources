/*
Problem : 200 Time Limit : 1000ms Memory Limit : 65536K 
 
description 
��ŵ������Ϸ��Ҷ�����ˣ�����ʲô�ľͲ�����˵һ���˰ɡ�
�������Ǹı���Ϸ���淨��������ֱ�Ӵ�����(��)���Ƶ�����(��)��(ÿ���ƶ�һ�����Ƶ��м�˻���м��Ƴ�)��Ҳ��������̷ŵ����̵����档
������N��Բ�̣����ٶ��ٴ��ƶ����ܰ���ЩԲ�̴�������Ƶ����ұߣ�
 
input 
�����������ݣ�ÿ������һ��Nֵ(1<=N=35)��
 
output 
����ÿ�����ݣ�����ƶ���С�Ĵ�����
 
sample_input 
1
3
12

 
sample_output 
2
26
531440


*/

#include <stdio.h>

int hansosa(int a)
{
	return a==1?2:(3*hansosa(a-1)+2);
}
int main(void)
{
	int a;
	while(scanf("%d",&a))
		printf("%d\n",hansosa(a));
	return 0;
}