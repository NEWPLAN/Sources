/*
 *�㷨ʱ�临�Ӷȣ�
 *����һ������n���ڵ㣬e���������˵����ʼ�������Ϊ0�Ķ���ջ��Ҫ������ж���
 *ִ��ʱ��ΪO(n)
 *�����У���AOV���޻�·����ÿ�������룬��ջ��һ�Σ�ÿ����ڵ���1�Σ�
 *��˼�ִ��ʱ��ΪO(n+e)
 *���������㷨���Ӷ�ΪO(n+e)
 *ʹ���ڽӱ�
*/




/*�߱�ڵ�����*/
typedef struct EdgeNode
{
	int adjvex;
	struct EdgeNode *next;
}EdgeNode;

/*����ڵ�����*/
typedef struct VertexNode
{
	int in ; /*���*/
	int data;
	EdgeNode *firstedge; /*����*/
}VertexNode,AdjList[HAXVEX];


/*
 *���������㷨
 *��GL�޻�·������������������в����OK �˳������߷���error
*/

Status ToplogicalSort(GraphicList GL)
{
	EdgeNode *e;
	int i,k,gettop;
	int top =0; /*����ջָ���±�����*/
	int count =0;/*����ͳ������������*/
	int *stack;/*���ڴ洢���Ϊ0�Ķ���*/
	
	stack=(int *)malloc(GL->numVertexes*sizeof(int));
	
	for(i=0;i<GL->numVertexes;i++)
	{
		if(0==GL->in)
			stack[++top]=i;/*����Ϊ0���±���ջ*/
	}
	
	while(0!=top)
	{
		gettop=stack[top--];
		
		printf("%d -> ",GL->adjList[gettop].data);
		count++;
		
		for(e=GL->adjList[gettop].firstedge;e;e=e->next)
		{
			k=e->adjvex;
			if(!(--GL->adjList[k].in)) /*�ж�����Ƿ�Ϊ0.Ϊ�����ջ��*/
				stack[++top]=k;
			
		}
	}
	
	if(count<GL->numVertexes) /*������������С���ܵ���*/
		return ERROR;
	return OK;
}