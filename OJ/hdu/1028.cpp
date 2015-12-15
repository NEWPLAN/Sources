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

int c1[N],c2[N];

int main()
{
    int n,i,j,k;
    while(cin>>n)
    {
        if(n==0) break;
        for(i=0;i<=n;i++)
        {
            c1[i]=1;
            c2[i]=0;
        }
        for(i=2;i<=n;i++)
        {
            for(j=0;j<=n;j++)
                for(k=0;k+j<=n;k+=i)
                    c2[k+j]+=c1[j];
            for(j=0;j<=n;j++)
            {
                c1[j]=c2[j];
                c2[j]=0;
            }
        }
        cout<<c1[n]<<endl;
    }
    return 0;
}
