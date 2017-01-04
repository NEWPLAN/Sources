#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int matrix[128][128]={0};

int getmin(int a, int b)
{
    return a>b?a:b;
}
int getmax(int a, int b)
{
    return a<b?a:b;
}

int getposition(char a)
{
    int index=0;
    for(index=0;index<7;index++)
        if(a&(1<<index))
            return index;
    return -1;
}

void getnext(char nnn,char input,char output,int blocks)
{
    char step1=input;
    char step2=output;
    if(input==0x7f)//到达一个可行状态,删除一个可行状态，进行下一步迭代
    {
        printf("reachalbe\n");
        matrix[nnn][output]++;
        return ;
    }
    int index =getposition(input);
    switch(blocks)
    {
    case 1:
        // xx
        // x
        if(index>=7||(input&(1<<(index+1)))||(output&(1<<index)))//unreachable
            return ;
        input+=((1<<index)+(1<<(index+1)));
        output+=(1<<index);
        getnext(nnn,input,output,1);
        getnext(nnn,input,output,2);
        getnext(nnn,input,output,3);
        getnext(nnn,input,output,4);
        break;
    case 2:
        // xx
        //  x
        if(index>=7||(input&(1<<(index+1)))||(output&(1<<(index+1))))//unreachable
            return ;
        input+=((1<<index)+(1<<(index+1)));
        output+=(1<<(index+1));
        getnext(nnn,input,output,1);
        getnext(nnn,input,output,2);
        getnext(nnn,input,output,3);
        getnext(nnn,input,output,4);
        break;
    case 3:
        // x
        // xx
        if(index>=7||(output&(1<<(index+1)))||(output&(1<<index)))//unreachable
            return ;
        input+=((1<<index));
        output+=((1<<index)+(1<<(index+1)));
        getnext(nnn,input,output,1);
        getnext(nnn,input,output,2);
        getnext(nnn,input,output,3);
        getnext(nnn,input,output,4);
        break;
    case 4:
        // x
        //xx
        if(index<=1||(output&(1<<(index-1)))||(output&(1<<(index))))//unreachable
            return ;
        input+=((1<<index));
        output+=(1<<(index-1)+(1<<(index-1)));
        getnext(nnn,input,output,1);
        getnext(nnn,input,output,2);
        getnext(nnn,input,output,3);
        getnext(nnn,input,output,4);
        break;
    default:
        break;
    }
    return ;
}


void initstates(void)
{
    int index=0;
    int loops=0x80;
    for(index=0;index<loops;index++)
    {
        getnext(index,index,0,1);
        getnext(index,index,0,2);
        getnext(index,index,0,3);
        getnext(index,index,0,4);
    }

    return ;

}


int main(int argc, char* argv[])
{
    int m,n;;
    scanf("%d%d",&n,&m);
    initstates();
    return 0;
}
