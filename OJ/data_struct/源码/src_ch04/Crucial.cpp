#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************
* �㷨4.7  �ؼ�·���㷨
******************************************************************************/
#define MAX (int)(((unsigned)(~((int)0)))>>1)

// �ڽӱ��㣺
typedef struct node{
	int vex;          // ������
	int wght;
	struct  node  *next; //����
}TNode, *TNodePtr;

// ��ͷ��㣺
typedef  struct {
	int vex;
	int indgr;      // �����
	TNodePtr link;  // ����
}Thead, *TGPtr;

int vexcnt(int Edges[][3], int num);
void destroy_graph(TGPtr pgraph, int nvex);


// ���ݱߵ���Ԫ����Ϣ��Edges���飩����ͼ���ڽӱ�
// ��Ԫ���ʽ�����a�����b��Ȩ��w
// ����nedgeΪ��Ԫ��ĸ�����Edges���鳤�ȣ�
TGPtr create_linklist(int Edges[][3], int nvex, int nedges)
{
	int i, vhead;
	TGPtr vptr;
	TNodePtr aptr;

	// Ϊͼ�Ķ������飨Ԫ��ΪThead�ṹ�壩�����ڴ�
	vptr = (TGPtr)malloc(sizeof(Thead)*(nvex+1));
	if ( !vptr ){
		printf("Error: Ϊͼ�Ķ�����������ڴ�ռ�ʧ��");
		exit(0);
	}
	for(i = 1; i <= nvex; i++){ // vptr[0]���ò���
		vptr[i].vex = i;
		vptr[i].indgr = 0;
		vptr[i].link = NULL;
	}

	for(i = 0; i < nedges; i++){
		aptr = (TNodePtr)malloc(sizeof(TNode));
		if ( !aptr ){
			printf("Error: Ϊͼ�ı���������ڴ�ռ�ʧ��");
			exit(0);
		}
		vhead = Edges[i][0];       // ���ߵ����
		aptr->vex = Edges[i][1];   // ���ߵ��յ�
		aptr->wght = Edges[i][2];  // �ߵ�Ȩ��
		aptr->next = vptr[vhead].link;
		vptr[vhead].link = aptr;
	}
	return vptr;
}

// �������ж�������
void count_indegree(Thead graph[], int nvex)
{
	int i, idx;
	TNodePtr aptr;
	for(i = 1; i <= nvex; i++){
		aptr = graph[i].link;
		while(aptr){
			idx = aptr->vex;
			graph[idx].indgr++;
			aptr = aptr->next;
		}
	}
}

// ���ر�(vtail, vhead)��Ӧ��Ȩֵ��������ʱ�䣩
int get_weight(Thead graph[], int vtail, int vhead)
{
	TNode *ptr;
	ptr = graph[vtail].link;
	while(ptr)
	{
		if( ptr->vex == vhead ){
			return( ptr->wght );
		}
		else{
			ptr = ptr->next;
		}
	}
	return(MAX);
}

// ��ͼgraph�����������򣬲��������������緢��ʱ�䣺
// ����Ev�������¼�������v�������緢��ʱ��
// ����topo_seq����������������
// ����ֵ�����������ж������
int sort_topology(Thead graph[], int nvex, int Ev[], int topo_seq[])
{
	int stack[20]; // �������Ϊ0�Ķ����ջ
	int m = 0;     // ��¼����Ķ������
	int top = 0;   // ��¼ջ��λ��
	int i, id;
	TNode *ptr;

	// ��ʼ���¼�������v�������緢��ʱ��
	for( i = 1; i <= nvex; i++){
		Ev[i] = 0;
	}

	// ���Ϊ0�Ķ�����ջ
	for( i = 1; i <= nvex; i++){
		if(graph[i].indgr == 0){
			stack[top++] = i;
		}
	}
	while( top > 0 ){
		id = stack[--top];
		topo_seq[m] = id;
		m++;
		ptr = graph[id].link;
		// �����붥��id�����Ķ������ȼ�һ
		while(ptr){
			i = ptr->vex;
			graph[i].indgr--;
			if(graph[i].indgr == 0){
				stack[top++] = i;
			}
			// ����������˳����Ev
			if( Ev[id] + get_weight(graph, id, i) > Ev[i] )
				Ev[i] = Ev[id] + get_weight(graph, id, i);
			ptr = ptr->next;
		}
	}
	if( m < nvex ){
		return 0;
	}
	else{
		return m;
	}
}

