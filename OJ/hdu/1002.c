/*
Problem : 1002 ( A + B Problem II )     Judge Status : Accepted
RunId : 15878392    Language : C    Author : NEWPLAN
Code Render Status : Rendered By HDOJ C Code Render Version 0.01 Beta
*/
#include <stdio.h>
#include <string.h>
#define MAXLEN 1005
char a1[MAXLEN];
char a2[MAXLEN];
static int v1[MAXLEN];
static int v2[MAXLEN];
static int v3[MAXLEN];
int i,j,n,L,z;
int main(void)
{
    scanf("%d",&n);
    for (j=0; j<n; j++)
    {
        memset(a1,0,MAXLEN);
        memset(a2,0,MAXLEN);

        scanf("%s%s",a1,a2);

        L=strlen(a1);
        for (i=0; i<L; i++)
            v1[i]=a1[L-1-i]-'0';

        L=strlen(a2);
        for (i=0; i<L; i++)
            v2[i]=a2[L-1-i]-'0';

        for (i=0; i<MAXLEN; i++)
            v3[i]=v1[i]+v2[i];

        for (i=0; i<MAXLEN; i++)
        {
            if (v3[i]>=10)
            {
                v3[i+1]+=v3[i]/10;
                v3[i]=v3[i]%10;
            }
        }

        printf("Case %d:\n", j+1);
        printf("%s + %s = ", a1, a2);

        z=0;
        for (i=MAXLEN-1; i>=0; i--)
        {
            if (z==0)
            {
                if (v3[i]!=0)
                {
                    printf("%d",v3[i]);
                    z=1;
                }
            }
            else
            {
                printf("%d",v3[i]);
            }
            v3[i]=0;
            v2[i]=0;
            v1[i]=0;
        }
        if (z==0) printf("0");
        if(j<n-1)
            printf("\n\n");
        else
            printf("\n");
    }
    return 0;
}
