#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************
* �㷨4.x  Kruskal�㷨
******************************************************************************/
#define MAX (int)(((unsigned)(~((int)0)))>>1)

// �ߣ������ṹ����
typedef struct anode{
    int vhead, vtail;      // ��������������
    int wght;              // �ߣ�������Ȩ��
    int flag;              // 0��δ����MST; 1������ѡ; -1��ɾ��
}TArcNode, *TArcPtr;

// ����ṹ����
typedef struct {
    int vertex;            // ��������Ϣ
    int gno;               // �������ڵ���ͨ�������
}TVexNode, *TVexPtr;

// ͼ�ṹ����
typedef struct{
    TVexPtr VexList;
    TArcPtr ArcList;
    int vexnum, arcnum;
}TGraph, *TGPtr;

int vexcnt(int Edges[][3], int num);
TGPtr gcreate(int Edges[][3], int nedge);
void destroy_list(TArcPtr phead);
void destroy_graph(TGPtr pgraph);

// ���ݱߵ���Ԫ����Ϣ��Edges���飩����ͼ���ڽӱ�
// ��Ԫ���ʽ�����a�����b��Ȩ��w
// ����nedgeΪ��Ԫ��ĸ�����Edges���鳤�ȣ�
TGPtr gcreate(int Edges[][3], int nedges)
{
    int i, nvex;
    TVexPtr vptr;
    TArcPtr aptr;

    // Ϊͼ�ṹ����ռ�
    TGPtr pgraph = (TGPtr) malloc(sizeof(TGraph));
    if ( !pgraph ){
        printf("Error: Ϊͼ�����ڴ�ռ�ʧ��");
        exit(0);
    }

    // ͳ�Ƹ�ͼ�еĶ�����
    nvex = vexcnt(Edges, nedges);

    // Ϊͼ��TGraph�ṹ�壩�еĶ�����������ڴ�
    vptr = (TVexPtr)malloc(sizeof(TVexNode)*(nvex+1));
    if ( !vptr ){
        printf("Error: Ϊͼ�Ķ�����������ڴ�ռ�ʧ��");
        exit(0);
    }
    for(i = 1; i <= nvex; i++){ // vptr[0]���ò���
        vptr[i].vertex = i;
        vptr[i].gno = i;
    }

    // Ϊͼ��TGraph�ṹ�壩�еı���������ڴ�
    aptr = (TArcPtr)malloc(sizeof(TArcNode)*nedges);
    if ( !aptr ){
        printf("Error: Ϊͼ�ı���������ڴ�ռ�ʧ��");
        exit(0);
    }
    // ��ȡ��Ԫ�飬�Ա߽��г�ʼ��
    for ( i = 0; i < nedges; i++ ) {
        aptr[i].vhead = Edges[i][0]; // ���ߵ����
        aptr[i].vtail = Edges[i][1]; // ���ߵ��յ�
        aptr[i].wght = Edges[i][2];  // �ߵ�Ȩ��
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

// ͳ����Ԫ���Edges�еĲ�ͬ������
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
    int nvex, nedges;  // nvexΪ��������nedgesΪ����
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
        // ѡȡδ���ʹ���Ȩ��С�ı�
        for(i = 0; i < nedges; i++)
        {
            if( aptr[i].wght < min && aptr[i].flag == 0){
                min = aptr[i].wght;
                idx = i;
            }
        }
        //��ѡ�еı����ӵ�2���������ڲ�ͬ����ͨ����
        if( vptr[aptr[idx].vhead].gno != vptr[aptr[idx].vtail].gno){
            aptr[idx].flag = 1;
            // ������β��ͷ�����gno��Ϊһ��
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
