#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************
* �㷨4.9  Floyd�㷨
******************************************************************************/
#define MAXUINT 1000 // ���ֵ
#define NUM_VEX 4  // ��󶥵����

typedef struct {
    int Arcs[NUM_VEX][NUM_VEX];    // �ڽӾ���
    int nvex; // ��������
    int narc;    // �ߵ�����

}TGraph;

void Floyd( int Dist[][NUM_VEX], int Path[][NUM_VEX], int nvex )
{
    int i, j, k;

    // ��ʼ��Path���飬�Path(A,B) = A
    for ( i = 0; i < nvex; ++i ){
        for ( j = 0; j < nvex; ++j ){
            Path[i][j] = i;
            //Dist[i][j] = MAXUINT;
        }
    }
    // ִ��Floyd�㷨
    for ( k = 0; k < nvex; ++k ){
        for ( i = 0; i < nvex; ++i ){
            for ( j = 0; j < nvex; ++j ){
                // ������ڱȵ�ǰ�ģ�i ��j�����·�����̵�·��
                if ( Dist[i][k] + Dist[k][j] < Dist[i][j] ){
                    // ���м���k���뵽��i, j����·����
                    Dist[i][j] = Dist[i][k] + Dist[k][j];
                    // �޸Ľ��j��·��ǰ��Ϊk
                    Path[i][j] = Path[k][j];
                }
            }
        }
    }
}

// ���Floyd�㷨�����ͼ�����нڵ��֮������·�����Ⱥ�·�������Ľ����
void output( int Dist[][NUM_VEX], int Path[][NUM_VEX], int nvex )
{
    int i, j, k, top;
    int stackVertices[NUM_VEX];
    printf("Origin -> Dest  Distance  Path\n");
    for ( i = 0; i < nvex; ++i ){
        for (j = 0; j < nvex; ++j ){
            if ( i != j ){
                printf("%7d -> %-4d", i+1, j+1);
                // i -> j ������·��
                if ( Dist[i][j] == MAXUINT ) {
                    printf("      Unreachable!\n", i+1, j+1);
                }
                else{
                    // ���·����i,j���ĳ���
                    printf("%6d", Dist[i][j]);
                    // ��ѯ���·���ǴӺ���ǰ�飬���ʹ��ջ�洢�м��ѯ���
                    k = j; top = 0;
                    do{
                        stackVertices[top++] = k;
                        k = Path[i][k];
                    } while ( k != i );
                    // ���·����i,j������ʼ��
                    printf("%6d", i+1);
                    top--;  // ��topָ��ջ��Ԫ�ص�ʵ�ʴ洢λ��
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

    // ��ʼ��arrDist

    for ( int i = 0; i < graph.nvex; ++i ){
        for ( int j = 0; j < graph.nvex; ++j ){
            arrDist[i][j] = graph.Arcs[i][j];
        }
    }
    // ����Floyd���������arrDist��arrPath����
    Floyd( arrDist, arrPath, graph.nvex );
    // ���Floyd�㷨���
    output( arrDist, arrPath, graph.nvex );

    system( "pause" );
    return 0;
}
