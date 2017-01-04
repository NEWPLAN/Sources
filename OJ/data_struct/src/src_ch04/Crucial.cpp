#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************
* 算法4.7  关键路径算法
******************************************************************************/
#define MAX (int)(((unsigned)(~((int)0)))>>1)

// 邻接表结点：
typedef struct node{
	int vex;          // 顶点域
	int wght;
	struct  node  *next; //链域
}TNode, *TNodePtr;

// 表头结点：
typedef  struct {
	int vex;
	int indgr;      // 入度域
	TNodePtr link;  // 链域
}Thead, *TGPtr;

int vexcnt(int Edges[][3], int num);
void destroy_graph(TGPtr pgraph, int nvex);


// 根据边的三元组信息（Edges数组）创建图（邻接表）
// 三元组格式：结点a，结点b，权重w
// 参数nedge为三元组的个数（Edges数组长度）
TGPtr create_linklist(int Edges[][3], int nvex, int nedges)
{
	int i, vhead;
	TGPtr vptr;
	TNodePtr aptr;

	// 为图的顶点数组（元素为Thead结构体）分配内存
	vptr = (TGPtr)malloc(sizeof(Thead)*(nvex+1));
	if ( !vptr ){
		printf("Error: 为图的顶点数组分配内存空间失败");
		exit(0);
	}
	for(i = 1; i <= nvex; i++){ // vptr[0]留置不用
		vptr[i].vex = i;
		vptr[i].indgr = 0;
		vptr[i].link = NULL;
	}

	for(i = 0; i < nedges; i++){
		aptr = (TNodePtr)malloc(sizeof(TNode));
		if ( !aptr ){
			printf("Error: 为图的边数组分配内存空间失败");
			exit(0);
		}
		vhead = Edges[i][0];       // 边线的起点
		aptr->vex = Edges[i][1];   // 边线的终点
		aptr->wght = Edges[i][2];  // 边的权重
		aptr->next = vptr[vhead].link;
		vptr[vhead].link = aptr;
	}
	return vptr;
}

// 计算所有顶点的入度
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

// 返回边(vtail, vhead)对应的权值（即持续时间）
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

// 对图graph进行拓扑排序，并求出各顶点的最早发生时间：
// 参数Ev：保存事件（顶点v）的最早发生时间
// 参数topo_seq：保存拓扑排序结果
// 返回值：拓扑序列中顶点个数
int sort_topology(Thead graph[], int nvex, int Ev[], int topo_seq[])
{
	int stack[20]; // 保存入度为0的顶点的栈
	int m = 0;     // 记录输出的顶点个数
	int top = 0;   // 记录栈顶位置
	int i, id;
	TNode *ptr;

	// 初始化事件（顶点v）的最早发生时间
	for( i = 1; i <= nvex; i++){
		Ev[i] = 0;
	}

	// 入度为0的顶点入栈
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
		// 所有与顶点id关联的顶点的入度减一
		while(ptr){
			i = ptr->vex;
			graph[i].indgr--;
			if(graph[i].indgr == 0){
				stack[top++] = i;
			}
			// 按拓扑序列顺序求Ev
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
	int *Ev;        //保存事件（顶点v）的最早发生时间
	int *Lv;        //保存事件（顶点v）的最迟发生时间
	int *topo_seq;  // 保存拓扑排序结果
	int numv;       // 拓扑序列中顶点个数
	char tag;
	TNode *ptr;

	Ev = (int *)malloc(sizeof(int)*(nvex+1));
	if ( !Ev ){
		printf("Error: 为Ev数组分配内存空间失败");
		exit(0);
	}
	Lv = (int *)malloc(sizeof(int)*(nvex+1));
	if ( !Lv ){
		printf("Error: 为Lv数组分配内存空间失败");
		exit(0);
	}
	topo_seq = (int *)malloc(sizeof(int)*(nvex+1));
	if ( !topo_seq ){
		printf("Error: 为topo_seq数组分配内存空间失败");
		exit(0);
	}

	numv = sort_topology(graph, nvex, Ev, topo_seq);

	// 求Lv（事件（顶点v）的最迟发生时间）
	if(!numv){
		printf("图中有环!");
	}
	else{
		for(i = 1; i <= nvex; i++){
			Lv[i] = MAX;
		}
		// 汇点的最早和最迟发生时间相等
		Lv[nvex] = Ev[nvex];

		// 根据拓扑排序结果的逆序求各顶点的最迟发生时间（Lv）
		for(i = numv - 1; i >= 0; i--){
			j = topo_seq[i];  // 根据拓扑顺序从后往前逐一取弧头元素
			ptr = graph[j].link;
			while(ptr){
				k = ptr->vex; // k是“以j为弧头”的狐尾顶点在图中的编号
				if(Lv[k] - get_weight(graph, j, k) < Lv[j]){
					Lv[j] = Lv[k] - get_weight(graph, j, k);
				}
				ptr = ptr->next;
			}
			printf("Lv[%d] = %d\n",j, Lv[j]);
		}

		//求关键活动
		for(j = 1; j <= nvex; j++){
			ptr = graph[j].link;
			while(ptr){
				k = ptr->vex;
				 // 对活动(j,k)而言，最早开始时间(ee)等于等于该活动的弧尾事件（j）的最早发生时间（Ev[j]）
				ee = Ev[j];
				// 对活动(j,k)而言，最迟开始时间(ll)等于该活动的弧头事件的最迟发生时间与该活动的持续时间之差
				ll = Lv[k] - get_weight(graph, j, k);

				// 如果活动(j,k)的“最早开始时间”（ee）等于“最迟开始时间”（ll）
				// 则标记该活动为“关键活动”，以*进行标识
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

	// 求图的边数
	nedges = sizeof(Edges)/sizeof(Edges[0]);

	// 统计该图中的顶点数
	nvex = vexcnt(Edges, nedges);

	// 根据三元组表（Edges）创建图的邻接表
	graph = create_linklist(Edges, nvex, nedges);

	// 计算顶点入度，填充到邻接表中
	count_indegree(graph, nvex);

	// 计算关键路径
	critical_path(graph, nvex);

	// 删除为存储图的邻接表所分配的空间
	destroy_graph(graph, nvex);
	system("pause");
	return 0;
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
