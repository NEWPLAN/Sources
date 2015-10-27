#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************
* �㷨4.8  Dijkstra�㷨
******************************************************************************/
#define MAX (int)(((unsigned)(~((int)0)))>>1)
#define NUM_VEX 6

// ��ͷ��㣺
typedef  struct {
    int vex[NUM_VEX];            // ��������
    int arc[NUM_VEX][NUM_VEX];   // �ڽӾ���
}TGraph;


// �Ͻ�˹������Dijkstra���㷨: ��������GN��v0���㵽���ඥ��v�����·��
void Dijkstra(TGraph GN, int v0, int path[], int dist[])
{
    int i, w, vi, vj;
    int S[NUM_VEX];

    // s������ҵ����·���Ķ��㣬��ֵֻ����v0����s[0] = 1
    // dist��¼Դ�㵽���������㵱ǰ����̾��룬���ֵΪ��
    // dist[i] = GN.arcs[0][i] i=1��2������n-1
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
    // ���Ƚ�v0���뵽����S��
    dist[v0] = 0; // v0�ġ�ǰ����Ϊ��
    S[v0] = 1;    // ��ʼʱԴ��v0����S����

    // ѭ����v0��ĳ������v�����·��������v����S��
    // �㷨ִ��ʱ��S����Ķ��㼯�ϣ�V-S����ѡ��һ������w��ʹdist[w]��ֵ��С
    // Ȼ��w���뼯��S�У���s[w]=1
    // ͬʱ�������ϣ�V-S���еĸ�������ľ���ֵ��
    //    ��ԭ����dist[j]��dist[w]+cost[w][j]��ѡ���С��ֵ��Ϊ�µ�dist[j]
    // �ظ��������̣� ֱ��S�а���ͼ�����ж��㣬����Ҳû�пɼ��뵽S�еĶ���Ϊֹ
    for( i = 1; i < (NUM_VEX - 1); i++) {
        int min = MAX;  vi = -1;
        // �ҳ���С��dist[w]
        for( w = 0; w < NUM_VEX; w++){
            // ������w������S����w��v0����
            if(!S[w] && dist[w] < min) {
                vi = w;
                min = dist[w];
            }
        }
        if(vi == -1){
            // ���޶���ɼ���S�У��˳�forѭ��
            break;
        }
        // ������vi���뼯��S
        S[vi] = 1;

        // ��������V-S�еĸ�������ľ���ֵ
        for(vj =0; vj < NUM_VEX; vj++){
            // ���µ�ǰ���·��������
            if(!S[vj] && ((min + GN.arc[vi][vj]) < dist[vj])){
                /*dist[j]��v0������j��ǰ�����·��*/
                // v0��v������j�ľ���= min + Gn.arcs[v][j];
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

    //// �������
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
