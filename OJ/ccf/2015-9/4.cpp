/*time out?*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>

#define MAXCITY 10005
char citys[MAXCITY][MAXCITY];


void floyd(int sizes)
{
    for(int i=1; i<=sizes; i++)
    {
        for(int j=1; j<=sizes; j++)
        {
        	//printf("city[%d][%d]=%x\n",i,j,citys[i][j]);
            if(citys[i][j])
                continue;
            for(int k=1; k<=sizes; k++)
            {
            	//printf("i=%d\tj=%d\tk=%d\n",i,j,k);
                if(citys[i][k]&&citys[k][j])
                {
                    citys[i][j]=1;
                    break;
                }
            }
        }
    }
}

int counts(int sizes)
{
    int rteval=0;
    for(int i=1; i<=sizes; i++)
    {
        for(int j=1; j<i; j++)
        {
            if(citys[i][j]&&citys[j][i])
                rteval++;
        }
    }
    return rteval;
}

int main(int argc, char* argv[])
{
    int m,n;
    int i,j;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        memset(citys,0,sizeof(citys));
        for(int w=0; w<m; w++)
        {
            scanf("%d%d",&i,&j);
            citys[i][j]=1;
        }
        clock_t start=clock();
        floyd(n);
        printf("%d\n",counts(n));
        clock_t ends=clock();
        printf("using time is %d\n",ends-start);
    }
    return 0;
}
