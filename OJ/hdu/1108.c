#include<stdio.h>

int gcd(int a,int b)
{
//    printf("%d %d\n",a,b);
    int temp;
    if(!b)
        return a?a:1;
    else
    {
        if(a<b)
        {
            a+=b;
            b=a-b;
            a=a-b;
            if(b==0)
                return a;
        }
        temp=a%b;
        a=b;
        b=temp;
        return gcd(a,b);
    }
}

int main(void)
{
    int a,b;
    while(scanf("%d%d",&a,&b)!=-1)
    {
        printf("%d\n",a/(gcd(a,b))*b);
    }
    return 0;
}
