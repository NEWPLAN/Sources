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
            /*ͳ�����*/
            for(int j=0; j<=1000; j++)
                if(coefficients2[j])
                {
                    /*ͳ�Ƹ��������֮ǰϵ��Ϊ0��֮��Ϊ0�������+1�����֮ǰϵ����0��֮��Ϊ0������-1*/
                    float first_is_not_nil=coefficients3[i+j];
                    coefficients3[i+j]+=coefficients[i]*coefficients2[j];
                    if(!first_is_not_nil && coefficients3[i+j])
                        cc++;
                    else if(first_is_not_nil && !coefficients3[i+j])
                        cc--;
                }
        }
    }
    cout<<cc;
    for(i=2000; i>=0; i--)
        if(coefficients3[i])
        {
            printf(" %d %.1f",i,coefficients3[i]);
        }
    return 0;
}
