#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int t=0,m=0,n=0;
    char nn[1100];
    scanf("%d",&t);
    while(t--)
    {
        int results=1;
        long long tmp=1;
        int index=0;
        memset(nn,0,sizeof(nn));
        scanf("%s%d",nn,&m);
        if(strlen(nn)>=7)
            n=m;
        else
            n=atoi(nn);
        for(index=1;index<=n;index++)
        {
            tmp=(tmp*index)%m;//乘法溢出
            results=(results+tmp)%m;
        }
        printf("%d\n",results%m);
    }
    return 0;
}
