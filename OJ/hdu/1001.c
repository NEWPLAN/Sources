/*
Problem : 1001 ( Sum Problem )     Judge Status : Wrong Answer
RunId : 15860308    Language : C    Author : NEWPLAN
Code Render Status : Rendered By HDOJ C Code Render Version 0.01 Beta

*/
#include <stdio.h>

int main(void)
{
    int i,j,k;
    while(scanf("%d",&i)!=-1)
    {
        j=i+1;
        if(j&1)
        {
            k=i/2;
            k*=j;
        }
        else
        {
            k=j/2;
            k*=i;
        }
        printf("%d\n",k);
    }
    return 0;
}
