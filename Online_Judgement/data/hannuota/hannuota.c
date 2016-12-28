#include <stdio.h>
/*
disk_count表示盘子数或者第n个盘子
pillar_1表示的是存放盘子的柱子
pillar_2表示的是借助的柱子
pillar_3表示的是目标柱子
*/
void hannuota(int disk_count, char pillar_1,char pillar_2,char pillar_3)
{
	if(disk_count==1)
		printf("\n将编号为%d 的盘子 从 %c 柱子移动到 %c柱子",disk_count,pillar_1,pillar_3);
	else
	{
		hannuota(disk_count-1,pillar_1,pillar_3,pillar_2);
		printf("\n将编号为%d 的盘子 从 %c 柱子移动到 %c柱子",disk_count,pillar_1,pillar_3);
		hannuota(disk_count-1,pillar_2,pillar_1,pillar_3);
	}
}
int main(int argc, char** argv)
{
	int n;
	char pillar_1='A',pillar_2='B',pillar_3='C';
	scanf("%d",&n);
	hannuota(n,pillar_1,pillar_2,pillar_3);
	return 0;
}