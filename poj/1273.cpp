#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

int G[300][300];
int prev[300];

bool visited[300];

int n,m;

unsigned Augment(void)
{
    int v,i;
    deque<int> q;
    memset(prev,0,sizeof(prev));
    memset(visited,0,sizeof(visited));
    prev[1]=0;
    visited[1]=1;
    q.push_back(1);
    bool bfindpath=false;
    while(!q.empty())
    {
        v=q.front();
        q.pop_front();
        for(i=1; i<=m; i++)
        {
            if(G[v][i]>0&&(!visited[i]))
            {
                prev[i]=v;
                visited[i]=1;
                if(i==m)
                {
                    bfindpath=true;
                    q.clear();
                    break;
                }
                else
                {
                    q.push_back(i);
                }
            }
        }
    }
    if(!bfindpath)
        return 0;
    int nminflow=1<<20;
    v=m;
    while(prev[v])
    {
        nminflow=min(nminflow,G[prev[v]][v]);
        v=prev[v];
    }
    v=m;
    while(prev[v])
    {
        G[prev[v]][v]-=nminflow;
        G[v][prev[v]]+=nminflow;
        v=prev[v];
    }
    return nminflow;
}
int main(void)
{
    while(cin>>n>>m)
    {
        int i,j,k;
        int s,e,c;
        memset(G,0,sizeof(G));
        for(i=0; i<n; i++)
        {
            cin>>s>>e>>c;
            G[s][e]+=c;
        }
        unsigned int maxflow=0;
        unsigned int aug;
        while(aug=Augment())
            maxflow+=aug;
        cout<<maxflow<<endl;
    }
    return 0;
}
