#include <iostream>
using namespace std;
#include <string.h>
#include <algorithm>
int n,m;
int head[10010],stackn[10010],DFN[10010],Low[10010];
int Belong[10010],instack[10010],cnt,top,f,scnt;
struct node
{
    int e,next;
} edge[100010];
void add(int s,int e)
{
    edge[f].e=e;
    edge[f].next=head[s];
    head[s]=f++;
}
void tarjan(int s)
{
    int t,k,i;
    DFN[s]=Low[s]=++cnt;
    stackn[top++]=s;
    instack[s]=1;
    for(i=head[s]; i!=-1; i=edge[i].next)
    {
        k=edge[i].e;
        if(!DFN[k])
        {
            tarjan(k);
            Low[s]=min(Low[k],Low[s]);
        }
        else if(instack[k])
        {
            Low[s]=min(Low[s],DFN[k]);
        }
    }
    if(Low[s]==DFN[s])
    {
        scnt++;
        do
        {
            t=stackn[--top];
            Belong[t]=scnt;
            instack[t]=0;
        }
        while(s!=t);
    }
}
int main()
{
    int i,j,a,b,v[10010];
    cin>>n>>m;
    f=1;
    memset(head,-1,sizeof(head));
    memset(v,0,sizeof(v));
    for(i=0; i<m; i++)
    {
        cin>>a>>b;
        add(a,b);
    }
    memset(DFN,0,sizeof(DFN));
    for(i=1; i<=n; i++)
    {
        if(!DFN[i])
            tarjan(i);
    }
    for(i=1; i<=n; i++)
        v[Belong[i]]++;
    long long sum=0;
    for(i=0; i<=n; i++)
    {
        if(v[i]>1)
            sum=sum+v[i]*(v[i]-1)/2;
    }
    cout<<sum<<endl;
    return 0;
}

