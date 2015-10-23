#include <iostream>
#include <queue>
#include <cstring>
#include <vector>
#include <algorithm>


using namespace std;

int G[300][300];
int layer[300];

bool visited[300];

int n,m;

bool countlayer(void)
{
    int layers=0;
    deque<int> q;
    memset(layer,0xff,sizeof(layer));
    layer[1]=0;
    q.push_back(1);
    while(!q.empty())
    {
        int v=q.front();
        q.pop_front();
        for(int j=1;j<=m;j++)
        {
            if(G[v][j]>0&&layer[j]!=0xff)
            {
                layer[j]=layer[v]+1;
                if(j==m)
                    return true;
                else
                    q.push_back(j);
            }
        }
    }
    return false;

}

int Dicnic(void)
{
    int i,s;
    int nmaxflow=0;
    deque<int> q;
    while(countlayer())
    {
        q.push_back(1);
        memset(visited,0,sizeof(visited));
        visited[1]=1;
        while(!q.empty())
        {
            int nd=q.back();
            if(nd==m)
            {
                int nminc=1<<20;
                int nminc_vs;/*capacity min starts*/
                for(i=1;i<q.size();i++)
                {
                    int vs=q[i-1];
                    int ve=q[i];
                    if(G[vs][ve]>0)
                    {
                        if(nminc>G[vs][ve])
                        {
                            nminc=G[vs][ve];
                            nminc_vs=vs;
                        }
                    }
                }
                nmaxflow+=nminc;
                for(i=1;i<q.size();i++)
                {
                    int vs=q[i-1];
                    int ve=q[i];
                    G[vs][ve]-=nminc;
                    G[ve][vs]+=nminc;
                }
                /*back to nminc_vs for new dfs*/
                while(!q.empty()&&q.back()!=nminc_vs)
                {
                    visited[q.back()]=0;
                    q.pop_back();
                }
            }
            else
            {
                for(i=1;i<=m;i++)
                {
                    if(G[nd][i]>0&&layer[i]==layer[nd]+1&&!visited[i])
                    {
                        visited[i]=1;
                        q.push_back(i);
                        break;
                    }
                }
                if(i>m)
                    q.pop_back();
            }
        }
    }
    return nmaxflow;
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

        cout<<Dicnic()<<endl;
    }
    return 0;
}
