#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************
* 算法4.9  Floyd算法
******************************************************************************/
#define MAXUINT 1000 // 最大值
#define NUM_VEX 4  // 最大顶点个数

typedef struct {
    int Arcs[NUM_VEX][NUM_VEX];    // 邻接矩阵
    int nvex; // 顶点数量
    int narc;    // 边的数量

}TGraph;

void Floyd( int Dist[][NUM_VEX], int Path[][NUM_VEX], int nvex )
{
    int i, j, k;

    // 初始化Path数组，令：Path(A,B) = A
    for ( i = 0; i < nvex; ++i ){
        for ( j = 0; j < nvex; ++j ){
            Path[i][j] = i;
            //Dist[i][j] = MAXUINT;
        }
    }
    // 执行Floyd算法
    for ( k = 0; k < nvex; ++k ){
        for ( i = 0; i < nvex; ++i ){
            for ( j = 0; j < nvex; ++j ){
                // 如果存在比当前的（i ，j）间的路径更短的路径
                if ( Dist[i][k] + Dist[k][j] < Dist[i][j] ){
                    // 将中间结点k加入到（i, j）的路径中
                    Dist[i][j] = Dist[i][k] + Dist[k][j];
                    // 修改结点j的路径前驱为k
                    Path[i][j] = Path[k][j];
                }
            }
        }
    }
}

// 输出Floyd算法结果：图中所有节点对之间的最短路径长度和路径经过的结点编号
void output( int Dist[][NUM_VEX], int Path[][NUM_VEX], int nvex )
{
    int i, j, k, top;
    int stackVertices[NUM_VEX];
    printf("Origin -> Dest  Distance  Path\n");
    for ( i = 0; i < nvex; ++i ){
        for (j = 0; j < nvex; ++j ){
            if ( i != j ){
                printf("%7d -> %-4d", i+1, j+1);
                // i -> j 不存在路径
                if ( Dist[i][j] == MAXUINT ) {
                    printf("      Unreachable!\n", i+1, j+1);
                }
                else{
                    // 输出路径（i,j）的长度
                    printf("%6d", Dist[i][j]);
                    // 查询最短路径是从后往前查，因此使用栈存储中间查询结果
                    k = j; top = 0;
                    do{
                        stackVertices[top++] = k;
                        k = Path[i][k];
                    } while ( k != i );
                    // 输出路径（i,j）的起始点
                    printf("%6d", i+1);
                    top--;  // 令top指向栈顶元素的实际存储位置
                    while(top >= 0){
                        printf(" -> %d", stackVertices[top--]+1);
                    }
                    printf("\n");
                }
            }
        }
    }
}


int main( void )
{
    int i, k;
    TGraph graph;
    int arrDist[NUM_VEX][NUM_VEX];
    int arrPath[NUM_VEX][NUM_VEX];

    graph.narc = 8;
    graph.nvex = NUM_VEX;

    for(i =0 ; i < NUM_VEX; i++){
        for(k = 0; k < NUM_VEX; k++){
            graph.Arcs[i][k] = MAXUINT;
        }
        graph.Arcs[i][i] = 0;
    }

    graph.Arcs[0][1] = 5;
    graph.Arcs[1][0] = 50;
    graph.Arcs[1][2] = 15;
    graph.Arcs[1][3] = 5;

    graph.Arcs[2][0] = 30;
    graph.Arcs[2][3] = 15;
    graph.Arcs[3][2] = 5;
    graph.Arcs[3][0] = 15;

    // 初始化arrDist

    for ( int i = 0; i < graph.nvex; ++i ){
        for ( int j = 0; j < graph.nvex; ++j ){
            arrDist[i][j] = graph.Arcs[i][j];
        }
    }
    // 调用Floyd函数（填充arrDist和arrPath矩阵）
    Floyd( arrDist, arrPath, graph.nvex );
    // 输出Floyd算法结果
    output( arrDist, arrPath, graph.nvex );

    system( "pause" );
    return 0;
}
