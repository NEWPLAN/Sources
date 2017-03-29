/*************************************************************************
	> File Name: sjt.c
	> Author:
	> Mail:
	> Created Time: Tue 21 Mar 2017 04:09:02 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef enum DIR {LEFT = -1, RIGHT = 1}DIR;


/*  函数功能：判断下标i所指p中元素是否处于活动状态
 *  输入参数：p  in,指向n个字符的一个当前排列
 *          dir in,标记p中每个元素的箭头方向
 *          i   in,待判定元素的下标
 *          N   in,待排列字符的个数
 *  返回值：    true 表示待判定元素为活动状态，
 *          false 表示待判定元素处于非活动状态
 */
int IsActive(const char *p, const DIR *dir, const int i, const int N)
{
	if (i + dir[i] < 0 || i + dir[i] >= N)
		return 0;
	if (p[i + dir[i]] < p[i]) //箭头所指一侧相邻的数比该数小
		return 1;
	else return 0;
}


/*  函数功能：找到p中处于活动状态的数中的最大者
 *  输入参数：p  in,指向n个字符的一个当前排列
 *          dir in,标记p中每个元素的箭头方向
 *          N   in,待排列字符的个数
 *  返回值：上述最大者的下标，-1表示调用参数有误,N表示没有活动者
 */
int MaxActive(const char *p, const DIR *dir, const int N)
{
	int k = N;
	for (int i = 0; i < N; i++)
		if (IsActive(p, dir, i, N) && (p[i] > p[k]))
			k = i;
	return k;
}


/*  函数功能：交换下标i所指元素与其箭头方向所指元素,原位交换
 *  输入参数：p  inout,指向n个字符的一个当前排列
 *          dir inout,标记p中每个元素的箭头方向
 *          i   in,待交换元素的下标
 *  返回值：    true 表示交换成功
 *          false 表示交换失败，失败原因为调用参数有误。
 */
int Swap(char *p, DIR *dir, int *i)
{
	if (p == NULL || dir == NULL)
		return 0;


	//交换相邻的元素；
	char temp = p[*i];
	p[*i] = p[*i + dir[*i]];
	p[*i + dir[*i]] = temp;


	//元素相关的箭头也得交换
	DIR T = dir[*i];
	dir[*i] = dir[*i + T];
	dir[*i + T] = T;


	*i = *i + T;    //使*i依旧是未交换前*i所指元素的下标
	return 1;
}


/*  函数功能：上述算法思路第三步，修改所以比k大的元素的箭头方向，原位修改
 *  输入参数：p  in,指向n个字符的一个当前排列
 *          dir inout,标记p中每个元素的箭头方向
 *          k   in,p中处于活动状态的最大者的下标，由MaxActive函数求出
 *          N   in,缓冲区p的长度,也是待排列字符的个数
 *  返回值：    true 表示函数执行成功
 *          false 表示函数执行失败，失败原因为调用参数有误
 */
int ModifyDir(const char *p, DIR *dir, const int k, const int N)
{
	int i;
	if (p == NULL || dir == NULL)
		return 0;
	for (i = 0; i < N; i++)
		if (p[i] > p[k])
			dir[i] = (dir[i] == LEFT ? RIGHT : LEFT);
	return 1;
}
int main(int argc, char** argv)
{
	int N = 0, i = 0;
printf("enter the number of elements: ");
	scanf("%d", &N);

	char *p = (char*)malloc(sizeof(char) * (N + 1));
	DIR *dir = (DIR*)malloc(sizeof(DIR) * (N + 1));
	for (i = 0; i < N; i++)
	{
		p[i] = '1' + i;
		dir[i] = LEFT;
	}
	p[N] = '\0';
	int k = 0;
//	printf("%s", p);
	int c = 1;
	do
	{
		k = MaxActive(p, dir, N);
		if (k == N)
			break;
		Swap(p, dir, &k);
		ModifyDir(p, dir, k, N);
//		printf("->%s", p);
		c++;
/*
		if (!strcmp(p, "83674521"))
		{
			printf("%d",c);
			break;
		}
		if (c == 20570)
			break;
		/*
		if (c == 2016)
			break;*/
	}	while (1);
	printf("\n%d\n", c);
	free(p);
	free(dir);
	return 0;
}
