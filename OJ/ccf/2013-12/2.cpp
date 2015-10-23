#include <stdio.h>

int main(void)
{
    int a[9]={0},temp=0,i;
    char b;
    scanf("%d-%1d%1d%1d-%1d%1d%1d%1d%1d-%c",&a[0],&a[1],&a[2],&a[3],&a[4],&a[5],&a[6],&a[7],&a[8],&b);
    for( i=1;i<=9;i++)
    {
        temp=temp+a[i-1]*i;
        temp=temp%11;
    }
    if(temp==10)
        if(b=='X')
             printf("Right");
        else
            printf("%d-%1d%1d%1d-%1d%1d%1d%1d%1d-X",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8]);
    else
    {
        if(temp==b-'0')
            printf("Right");
        else
            printf("%d-%1d%1d%1d-%1d%1d%1d%1d%1d-%d",a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7],a[8],temp);
    }

    return 0;
}
