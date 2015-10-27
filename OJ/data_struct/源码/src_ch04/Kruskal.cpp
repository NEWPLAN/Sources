#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************
* 算法4.x  Kruskal算法
******************************************************************************/
#define MAX (int)(((unsigned)(~((int)0)))>>1)

// 边（弧）结构定义
typedef struct anode{
    int vhead, vtail;      // 边依附的两顶点
    int wght;              // 边（弧）的权重
    int flag;              // 0：未加入MST; 1：已入选; -1：删除
}TArcNode, *TArcPtr;

// 顶点结构定义
typedef struct {
    int vertex;            // 顶点编号信息
    int gno;               // 顶点所在的连通分量编号
}TVexNode, *TVexPtr;

// 图结构定义
typedef struct{
    TVexPtr VexList;
    TArcPtr ArcList;
    int vexnum, arcnum;
}TGraph, *TGPtr;

int vexcnt(int Edges[][3], int num);
TGPtr gcreate(int Edges[][3], int nedge);
void destroy_list(TArcPtr phead);
void destroy_graph(TGPtr pgraph);

// 根据边的三元组信息（Edges数组）创建图（邻接表）
// 三元组格式：结点a，结点b，权重w
// 参数nedge为三元组的个数（Edges数组长度）
TGPtr gcreate(int Edges[][3], int nedges)
{
    int i, nvex;
    TVexPtr vptr;
    TArcPtr aptr;

    // 为图结构申请空间
    TGPtr pgraph = (TGPtr) malloc(sizeof(TGraph));
    if ( !pgraph ){
        printf("Error: 为图分配内存空间失败");
        exit(0);
    }

    // 统计该图中的顶点数
    nvex = vexcnt(Edges, nedges);

    // 为图（TGraph结构体）中的顶点数组分配内存
    vptr = (TVexPtr)malloc(sizeof(TVexNode)*(nvex+1));
    if ( !vptr ){
        printf("Error: 为图的顶点数组分配内存空间失败");
        exit(0);
    }
    for(i = 1; i <= nvex; i++){ // vptr[0]留置不用
        vptr[i].vertex = i;
        vptr[i].gno = i;
    }

    // 为图（TGraph结构体）中的边数组分配内存
    aptr = (TArcPtr)malloc(sizeof(TArcNode)*nedges);
    if ( !aptr ){
        printf("Error: 为图的边数组分配内存空间失败");
        exit(0);
    }
    // 读取三元组，对边进行初始化
    for ( i = 0; i < nedges; i++ ) {
        aptr[i].vhead = Edges[i][0]; // 边线的起点
        aptr[i].vtail = Edges[i][1]; // 边线的终点
        aptr[i].wght = Edges[i][2];  // 边的权重
        aptr[i].flag = 0;
    }

    pgraph->VexList = vptr;
    pgraph->ArcList = aptr;
    pgraph->vexnum = nvex;
    pgraph->arcnum = nedges;

    return pgraph;
}

void destroy_graph(TGPtr pgraph)
{
    free(pgraph->ArcList);
    free(pgraph->VexList);
    free(pgraph);
}

// 统计三元组表Edges中的不同顶点数
int vexcnt(int Edges[][3], int num)
{
    int i, m, n, nvex = 0;
    int *TMPA = (int*)malloc(sizeof(int) * num);
    for ( i = 0; i < num; i++ ) {
        TMPA[i] = 0;
    }
    for ( i = 0; i < num; i++ ) {
        m = Edges[i][0];
        n = Edges[i][1];
        if( TMPA[m] < 1){
            TMPA[m]++;
        }
        if( TMPA[n] < 1){
            TMPA[n]++;
        }
    }
    for ( i = 0; i < num; i++ ) {
        nvex += TMPA[i];
    }
    free(TMPA);
    return nvex;
}

void mst_kruskal(TGPtr pgraph)
{
    int nvex, nedges;  // nvex为顶点数，nedges为边数
    int i, min, idx, count;
    TVexPtr vptr;
    TArcPtr aptr;

    nvex = pgraph->vexnum;
    nedges = pgraph->arcnum;
    vptr = pgraph->VexList;
    aptr = pgraph->ArcList;

    count = 1;
    while(count < nvex){
        min = MAX;
        // 选取未访问过的权最小的边
        for(i = 0; i < nedges; i++)
        {
            if( aptr[i].wght < min && aptr[i].flag == 0){
                min = aptr[i].wght;
                idx = i;
            }
        }
        //若选中的边连接的2个顶点属于不同的连通分量
        if( vptr[aptr[idx].vhead].gno != vptr[aptr[idx].vtail].gno){
            aptr[idx].flag = 1;
            // 将弧的尾和头顶点的gno改为一致
            for(i = 1;i <= nvex; i++){
                if(vptr[i].gno == vptr[aptr[idx].vtail].gno)
                    vptr[i].gno = vptr[aptr[idx].vhead].gno;
            }
            vptr[aptr[idx].vtail].gno = vptr[aptr[idx].vhead].gno;
            count++;
        }
        else{
            aptr[idx].flag = -1;
        }
    }
}

void output_mst(TGPtr pgraph)
{
    int i, nedges;
    nedges = pgraph->arcnum;
    for( i = 0; i < nedges; i++)
       if(pgraph->ArcList[i].flag == 1)
           printf("%d -> %d : %d\n", pgraph->ArcList[i].vhead,\
           pgraph->ArcList[i].vtail, pgraph->ArcList[i].wght );
}

int main(void)
{
    TGPtr pgraph;
    int nedge;
    int Edges[][3] = {
        {1, 2, 6}, {2, 1, 6}, {1, 3, 1}, {3, 1, 1},
        {1, 4, 5}, {4, 1, 5}, {2, 5, 3}, {5, 2, 3},
        {2, 3, 5}, {3, 2, 5}, {3, 4, 5}, {4, 3, 5},
        {3, 5, 6}, {5, 3, 6}, {3, 6, 4}, {6, 3, 4},
        {4, 6, 2}, {6, 4, 2}, {5, 6, 6}, {6, 5, 6}
    };

    nedge = sizeof(Edges)/sizeof(Edges[0]);
    //printf("%d\n", vexcnt(Edges, nedge));

    pgraph = gcreate(Edges, nedge);

    mst_kruskal(pgraph);
    output_mst(pgraph);

    destroy_graph(pgraph);
    system("pause");
    return 0;
}
