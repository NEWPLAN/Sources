#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int compare(const void *a, const void *b)
{
    return *(int *)a-*(int *)b;
}

int main(int argc,char * argv[])
{
    char ppt[1002];
    int a[1000];
    char *p=ppt;
    int i=0;
    while(scanf("%s",ppt)!=EOF)
    {
        i=0;
        a[0]=0;
        while(*p)
        {
            if(*p!='5')
            {
                a[i]=a[i]*10+(*p-'0');
                p++;
            }
            else
            {
                while(*p=='5')
                    p++;
                i++;
                a[i]=0;
            }
        }
        qsort((void *)a,i,4,compare);
        {
            int k=0;
            for(k=0;k<i-1;k++)
                printf("%d ",a[k]);
            printf("%d\n",a[k]);
        }
        p=ppt;
        memset(ppt,0,1002);
    }
    return 0;
}
