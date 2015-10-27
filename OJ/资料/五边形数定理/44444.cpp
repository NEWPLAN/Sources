/*----------------------------------------------
 *        Author    :��������love
 *        Date    :2013-02-28
 *        Email    :9974**140@qq.com
 *        Copyright:anyone
-----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG 
//�ݹ鷨�����������
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


//ĸ����������������

#define MAXNUM 100            //��ߴ���
unsigned long a[MAXNUM];
unsigned long b[MAXNUM];
unsigned long c[MAXNUM];    //������

//��������ʽ���г˷���ϵ���ֱ𱣴���a��b�У�������浽c�������������MAXNUM
void Poly()
{
    int i;
    int j;
    memset(c, 0, sizeof(c));
    for(i = 0; i < MAXNUM; i++)
    {
        for(j = 0; j < MAXNUM - i; j++)    //j < MAXNUM - i,ȷ��i+j��Խ��
        {
            c[i + j] += a[i] * b[j];
        }
    }
}
//����ǰN���ϵ������g(x,1)*g(x,2)*....*g(x,n)��չ�����
void Init(int m)
{
    int i;
    int j;
    memset(a, 0, sizeof(a));
    memset(c, 0, sizeof(c));
    //��һ������ʽ:g(x) = x^0 + x^1 + x^2 + ... + x^n
    for(i = 0; i < MAXNUM; i++)
    {
        a[i] = 1;
    }
    //for(j = 2; j <= MAXNUM; j++)//ֻ����f(n,n)
    //ͨ���޸����ʹ�ÿ�����f(n,m)�����������������n,m���ʺ�
    for(j = 2; j <= m; j++)
    {
        memset(b, 0, sizeof(b));
        //��i������ʽ:g(x) = x^0 + x^i + x^(2k) + ...
        for(i = 0; i <= MAXNUM; i += j)
        {
            b[i] = 1;
        }
        //����ʽ���:c = a * b
        Poly();    
        //�����c���浽a��
        memcpy(a, c, sizeof(c));
    }
}

//ĸ���������ó�����������Ӧ�Ļ�����Ŀ
//n:����
//m:���ַ���
void CalPrint(int n, int m)
{
    if(n < m)
    {
        Init(n);
        //����nС��m����ʱ��n == m��ӡ
        printf("����nС��m������(%d,%d) = (%d,%d) = %ldn", n, m, n, n, c[n]);
    }
    else
    {
        
        Init(m);
        printf("��������(%d,%d)������Ŀf(%d,%d) = %ldn", n, m, n, m, c[n]);
    }
}

int main(int argc, char **argv)
{
    int n;
    int m;
    unsigned long count;
    printf("������Ҫ���ֵ�����:n");
    scanf("%d", &n);
    printf("�����뻮����:n");
    scanf("%d", &m);
    if(n <= 0) 
    {
        fprintf(stderr, "�������������Ϊ������.n");
        return -1;
    }
    if(m <= 0)
    {
        fprintf(stderr, "����Ļ���������Ϊ������.n");
        return -1;
    }
    count = GetPartitionCount(n, m);

    printf("����һ���ݹ鷨n");
    printf("�������֣�%d,%d)�ķ�����Ϊ��%dnn", n, m, count);
    
    printf("��������ĸ������n");
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