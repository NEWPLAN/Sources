#include <iostream>
#include <cstdio>

using namespace std;

int main(void)
{
    int n,i,cc=0;
    float coefficients[1001]= {0},coefficients2[1001]= {0},coefficients3[2001]= {0},temps;
    cin>>n;
    for(int a=0; a<n; a++)
    {
        cin>>i>>temps;
        coefficients[i]+=temps;
    }
    cin>>n;
    for(int a=0; a<n; a++)
    {
        cin>>i>>temps;
        coefficients2[i]+=temps;
    }
    for(int i=0; i<=1000; i++)
    {
        if(coefficients[i])
        {
            for(int j=0; j<=1000; j++)
                if(coefficients2[j])
                {
                    if(!coefficients3[i+j])
                        cc++;
                    coefficients3[i+j]+=coefficients[i]*coefficients2[j];
                }
        }
    }
    //cout<<cc;
    /*统计系数相*/
    cc=0;
    for(i=0;i<=2000;i++)
        if(coefficients3[i])
            cc++;
    cout<<cc;
    for(i=2000; i>=0; i--)
        if(coefficients3[i])
        {
            printf(" %d %.1f",i,coefficients3[i]);
        }
    return 0;
}
