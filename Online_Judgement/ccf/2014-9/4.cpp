#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<iostream>
#include<queue>

using namespace std;

struct State
{
    int x,y;
    int layer;
    State(int _x,int _y,int _l=0):x(_x),y(_y),layer(_l) {}
    State()
    {
        x=y=layer=0;
    }
};

int data[1000+5][1000+5]= {0};
//0 0/1 vis     1 0/1 client  other clients needs;
std::queue<State> que;

int n,m,k,d,x,y,z;
long long ans;
void bfs();
int main()
{
    scanf("%d%d%d%d",&n,&m,&k,&d);
    for(int i=0; i<m; i++) //·Öµê
    {
        scanf("%d%d",&x,&y);
        que.push(State(x,y));
    }
    for(int i=0; i<k; i++)
    {
        scanf("%d%d%d",&x,&y,&z);
        data[x][y]=(data[x][y]|2)+(z<<2);
    }
    for(int i=0; i<d; i++)
    {
        scanf("%d%d",&x,&y);
        data[x][y]|=1;
    }
    bfs();
    return 0;
}
void bfs()
{
    ans=0;
    State tmp,cur;
    while(!que.empty())
    {
        cur=que.front();
        que.pop();
        int curx=cur.x,cury=cur.y,curl=cur.layer;
        if(curx>0&&cury>0&&curx<=n&&cury<=n&&!(data[curx][cury]&1))
        {
            data[curx][cury]|=1;
            que.push(State(curx+1,cury,curl+1));
            que.push(State(curx,cury+1,curl+1));
            que.push(State(curx-1,cury,curl+1));
            que.push(State(curx,cury-1,curl+1));
            if(data[curx][cury]&2)
                ans+=(data[curx][cury]>>2)*curl;
        }
    }
    cout<<ans;
    //printf("%d",ans);
}
