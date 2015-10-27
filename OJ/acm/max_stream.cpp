/*
Description

  Every time it rains on Farmer John's fields, a pond forms over Bessie's favorite clover patch. This means that the clover is covered by water for awhile and takes quite a long time to regrow. Thus, Farmer John has built a set of drainage ditches so that Bessie's clover patch is never covered in water. Instead, the water is drained to a nearby stream. Being an ace engineer, Farmer John has also installed regulators at the beginning of each ditch, so he can control at what rate water flows into that ditch. 
Farmer John knows not only how many gallons of water each ditch can transport per minute but also the exact layout of the ditches, which feed out of the pond and into each other and stream in a potentially complex network. 
Given all this information, determine the maximum rate at which water can be transported out of the pond and into the stream. For any given ditch, water flows in only one direction, but there might be a way that water can flow in a circle.

Input

  The input includes several cases. For each case, the first line contains two space-separated integers, N (0 <= N <= 200) and M (2 <= M <= 200). N is the number of ditches that Farmer John has dug. M is the number of intersections points for those ditches. Intersection 1 is the pond. Intersection point M is the stream. Each of the following N lines contains three integers, Si, Ei, and Ci. Si and Ei (1 <= Si, Ei <= M) designate the intersections between which this ditch flows. Water will flow through this ditch from Si to Ei. Ci (0 <= Ci <= 10,000,000) is the maximum rate at which water will flow through the ditch.

Output

  For each case, output a single integer, the maximum rate at which water may emptied from the pond.

Sample Input

5 4

1 2 40

1 4 20

2 4 20

2 3 30

3 4 10

Sample Output

50
*/

#include <iostream>
#include <queue>
#include <cstdio>
using namespace std;
  
 const int N = 210;
 const int INF = 0x7FFFFFFF;
 int n,m,map[N][N],path[N],flow[N],start,end;
 queue<int> q;
  
 int bfs(){
     int i,t;
     while(!q.empty()) q.pop();
     memset(path,-1,sizeof(path));
     path[start]=0,flow[start]=INF;
     q.push(start);
     while(!q.empty()){
         t=q.front();
         q.pop();
         if(t==end) break;
         for(i=1;i<=m;i++){
             if(i!=start && path[i]==-1 && map[t][i]){
                 flow[i]=flow[t]<map[t][i]?flow[t]:map[t][i];
                 q.push(i);
                 path[i]=t;
             }
         }
     }
     if(path[end]==-1) return -1;
     return flow[m];                   //一次遍历之后的流量增量
 }
 int Edmonds_Karp(){
     int max_flow=0,step,now,pre;
     while((step=bfs())!=-1){          //找不到增路径时退出
         max_flow+=step;
         now=end;
         while(now!=start){
             pre=path[now];
             map[pre][now]-=step;      //更新正向边的实际容量
             map[now][pre]+=step;      //添加反向边
             now=pre;
         }
     }
     return max_flow;
 }
 int main(){
     int i,u,v,cost;
     while(scanf("%d %d",&n,&m)!=EOF){
         memset(map,0,sizeof(map));
         for(i=0;i<n;i++){
             scanf("%d %d %d",&u,&v,&cost);
             map[u][v]+=cost;           //not just only one input
         }
         start=1,end=m;
         printf("%d\n",Edmonds_Karp());
     }
     return 0;
 }
 