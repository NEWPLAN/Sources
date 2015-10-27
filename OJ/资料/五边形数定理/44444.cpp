/*----------------------------------------------
 *        Author    :梦醒潇湘love
 *        Date    :2013-02-28
 *        Email    :9974**140@qq.com
 *        Copyright:anyone
-----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 
//递归法求解整数划分
unsigned long GetPartitionCount(int n, int max)
{
    if(n == 1 || max == 1)
    {
        return 1;
    }
    if(n < max)
    {
        return GetPartitionCount(n, n);
    }
    if(n == max)
    {
        return 1 + GetPartitionCount(n, n - 1);
    }
    else
    {
        return GetPartitionCount(n - max, max) + GetPartitionCount(n, max - 1);
    }
}


//母函数法求整数划分

#define MAXNUM 100            //最高次数
unsigned long a[MAXNUM];
unsigned long b[MAXNUM];
unsigned long c[MAXNUM];    //保存结果

//两个多项式进行乘法，系数分别保存在a和b中，结果保存到c，项的最大次数到MAXNUM
void Poly()
{
    int i;
    int j;
    memset(c, 0, sizeof(c));
    for(i = 0; i < MAXNUM; i++)
    {
        for(j = 0; j < MAXNUM - i; j++)    //j < MAXNUM - i,确保i+j不越界
        {
            c[i + j] += a[i] * b[j];
        }
    }
}
//计算前N项的系数，即g(x,1)*g(x,2)*....*g(x,n)的展开结果
void Init(int m)
{
    int i;
    int j;
    memset(a, 0, sizeof(a));
    memset(c, 0, sizeof(c));
    //第一个多项式:g(x) = x^0 + x^1 + x^2 + ... + x^n
    for(i = 0; i < MAXNUM; i++)
    {
        a[i] = 1;
    }
    //for(j = 2; j <= MAXNUM; j++)//只能求f(n,n)
    //通过修改这里，使得可以求f(n,m)，对于任意的正整数n,m都适合
    for(j = 2; j <= m; j++)
    {
        memset(b, 0, sizeof(b));
        //第i个多项式:g(x) = x^0 + x^i + x^(2k) + ...
        for(i = 0; i <= MAXNUM; i += j)
        {
            b[i] = 1;
        }
        //多项式相乘:c = a * b
        Poly();    
        //将结果c保存到a中
        memcpy(a, c, sizeof(c));
    }
}

//母函数方法得出整数划分相应的划分数目
//n:整数
//m:划分方法
void CalPrint(int n, int m)
{
    if(n < m)
    {
        Init(n);
        //由于n小于m，此时按n == m打印
        printf("由于n小于m，所有(%d,%d) = (%d,%d) = %ldn", n, m, n, n, c[n]);
    }
    else
    {
        
        Init(m);
        printf("整数划分(%d,%d)方法数目f(%d,%d) = %ldn", n, m, n, m, c[n]);
    }
}

int main(int argc, char **argv)
{
    int n;
    int m;
    unsigned long count;
    printf("请输入要划分的整数:n");
    scanf("%d", &n);
    printf("请输入划分数:n");
    scanf("%d", &m);
    if(n <= 0) 
    {
        fprintf(stderr, "输入的整数不能为非正数.n");
        return -1;
    }
    if(m <= 0)
    {
        fprintf(stderr, "输入的划分数不能为非正数.n");
        return -1;
    }
    count = GetPartitionCount(n, m);

    printf("方法一：递归法n");
    printf("整数划分（%d,%d)的方法数为：%dnn", n, m, count);
    
    printf("方法二：母函数法n");
    CalPrint(n,m);
    
    #ifdef DEBUG
    int i = 0;
    for( i = 0; i < MAXNUM; i++)
    {
        printf("%9ld ", c[i]);
        if((i + 1) % 10 == 0)
        {
            printf("n");
        }
    }
    printf("n");
    #endif
    return 0;
}