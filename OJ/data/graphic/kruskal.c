#include "stdio.h"
#include "stdlib.h"
struct edge
{
    int m;
    int n;
    int d;
}a[5010];
int cmp(const void *a,const void *b)//����������
{
    return((struct edge*)a)->d - ((struct edge*)b)->d;
}
int main(void)
{
    int i,n,t,num,min,k,g,x[100];
    printf("�����붥��ĸ�����");
    scanf("%d",&n);
    t = n * ( n - 1 ) / 2;
    for(i=1;i<=n;i++)
        x[i]=i;
    printf("\n������ÿ���ߵ���ʼ�˵㡢Ȩֵ��\n");
    for(i=0;i<t;i++)
    scanf("%d%d%d",&a[i].m,&a[i].n,&a[i].d);//����ÿ���ߵ�Ȩֵ
    qsort(a,t,sizeof(a[0]),cmp);
    min=num=0;
        for(i=0;i<t && num < n-1;i++)
        {
            for(k=a[i].m;x[k]!=k;k=x[k])//�ж��߶ε���ʼ�����ڵļ���
                x[k]=x[x[k]];
            for(g=a[i].n;x[g]!=g;g=x[g])//�ж��߶ε��յ����ڵļ���
                x[g]=x[x[g]];
            if(k!=g)//����߶ε������˵����ڵļ��ϲ�һ��
            {
                x[g]=k;
                min+=a[i].d;
                num++;
                printf("��С�������м���ߣ�%d %d\n",a[i].m,a[i].n);
            }
        }
    printf("��С��������ȨֵΪ��%d\n",min);
    system("pause");
    return 0;
}