#include <stdio.h>
/*
disk_count��ʾ���������ߵ�n������
pillar_1��ʾ���Ǵ�����ӵ�����
pillar_2��ʾ���ǽ���������
pillar_3��ʾ����Ŀ������
*/
void hannuota(int disk_count, char pillar_1,char pillar_2,char pillar_3)
{
	if(disk_count==1)
		printf("\n�����Ϊ%d ������ �� %c �����ƶ��� %c����",disk_count,pillar_1,pillar_3);
	else
	{
		hannuota(disk_count-1,pillar_1,pillar_3,pillar_2);
		printf("\n�����Ϊ%d ������ �� %c �����ƶ��� %c����",disk_count,pillar_1,pillar_3);
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