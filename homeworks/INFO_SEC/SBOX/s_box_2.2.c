#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

unsigned char *full_array2(int n)//����0��255�����ȫ����
{
	int i, j, l = 0, m, flag;
	static unsigned char P[256];
	m = 300;
	srand((unsigned)time(NULL));
	for (i = 0; i < m; i++)
	{
		P[l++] = (unsigned char)(rand() % (n + 1));
		for (j = 0; j < l - 1; j++)
		{
			if (P[j] == P[l - 1]) //��������������ǰ���Ѿ����ֹ�,���˳�
			{
				l--;
				break;
			}
		}
		if(l>=256)
            break;
	}
	for (i = 0; i <= n; i++)
	{
		flag = 0;
		for (j = 0; j < l; j++) //��û�г��ֵ�0~n���β�д�ں���
			if (P[j] == i)
			{
				flag = 1;
				break;
			}
		if (!flag)P[l++] = i;
	}
	return (P);
}


int count(unsigned char byte)//ͳ��byte�б���1��λ��
{
	int i, j = 0;
	for (i = 0; i < 8; i++)
		if (byte & (0x01 << i))j++;
	return j;
}


int main(int argc, char** argv)
{
	int i, j, k, b1, times;
	FILE *fp;
	char filename[20];
	unsigned char Darea[256], *DTab, data_s[256], data_p[256], data_s1[256], data_p1[256];
	unsigned char b[256][8];
	float total = 0, average = 0, variance = 0;
start1:
	printf("-----------------------------------------------------------------\n");
	printf("�������ṩ�������ݵ�����ļ�����");
	scanf("%s", filename);
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("û���ҵ����ļ�������\n");
		goto start1;
	}
	fread(Darea, 256, 1, fp);
	fclose(fp);
	printf("\n�����ȡ�� Darea��\n");
	for (i = 0; i < 256; i++)
	{
		printf("%3x", Darea[i]);
		if ((i + 1) % 16 == 0)printf("\n");
	}
	DTab = full_array2(255);
	printf("\n0��255�����ȫ���� DTab:\n");
	for (i = 0; i < 256; i++)
	{
		printf("%3x", DTab[i]);
		if ((i + 1) % 16 == 0)printf("\n");
	}
	printf("\n����s-���û�:\n");
	for (i = 0; i < 256; i++)
	{
		printf("%3x", DTab[Darea[i]]); //�������ȡ��Dareaͨ��S-�����
		if ((i + 1) % 16 == 0)printf("\n");
	}
	printf("\n����p-���û�:\n");
	for (i = 0; i < 256; i++)
	{
		printf("%3x", Darea[DTab[i]]); //�������ȡ��Dareaͨ��P-�����
		if ((i + 1) % 16 == 0)printf("\n");
	}
	printf("\n");


start2:
	printf("-----------------------------------------------------------------\n");
	printf("��������Ҫ���е���������:  ");
	scanf("%d", &times);
	getchar();
	printf("����  %d  �ֵ�����Ľ��:\n", times);
	for (i = 0; i < 256; i++)data_s[i] = data_p[i] = 0;
	for (i = 0; i < 256; i++)data_s[i] = Darea[i];
	for (j = 0; j < times; j++) //times�ֵ���
	{
		for (i = 0; i < 256; i++)data_s[i] = DTab[data_s[i]];
		for (i = 0; i < 256; i++)data_p[i] = data_s[DTab[i]];
		for (i = 0; i < 256; i++)data_s[i] = data_p[i];
	}//S-����P-�б任�ĳ˻�
	for (i = 0; i < 256; i++)
	{
		printf("%3x", data_s[i]);
		if ((i + 1) % 16 == 0)printf("\n");
	}
	for (i = 0; i < 256; i++)data_s1[i] = data_p1[i] = 0;
	for (i = 0; i < 256; i++)
	{
		b1 = Darea[i];
		for (j = 0; j < 8; j++)
		{
			data_s1[i] = Darea[i] ^ (0x01 << j); //�ı�����ĵ�iλ
			for (k = 0; k < times; k++)
			{
				data_s1[i] = DTab[data_s1[i]];
				data_p1[i] = data_s1[DTab[i]];
				data_s1[i] = data_p1[i];
			}
			b[i][j] = count(data_s1[i] ^ data_s[i]);
			Darea[i] = b1;
		}
	}
	printf("\n");
	total = average = variance = 0;
	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 8; j++)
			total += b[i][j];
	}//ͳ������ı��λ��
	average = total / (256 * 8);
	total = 0;
	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 8; j++)
			total += (b[i][j] - average) * (b[i][j] - average);
	}
	variance = total / (256 * 8);
	printf("��ֵΪ:%.3f       ����Ϊ:%.3f\n", average, variance);
	printf("\n��Ҫ������?(������y��������n):");
	if (getchar() == 'y')
		goto start2;
	return 0;
}
