#include "stdio.h"
#include "stdlib.h"
struct edge
{
    int m;
    int n;
    int d;
}a[5010];
int cmp(const void *a,const void *b)//按升序排列
{
    return((struct edge*)a)->d - ((struct edge*)b)->d;
}
int main(void)
{
    int i,n,t,num,min,k,g,x[100];
    printf("请输入顶点的个数：");
    scanf("%d",&n);
    t = n * ( n - 1 ) / 2;
    for(i=1;i<=n;i++)
        x[i]=i;
    printf("\n请输入每条边的起始端点、权值：\n");
    for(i=0;i<t;i++)
    scanf("%d%d%d",&a[i].m,&a[i].n,&a[i].d);//输入每条边的权值
    qsort(a,t,sizeof(a[0]),cmp);
    min=num=0;
        for(i=0;i<t && num < n-1;i++)
        {
            for(k=a[i].m;x[k]!=k;k=x[k])//判断线段的起始点所在的集合
                x[k]=x[x[k]];
            for(g=a[i].n;x[g]!=g;g=x[g])//判断线段的终点所在的集合
                x[g]=x[x[g]];
            if(k!=g)//如果线段的两个端点所在的集合不一样
            {
                x[g]=k;
                min+=a[i].d;
                num++;
                printf("最小生成树中加入边：%d %d\n",a[i].m,a[i].n);
            }
        }
    printf("最小生成树的权值为：%d\n",min);
    system("pause");
    return 0;
}