/*
  背包问题
*/
#include <cstdio>
#include <cstring>
#define MAXVAL 1001

int Weight[MAXVAL];
int Values[MAXVAL];

int status[MAXVAL][MAXVAL];
int visited[MAXVAL][MAXVAL];

#define MAXV(a,b)\
    (a)>(b)?(a):(b)

void inits(void)
{
    memset(Values,0,sizeof(Values));
    memset(Weight,0,sizeof(Weight));
    memset(status,0,sizeof(status));
    memset(visited,0,sizeof(visited));
}

/*********************************************0-1**************************************************/
int changestatus(int i, int j)
{

    if(i<=0||j<=0)
        return 0;
    if(j>=Weight[i])/*此处必须要求能够装得下本物件*/
    {
        if(!visited[i-1][j])
        {
            status[i-1][j]=changestatus(i-1,j);
            visited[i-1][j]=1;
        }

        if(!visited[i-1][j-Weight[i]])
        {
            status[i-1][j-Weight[i]]=changestatus(i-1,j-Weight[i]);
            visited[i-1][j-Weight[i]]=1;
        }

        status[i][j]=MAXV(status[i-1][j],status[i-1][j-Weight[i]]+Values[i]);
    }
    else/*否则就要必须丢下本件物品*/
        status[i][j]=status[i-1][j];
    printf("%d\t%d\t%d\n",i,j,status[i][j]);
    return status[i][j];
}

/*
0-1背包问题
状态转移方程
f[i][v]=max{f[i-1][v], f[i-1][v-w[i]]+v[i]};
f[i][v]表示前i个物件装在容量为v的包里产生的价值
w[i]物件i的重量;
v[i]物件i的价值;
*/
int pack_0_1(int n,int sizes)
{
    return changestatus(n,sizes);
}

void pack_0_1_test(void)
{
	int nums;
    int contains;/*the contain of packages*/
    while(scanf("%d",&contains)!=EOF)
    {
        if(!contains)
            break;
        scanf("%d",&nums);/*number of packages*/
        inits();
        for(int i=1; i<=nums; i++)
            scanf("%d%d",&Weight[i],&Values[i]);
        printf("=========max value is %d============\n",pack_0_1(nums,contains));
        for(int i=0; i<=nums; i++)
        {
            for (int j = 0; j <= contains; ++j)
            {
                printf("%4d  ",status[i][j]);
            }
            printf("\n");
        }
    }
}
/*********************************************0-1**************************************************/

/*********************************************完全**************************************************/
/*
有N种物品和一个容量为V的背包，每种物品都有无限件可用。第i种物品的费用是c[i]，价值是w[i]。
求解将哪些物品装入背包可使这些物品的费用总和不超过背包容量，且价值总和最大。
*/

int main(int argc,char* argv[])
{
    pack_0_1_test();
    return 0;
}
