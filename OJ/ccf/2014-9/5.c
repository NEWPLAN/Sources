#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define M 1000000007


unsigned int matrix[128][128]= {0};
int Mval=0;


int getposition(char a)
{
    int index=0;
    for(index=0; index<Mval; index++)
        if((a&(1<<index))==0)
            return index;
    return index;
}

void getnext(char nnn,char input,char output,int blocks)
{
    int index =getposition(input);
    if(input==((1<<Mval)-1)) 
    {
//            matrix[input][output]++;
            return;
    }
    switch(blocks)
    {
    case 1:
        // xx
        // x
        if(index>=(Mval-1)||(input&(1<<(index+1)))||(output&(1<<index)))//unreachable
            return ;
        input+=((1<<index)+(1<<(index+1)));
        output+=(1<<index);
        if(input==((1<<Mval)-1))   //到达一个可行状态,删除一个可行状态，进行下一步迭代
        {
#if __DEBUG__
            printf("reachalbe from %x to %x\n",nnn,output);
#endif
            matrix[nnn][output]++;
            return ;
        }
        getnext(nnn,input,output,1);
        getnext(nnn,input,output,2);
        getnext(nnn,input,output,3);
        getnext(nnn,input,output,4);
        break;
    case 2:
        // xx
        //  x
        if(index>=(Mval-1)||(input&(1<<(index+1)))||(output&(1<<(index+1))))//unreachable
            return ;
        input+=((1<<index)+(1<<(index+1)));
        output+=(1<<(index+1));
        if(input==((1<<Mval)-1))   //到达一个可行状态,删除一个可行状态，进行下一步迭代
        {
#if __DEBUG__
            printf("reachalbe from %x to %x\n",nnn,output);
#endif
            matrix[nnn][output]++;
            return ;
        }
        getnext(nnn,input,output,1);
        getnext(nnn,input,output,2);
        getnext(nnn,input,output,3);
        getnext(nnn,input,output,4);
        break;
    case 3:
        // x
        // xx
        if(index>=(Mval-1)||(output&(1<<(index+1)))||(output&(1<<index)))//unreachable
            return ;
        input+=((1<<index));
        output+=((1<<index)+(1<<(index+1)));
        if(input==((1<<Mval)-1))   //到达一个可行状态,删除一个可行状态，进行下一步迭代
        {
#if __DEBUG__
            printf("reachalbe from %x to %x\n",nnn,output);
#endif
            matrix[nnn][output]++;
            return ;
        }
        getnext(nnn,input,output,1);
        getnext(nnn,input,output,2);
        getnext(nnn,input,output,3);
        getnext(nnn,input,output,4);
        break;
    case 4:
        // x
        //xx
        if(index==0||(output&(1<<(index-1)))||(output&(1<<(index))))//unreachable
            return ;
        input+=(1<<index);
        output+=((1<<index)+(1<<(index-1)));
        if(input==((1<<Mval)-1))   //到达一个可行状态,删除一个可行状态，进行下一步迭代
        {
#if __DEBUG__
            printf("reachalbe from %x to %x\n",nnn,output);
#endif
            matrix[nnn][output]++;
            return ;
        }
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
    int loops=(1<<Mval);
    for(index=0; index<loops; index++)
    {
        getnext(index,index,0,1);
        getnext(index,index,0,2);
        getnext(index,index,0,3);
        getnext(index,index,0,4);
    }
    matrix[index-1][0]++;

    return ;

}

void printmatrix(unsigned int matrix[][128])
{
    int ral, cow;
    printf("   ");
    for(cow=0; cow<(1<<Mval); cow++)
        printf("|%.3x",cow);
    printf("\n");
    for(ral=0; ral<(1<<Mval); ral++)
    {
        printf("%.3x",ral);
        for(cow=0; cow<(1<<Mval); cow++)
            if(matrix[ral][cow]==0)
                printf("|   ");
            else
                printf("|%.3d",matrix[ral][cow]);
        printf("\n");
    }
}

void FastExponentiation(unsigned int src[][128], unsigned int results[][128], long long times)
{
    if(times==0)return;
    if (times == 1)
    {
        memcpy(results, src, sizeof(unsigned int) * 128 * 128);
        return;
    }
    unsigned int sss[128][128] = {0};
    long long temp = 0;
    FastExponentiation(src, sss, times / 2);
    int i, j, k;
    for (i = 0; i < (1<<Mval); i++)
        for (j = 0; j < (1<<Mval); j++)
            for (k = 0; k < (1<<Mval); k++)
            {
                temp = (long long)sss[i][k] * sss[k][j];
                results[i][j] += (temp % M) ;
                results[i][j] %= M;
            }
#if __DEBUG__
    printf("        cccccc %d\n\n\n",times);
    printmatrix(sss);
    printmatrix(results);
    printf("        cccccc\n\n\n");
#endif // __DEBUG__
    if (times % 2)
    {
        memcpy(sss, results, sizeof(unsigned int) * 128 * 128);
        memset(results,0,sizeof(unsigned int) * 128 * 128);
        for (i = 0; i < (1<<Mval); i++)
            for (j = 0; j < (1<<Mval); j++)
                for (k = 0; k < (1<<Mval); k++)
                {
                    temp = (long long)sss[i][k] * src[k][j];
                    results[i][j] += (temp % M);
                    results[i][j] %= M;
                }
    }
#if __DEBUG__
    printmatrix(sss);
    printmatrix(results);
#endif // __DEBUG__
    return ;

}

void FastExponentiation2(unsigned int src[][128], unsigned int results[][128], long long times)
{
    unsigned int s1[128][128] = {0};
    unsigned int s2[128][128] = {0};
    unsigned int s3[128][128] = {0};
    long long temp = 0;
    int i, j, k;
    for(i=0; i<128; i++)results[i][i]=1;
//    printmatrix(results);
    memcpy(s1,src,sizeof(unsigned int)*128*128);
    while(times)
    {
//        printmatrix(s1);
        memset(s2,0,sizeof(unsigned int)*128*128);
        if(times&1)
        {
            memcpy(s3,results,sizeof(unsigned int)*128*128);
            memset(results,0,sizeof(unsigned int)*128*128);
//           printmatrix(s3);
            for (i = 0; i < (1<<Mval); i++)
                for (j = 0; j < (1<<Mval); j++)
                    for (k = 0; k < (1<<Mval); k++)
                    {
                        temp = (long long)s3[i][k] * s1[k][j];
                        results[i][j] += (temp % M) ;
                        results[i][j] %= M;
                    }
        }
//       printmatrix(results);
        times=times>>1;
        for (i = 0; i < (1<<Mval); i++)
            for (j = 0; j < (1<<Mval); j++)
                for (k = 0; k < (1<<Mval); k++)
                {
                    temp = (long long)s1[i][k] * s1[k][j];
                    s2[i][j] += (temp % M) ;
                    s2[i][j] %= M;
                }
        memcpy(s1,s2,sizeof(unsigned int)*128*128);
    }
    return ;

}
//#define __DEBUG__ 1
int main(int argc, char* argv[])
{
//    freopen("out.txt", "w", stdout);
    int m;
    long long n;
    unsigned int sss[128][128] = {0};

    scanf("%lld%d",&n,&m);
    Mval=m;
    initstates();
#if __DEBUG__
    printmatrix(matrix);
    printf("\n\n\n");
#endif
//   FastExponentiation(matrix,sss,n-1);
//    printf("%d\n",sss[0][(1<<m)-1]);
    memset(sss,0,sizeof(unsigned int)*128*128);
    FastExponentiation2(matrix,sss,n-1);
    printf("%d\n",sss[0][(1<<m)-1]);
#if __DEBUG__
    printmatrix(sss);
    printf("\n\n\n");
    int j=2;
    for (j=2; j<=n; j++)
    {
        memset(sss,0,sizeof(unsigned int)*128*128);
        printf("\n\n%d\n",j);
        FastExponentiation(matrix,sss,j-1);
        printmatrix(sss);
    }
#endif
    return 0;
}