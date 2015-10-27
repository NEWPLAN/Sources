//201312-1
#include<cstdio>
#include<cmath>
#include<cstring>
#include<algorithm>
#include<iostream>
using namespace std;
int t[10005];
char p[10001][10001];
int main()
{
    int n,i;
    cin>>n;
    memset(t,0,sizeof(t));
    while(n--)
    {
        cin>>i;
        t[i]++;
    }
    int maxn=-1,maxnum=0;
    for(i=1; i<=10000; i++)
    {
        if(t[i]>maxn)
        {
            maxnum=i;
            maxn=t[i];
        }
    }
    cout<<maxnum<<endl;
    return 0;
}
