/*
Problem : 200 Time Limit : 1000ms Memory Limit : 65536K 
 
description 
汉诺塔的游戏大家都玩过了，规则什么的就不用再说一遍了吧。
现在我们改变游戏的玩法，不允许直接从最左(右)边移到最右(左)边(每次移动一定是移到中间杆或从中间移出)，也不允许大盘放到下盘的上面。
现在有N个圆盘，至少多少次移动才能把这些圆盘从最左边移到最右边？
 
input 
包含多组数据，每次输入一个N值(1<=N=35)。
 
output 
对于每组数据，输出移动最小的次数。
 
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