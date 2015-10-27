/*关键路径
 *AOE网（activity one dge network）
 *把没有入边的点称为源点，每个没有出边的称为终点（汇点）
 *
 *AOV VS AOE 
 *
 *etv earliest  time  of vertex 事件最早发生时间，就是顶点的最早发生时间
 *ltv latest time of vertex 事件最晚发生时间，就是顶点的最晚发生时间，超过时间影响工期
 *ete arliest  time  of edge 活动最早发生时间，就是弧的最早发生时间
 *lte latest time of edge 活动最晚发生时间，就是弧的最晚发生时间，超过时间影响工期
 *
 *
 *etv从源点开始推
 *ltv需要从汇点推 计算到达顶点时间
 *ete从源点开始推 计算活动的发生时间
 *ete从汇点开始推 计算活动的发生时间
 */
 
 
 int *etv ,*ltv;
 int *stack2;		/*用于存储拓扑排序列的栈*/
 int top2;			/*用于stack2的栈顶指针*/
 
/*边表节点声明*/
typedef struct EdgeNode
{
	int adjvex;
	struct EdgeNode *next;
}EdgeNode;

/*顶点节点声明*/
typedef struct VertexNode
{
	int in ; /*入度*/
	int data;
	EdgeNode *firstedge; /*出度*/
}VertexNode,AdjList[HAXVEX];

typedef struct
 {
	AdjList adjList;
	int numVertexes, numEdges;
 }graphAdjList,*GraphAdjList;
 


/*
 *拓扑排序算法
 *若GL无回路，则输出拓扑排序序列并输出OK 退出，否者返回error
*/

Status ToplogicalSort(GraphAdjList GL)
{
	EdgeNode *e;
	int i,k,gettop;
	int top =0; /*用于栈指针下表索引*/
	int count =0;/*用于统计输出顶点个数*/
	int *stack;/*用于存储入度为0的顶点*/
	
	stack=(int *)malloc(GL->numVertexes*sizeof(int));
	
	for(i=0;i<GL->numVertexes;i++)
	{
		if(0==GL->in)
			stack[++top]=i;/*将度为0的下标入栈*/
	}
	
	/*初始化etv都为0*/
	top2=0;
	etv=(int*)malloc(GL->numVertexes*sizeof(int));
	memset(etv,'0',GL->numVertexes*sizeof(int));
	
	stack2=(int*)malloc(GL->numVertexes*sizeof(int));
	
	while(0!=top)
	{
		gettop=stack[top--];
		
		//printf("%d -> ",GL->adjList[gettop].data);
		stack2[++top2]=gettop; /*保存拓扑排序序列 c1,*/
		count++;
		
		for(e=GL->adjList[gettop].firstedge;e;e=e->next)
		{
			k=e->adjvex;
			if(!(--GL->adjList[k].in)) /*判断入度是否为0.为点加入栈中*/
				stack[++top]=k;
			
			if((etv[gettop]+e->weight)>etv[k])
				etv[k]=etv[gettop]+e->weight;
		}
		
	}
	
	if(count<GL->numVertexes) /*如果输出顶点数小于总点数*/
		return ERROR;
	return OK;
}

void CriticalPath(GraphAdjList GL)
{
	EdgeNode *e;
	int i,k,gettop,j;
	int ete,lte;
	
	/*调用改进的拓扑排序，求出etv和stack2*/
	ToplogicalSort(GL);
	
	/*初始化ltv都为汇点的时间*/
	ltv=(int*)malloc(GL->numVertexes*sizeof(int));
	memset(ltv,etv(GL->numVertexes-1),GL->numVertexes*sizeof(int));
	
	/*从汇点计算ltv时间*/
	while(0!=top2)
	{
		gettop=stack2[top2--];/*第一个出栈是汇点*/
		
		for(e=GL->adjList[gettop].firstedge;e;e=e->next)
		{
			k=e->adjvex;			
			if((ltv[k]-e->weight)<ltv[gettop])
				ltv[gettop]=ltv[k]-e->weight;
		}		
	}
	
	/*用ltv和etv，求ete和lte*/
	for(j=0;j<GL->numVertexes;j++)
	{
		for(e=GL->adjList[j].firstedge;e;e=e->next)
		{
			k=e->adjvex;
			ete=etv[j];
			lte=ltv[k]-e->weight;
			
			if(ete==lte)
				printf("<V%d,V%d> length: %d, ",GL->adjList[j].data,GL->adjList[k].data,e->weight);
		}
	}
}