void critical_path( Thead graph[], int nvex )
{
	int i, j, k, ee, ll;
	int *Ev;        //�����¼�������v�������緢��ʱ��
	int *Lv;        //�����¼�������v������ٷ���ʱ��
	int *topo_seq;  // ��������������
	int numv;       // ���������ж������
	char tag;
	TNode *ptr;

	Ev = (int *)malloc(sizeof(int)*(nvex+1));
	if ( !Ev ){
		printf("Error: ΪEv��������ڴ�ռ�ʧ��");
		exit(0);
	}
	Lv = (int *)malloc(sizeof(int)*(nvex+1));
	if ( !Lv ){
		printf("Error: ΪLv��������ڴ�ռ�ʧ��");
		exit(0);
	}
	topo_seq = (int *)malloc(sizeof(int)*(nvex+1));
	if ( !topo_seq ){
		printf("Error: Ϊtopo_seq��������ڴ�ռ�ʧ��");
		exit(0);
	}

	numv = sort_topology(graph, nvex, Ev, topo_seq);

	// ��Lv���¼�������v������ٷ���ʱ�䣩
	if(!numv){
		printf("ͼ���л�!");
	}
	else{
		for(i = 1; i <= nvex; i++){
			Lv[i] = MAX;
		}
		// �����������ٷ���ʱ�����
		Lv[nvex] = Ev[nvex];

		// ������������������������������ٷ���ʱ�䣨Lv��
		for(i = numv - 1; i >= 0; i--){
			j = topo_seq[i];  // ��������˳��Ӻ���ǰ��һȡ��ͷԪ��
			ptr = graph[j].link;
			while(ptr){
				k = ptr->vex; // k�ǡ���jΪ��ͷ���ĺ�β������ͼ�еı��
				if(Lv[k] - get_weight(graph, j, k) < Lv[j]){
					Lv[j] = Lv[k] - get_weight(graph, j, k);
				}
				ptr = ptr->next;
			}
			printf("Lv[%d] = %d\n",j, Lv[j]);
		}

		//��ؼ��
		for(j = 1; j <= nvex; j++){
			ptr = graph[j].link;
			while(ptr){
				k = ptr->vex;
				 // �Ի(j,k)���ԣ����翪ʼʱ��(ee)���ڵ��ڸû�Ļ�β�¼���j�������緢��ʱ�䣨Ev[j]��
				ee = Ev[j];
				// �Ի(j,k)���ԣ���ٿ�ʼʱ��(ll)���ڸû�Ļ�ͷ�¼�����ٷ���ʱ����û�ĳ���ʱ��֮��
				ll = Lv[k] - get_weight(graph, j, k);

				// ����(j,k)�ġ����翪ʼʱ�䡱��ee�����ڡ���ٿ�ʼʱ�䡱��ll��
				// ���ǸûΪ���ؼ��������*���б�ʶ
				if(ee == ll){
					tag='*';
				}
				else{
					tag=' ';
				}
				printf("(%c) <%d,%d>  Len = %d  ee = %d  ll = %d\n", tag, j, k, get_weight(graph, j, k), ee, ll);
				ptr = ptr->next;
			}
		}
	}
	free(Lv);
	free(Ev);
	free(topo_seq);
	return;
}

void destroy_graph(TGPtr pgraph, int nvex)
{
	int i;
	for(i = 1; i <= nvex; i++){
		free(pgraph[i].link);
	}
	free(pgraph);
}

int main(void)
{
	TGPtr graph;
	int nvex, nedges;
	int Edges[][3] = {
		{1, 2, 3}, {1, 3, 2}, {2, 4, 2}, {2, 5, 3}, {3, 4, 4}, {3, 6, 3}, {4, 6, 2}, {5, 6, 1}
	};

	// ��ͼ�ı���
	nedges = sizeof(Edges)/sizeof(Edges[0]);

	// ͳ�Ƹ�ͼ�еĶ�����
	nvex = vexcnt(Edges, nedges);

	// ������Ԫ���Edges������ͼ���ڽӱ�
	graph = create_linklist(Edges, nvex, nedges);

	// ���㶥����ȣ���䵽�ڽӱ���
	count_indegree(graph, nvex);

	// ����ؼ�·��
	critical_path(graph, nvex);

	// ɾ��Ϊ�洢ͼ���ڽӱ�������Ŀռ�
	destroy_graph(graph, nvex);
	system("pause");
	return 0;
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
