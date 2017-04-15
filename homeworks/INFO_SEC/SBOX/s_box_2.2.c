#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

unsigned char *full_array2(int n)//生成0—255的随机全排列
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
			if (P[j] == P[l - 1]) //产生的随机数如果前面已经出现过,则退出
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
		for (j = 0; j < l; j++) //把没有出现的0~n依次补写在后面
			if (P[j] == i)
			{
				flag = 1;
				break;
			}
		if (!flag)P[l++] = i;
	}
	return (P);
}


int count(unsigned char byte)//统计byte中比特1的位数
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
	printf("请输入提供采样数据的随机文件名：");
	scanf("%s", filename);
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		printf("没有找到该文件！！！\n");
		goto start1;
	}
	fread(Darea, 256, 1, fp);
	fclose(fp);
	printf("\n随机获取的 Darea：\n");
	for (i = 0; i < 256; i++)
	{
		printf("%3x", Darea[i]);
		if ((i + 1) % 16 == 0)printf("\n");
	}
	DTab = full_array2(255);
	printf("\n0—255的随机全排列 DTab:\n");
	for (i = 0; i < 256; i++)
	{
		printf("%3x", DTab[i]);
		if ((i + 1) % 16 == 0)printf("\n");
	}
	printf("\n进行s-盒置换:\n");
	for (i = 0; i < 256; i++)
	{
		printf("%3x", DTab[Darea[i]]); //对随机获取的Darea通过S-盒输出
		if ((i + 1) % 16 == 0)printf("\n");
	}
	printf("\n进行p-盒置换:\n");
	for (i = 0; i < 256; i++)
	{
		printf("%3x", Darea[DTab[i]]); //对随机获取的Darea通过P-盒输出
		if ((i + 1) % 16 == 0)printf("\n");
	}
	printf("\n");


start2:
	printf("-----------------------------------------------------------------\n");
	printf("请输入需要进行迭代的轮数:  ");
	scanf("%d", &times);
	getchar();
	printf("经过  %d  轮迭代后的结果:\n", times);
	for (i = 0; i < 256; i++)data_s[i] = data_p[i] = 0;
	for (i = 0; i < 256; i++)data_s[i] = Darea[i];
	for (j = 0; j < times; j++) //times轮迭代
	{
		for (i = 0; i < 256; i++)data_s[i] = DTab[data_s[i]];
		for (i = 0; i < 256; i++)data_p[i] = data_s[DTab[i]];
		for (i = 0; i < 256; i++)data_s[i] = data_p[i];
	}//S-盒与P-盒变换的乘积
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
			data_s1[i] = Darea[i] ^ (0x01 << j); //改变输入的第i位
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
	}//统计输出改变的位数
	average = total / (256 * 8);
	total = 0;
	for (i = 0; i < 256; i++)
	{
		for (j = 0; j < 8; j++)
			total += (b[i][j] - average) * (b[i][j] - average);
	}
	variance = total / (256 * 8);
	printf("均值为:%.3f       方差为:%.3f\n", average, variance);
	printf("\n还要继续吗?(是输入y，否输入n):");
	if (getchar() == 'y')
		goto start2;
	return 0;
}
