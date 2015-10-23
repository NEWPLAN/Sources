#include <stdio.h>

int main(void)
{
    int A[1001]={0};
    int a=0;
    int t=0;
    scanf("%d",&a);
    int i=0;
    while(a--)
    {
        scanf("%d",&t);
        A[t]++;
        if(A[i]<A[t])
            i=t;
    }

    printf("%d",i);
    return 0;
}
