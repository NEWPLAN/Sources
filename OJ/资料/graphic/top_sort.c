/*
 *算法时间复杂度：
 *对于一个具有n个节点，e条变的网来说，初始建立入度为0的顶点栈，要检查所有顶点
 *执行时间为O(n)
 *排序中，若AOV网无回路，则每个顶点入，出栈个一次，每个表节点检查1次，
 *因此记执行时间为O(n+e)
 *所以整个算法复杂度为O(n+e)
 *使用邻接表
*/




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


/*
 *拓扑排序算法
 *若GL无回路，则输出拓扑排序序列并输出OK 退出，否者返回error
*/

Status ToplogicalSort(GraphicList GL)
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
	
	while(0!=top)
	{
		gettop=stack[top--];
		
		printf("%d -> ",GL->adjList[gettop].data);
		count++;
		
		for(e=GL->adjList[gettop].firstedge;e;e=e->next)
		{
			k=e->adjvex;
			if(!(--GL->adjList[k].in)) /*判断入度是否为0.为点加入栈中*/
				stack[++top]=k;
			
		}
	}
	
	if(count<GL->numVertexes) /*如果输出顶点数小于总点数*/
		return ERROR;
	return OK;
}