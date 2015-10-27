#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;

const int N=10005;

/*
c1�洢����ǰk-1��ʽ�ӵĺϲ��C2�洢���ǵ�K��ʽ�ӵ�ϵ��ֵ
*/
int c1[20],c2[20];
/**
(1+x+x^2+x^3+x^4+...+x^n)(1+x^2+x^(2*2)+x^(3*2)+x^(4*2)+...+x^(n*2))
(1+x^3+x^(2*3)+x^(3*3)+x^(4*3)+...+x^(n*3))
(1+x^4+x^(2*4)+x^(3*4)+x^(4*4)+...+x^(n*4))
.......
(1+x^n+x^(2*n)+x^(3*n)+x^(4*n)+...+x^(n*n))
*/
int main()
{
    int n,i,j,k;/*i��ʾ�͵�i�����ȡ��ϵ����j��ʾǰi-1���еĵ�j��ϵ�����ֱ�ȡ�����i����˻�ȡϵ���������Ӧ�j+k����,k��ʾ����ϵ��Ϊk��ÿ�ε���i����Ϊ��i�ı�����*/
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