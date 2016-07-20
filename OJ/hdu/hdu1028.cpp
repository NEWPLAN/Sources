/*
Problem : 1028 ( Ignatius and the Princess III )     Judge Status : Accepted
RunId : 13305215    Language : G++    Author : NEWPLAN
Code Render Status : Rendered By HDOJ G++ Code Render Version 0.01 Beta
五边形数定理
*/
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;
const int N=10005;

/*		+int c1[N],c2[N];
c1存储的是前k-1个式子的合并项，C2存储的是第K个式子的系数值
*/
int c1[20],c2[20];
/**
-(1+x+x^2+x^3+x^4+...+x^n)(1+x^2+x^(2*2)+x^(3*2)+x^(4*2)+...+x^(n*2))
-(1+x^3+x^(2*3)+x^(3*3)+x^(4*3)+...+x^(n*3))
-(1+x^4+x^(2*4)+x^(3*4)+x^(4*4)+...+x^(n*4))
-.......
-(1+x^n+x^(2*n)+x^(3*n)+x^(4*n)+...+x^(n*n))
*/
int main()
{
    /*
    i表示和第i行相乘取得系数，j表示前i-1行中的第j个系数（分别取出与第i行相乘获取系数，存入对应项（j+k））
    ,k表示的是系数为k（每次递增i，因为是i的倍数）
    */
    int n,i,j,k;
    while(cin>>n)
    {
        if(n==0) break;
        for(i=0; i<=n; i++)
        {
            c1[i]=1;
            c2[i]=0;
        }
        for(i=2; i<=n; i++)
        {
            for(j=0; j<=n; j++)
                for(k=0; k+j<=n; k+=i)
                    c2[k+j]+=c1[j];
            for(j=0; j<=n; j++)
            {
                c1[j]=c2[j];
                c2[j]=0;
            }
        }
        cout<<c1[n]<<endl;
    }
    return 0;
}
