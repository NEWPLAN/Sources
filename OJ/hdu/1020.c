/*
Problem : 1020 ( Encoding )     Judge Status : Accepted
RunId : 15860397    Language : C    Author : NEWPLAN
Code Render Status : Rendered By HDOJ C Code Render Version 0.01 Beta
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(void)
{
    char ps[10001],*tt;
    int icn=0,ccc;
    scanf("%d",&icn);
    while(icn--)
    {
        memset(ps,0,1001);
        scanf("%s",ps);
        tt=ps;
    again:
        ccc=1;
        while((*tt)==*(tt+1))
        {
            ccc++;
            tt++;
        }

        if(ccc==1)
            printf("%c",*tt);
        else
            printf("%d%c",ccc,*tt);
        tt++;
        if(*tt)
            goto again;
        else
            printf("\n");
    }
    return 0;
}
