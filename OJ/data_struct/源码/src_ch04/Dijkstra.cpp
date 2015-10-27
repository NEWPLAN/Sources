#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************
* 算法4.8  Dijkstra算法
******************************************************************************/
#define MAX (int)(((unsigned)(~((int)0)))>>1)
#define NUM_VEX 6

// 表头结点：
typedef  struct {
    int vex[NUM_VEX];            // 顶点数组
    int arc[NUM_VEX][NUM_VEX];   // 邻接矩阵
}TGraph;


// 迪杰斯特拉（Dijkstra）算法: 求有向网GN的v0顶点到其余顶点v的最短路径
void Dijkstra(TGraph GN, int v0, int path[], int dist[])
{
    int i, w, vi, vj;
    int S[NUM_VEX];

    // s标记已找到最短路径的顶点，初值只包括v0，即s[0] = 1
    // dist记录源点到其他各顶点当前的最短距离，其初值为：
    // dist[i] = GN.arcs[0][i] i=1，2，…，n-1
    for(i = 0; i < NUM_VEX; i++) {
        S[i] = 0;
        dist[i] = GN.arc[v0][i];
        if( dist[i] < MAX ){
            path[i] = v0;
        }
        else{
            path[i] = -1;
        }
    }
    // 首先将v0加入到集合S中
    dist[v0] = 0; // v0的“前驱”为空
    S[v0] = 1;    // 初始时源点v0属于S集合

    // 循环求v0到某个顶点v的最短路径，并将v加入S集
    // 算法执行时从S以外的顶点集合（V-S）中选出一个顶点w，使dist[w]的值最小
    // 然后将w加入集合S中，即s[w]=1
    // 同时调整集合（V-S）中的各个顶点的距离值：
    //    从原来的dist[j]和dist[w]+cost[w][j]中选择较小的值作为新的dist[j]
    // 重复上述过程， 直到S中包含图中所有顶点，或再也没有可加入到S中的顶点为止
    for( i = 1; i < (NUM_VEX - 1); i++) {
        int min = MAX;  vi = -1;
        // 找出最小的dist[w]
        for( w = 0; w < NUM_VEX; w++){
            // 若顶点w不属于S，且w离v0更近
            if(!S[w] && dist[w] < min) {
                vi = w;
                min = dist[w];
            }
        }
        if(vi == -1){
            // 已无顶点可加入S中，退出for循环
            break;
        }
        // 将顶点vi并入集合S
        S[vi] = 1;

        // 调整集合V-S中的各个顶点的距离值
        for(vj =0; vj < NUM_VEX; vj++){
            // 更新当前最短路径及距离
            if(!S[vj] && ((min + GN.arc[vi][vj]) < dist[vj])){
                /*dist[j]是v0到顶点j当前的最短路径*/
                // v0经v到顶点j的距离= min + Gn.arcs[v][j];
                dist[vj] = min + GN.arc[vi][vj];
                path[vj] = vi;
            }
        }
    }
}

void show_path(int path[], int dist[])
{
    int i, pre, tmp;
    for(i = 0; i < NUM_VEX; i++){
        printf("path = %d", i);
        pre = path[i];
        while(pre){
            if(pre == -1) break;
            printf(" <- %d", pre);
            tmp = pre;
            pre = path[tmp];
        }
        printf(" <- 0");
        printf("\ndist = %d\n", dist[i]);
    }
}

int main(void)
{
    TGraph graph;
    int path[NUM_VEX];
    int dist[NUM_VEX];
    int i, k;

    for(i =0 ; i < NUM_VEX; i++){
        graph.vex[NUM_VEX] = i;
    }
    for(i =0 ; i < NUM_VEX; i++){
        for(k = 0; k < NUM_VEX; k++){
            graph.arc[i][k] = MAX;
        }
        graph.arc[i][i] = 0;
    }
    graph.arc[0][2] = 10;
    graph.arc[0][4] = 30;
    graph.arc[0][5] = 100;
    graph.arc[1][2] = 5;
    graph.arc[2][3] = 50;
    graph.arc[3][5] = 10;
    graph.arc[4][3] = 20;
    graph.arc[4][5] = 60;

    //// 测试输出
    //for(i =0 ; i < NUM_VEX; i++){
    //    for(k = 0; k < NUM_VEX; k++){
    //        printf("%5d", graph.arc[i][k]);
    //    }
    //    printf("\n");
    //}

    Dijkstra(graph, 0, path, dist);

    show_path(path, dist);

    system("pause");
    return 0;
}
