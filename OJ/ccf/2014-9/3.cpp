#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char pattern[101];
char text[101];
char patternuper[101];
char textuper[101];

int mycomp(char* A,char* patt)
{
    if(strstr(A,patt)!=NULL)
        return 1;
    return 0;
}

int main(void)
{
    int switchs;
    int n,i=0;
    scanf("%s%d%d",pattern,&switchs,&n);
    while(n--)
    {
        memset(text,0,101);
        memset(textuper,0,101);
        scanf("%s",text);
        if(switchs)
        {
            if(mycomp(text,pattern))
                printf("%s\n",text);
        }
        else
        {
            strcpy(patternuper,strupr((pattern)));
            strcpy(textuper,text);
            strcpy(textuper,strupr((textuper)));
            if(mycomp(textuper,patternuper))
                printf("%s\n",text);

        }
    }
    return 0;
}
