#include <stdio.h>

//使用一个数组记录在递归过程中产生的前面需要重复输出的值
int set[100];
//用于在递归过程中判断是否递归到最深处，输出回车
int k;

//此函数表示使用不大于m的整数对n进行拆分的情况，i用于表示set数组已经存在的记录数长度
void q(int n,int m,int i)
{
    if(n==k&&n!=m)
    {
        //此时递归栈已经退回到某一分支的最上层，输出回车
        //并重置计数器i为0
        printf("\n");
        i=0;
    }
    if(n==1)
    {
        //当n为1，意味者着只能表示1
        printf("1 ");
        return;
    }
    else if(m==1)
    {
        //当m为1，意味着要输出n个m相加
        for(int i=0; i<n-1; i++)
            printf("1+");
        printf("1 ");
        return;
    }
    if(n<m)
    {
        q(n,n,i);
    }
    if(n==m)
    {
        //当n等于m时，到达本次递归求和的一个叶子，此时需要输出多一个空格，表示下一次输出为另一个叶子
        printf("%d ",n);
        //在递归输出另一个叶子之前，将之前记录的在叶子之上的数一并输出，如上图示过程1
        for(int j=0; j<i; j++)
            printf("%d+",set[j]);
        q(n,m-1,i);

    }
    if(n>m)
    {
        //如果n大于m，使用m作为分解，则要首先输出m+的形式
        printf("%d+",m);
        //记录下作为树干节点m的值并使i自增
        set[i++]=m;
        //递归输出m+以后的分解
        q(n-m,m,i);
        //递归完毕后需要将数组记录后退一个，回到上一个节点，如上图示过程2
        i--;
        //执行另一个分支，在下一次递归之前输出记录的数据，如上图示过程3
        for(int j=0; j<i; j++)
            printf("%d+",set[j]);
        //递归输出另一分支情况
        q(n,m-1,i);
    }


}

int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        if(n<=0)
        {
            printf("Please input a positive interger.\n\n");
            continue;
        }
        k=n;
        q(n,n,0);
        printf("\n\n");
    }
    return 0;
}
