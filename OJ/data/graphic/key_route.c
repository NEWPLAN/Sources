/*�ؼ�·��
 *AOE����activity one dge network��
 *��û����ߵĵ��ΪԴ�㣬ÿ��û�г��ߵĳ�Ϊ�յ㣨��㣩
 *
 *AOV VS AOE 
 *
 *etv earliest  time  of vertex �¼����緢��ʱ�䣬���Ƕ�������緢��ʱ��
 *ltv latest time of vertex �¼�������ʱ�䣬���Ƕ����������ʱ�䣬����ʱ��Ӱ�칤��
 *ete arliest  time  of edge ����緢��ʱ�䣬���ǻ������緢��ʱ��
 *lte latest time of edge �������ʱ�䣬���ǻ���������ʱ�䣬����ʱ��Ӱ�칤��
 *
 *
 *etv��Դ�㿪ʼ��
 *ltv��Ҫ�ӻ���� ���㵽�ﶥ��ʱ��
 *ete��Դ�㿪ʼ�� �����ķ���ʱ��
 *ete�ӻ�㿪ʼ�� �����ķ���ʱ��
 */
 
 
 int *etv ,*ltv;
 int *stack2;		/*���ڴ洢���������е�ջ*/
 int top2;			/*����stack2��ջ��ָ��*/
 
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

typedef struct
 {
	AdjList adjList;
	int numVertexes, numEdges;
 }graphAdjList,*GraphAdjList;
 


/*
 *���������㷨
 *��GL�޻�·������������������в����OK �˳������߷���error
*/

Status ToplogicalSort(GraphAdjList GL)
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
	
	/*��ʼ��etv��Ϊ0*/
	top2=0;
	etv=(int*)malloc(GL->numVertexes*sizeof(int));
	memset(etv,'0',GL->numVertexes*sizeof(int));
	
	stack2=(int*)malloc(GL->numVertexes*sizeof(int));
	
	while(0!=top)
	{
		gettop=stack[top--];
		
		//printf("%d -> ",GL->adjList[gettop].data);
		stack2[++top2]=gettop; /*���������������� c1,*/
		count++;
		
		for(e=GL->adjList[gettop].firstedge;e;e=e->next)
		{
			k=e->adjvex;
			if(!(--GL->adjList[k].in)) /*�ж�����Ƿ�Ϊ0.Ϊ�����ջ��*/
				stack[++top]=k;
			
			if((etv[gettop]+e->weight)>etv[k])
				etv[k]=etv[gettop]+e->weight;
		}
		
	}
	
	if(count<GL->numVertexes) /*������������С���ܵ���*/
		return ERROR;
	return OK;
}

void CriticalPath(GraphAdjList GL)
{
	EdgeNode *e;
	int i,k,gettop,j;
	int ete,lte;
	
	/*���øĽ��������������etv��stack2*/
	ToplogicalSort(GL);
	
	/*��ʼ��ltv��Ϊ����ʱ��*/
	ltv=(int*)malloc(GL->numVertexes*sizeof(int));
	memset(ltv,etv(GL->numVertexes-1),GL->numVertexes*sizeof(int));
	
	/*�ӻ�����ltvʱ��*/
	while(0!=top2)
	{
		gettop=stack2[top2--];/*��һ����ջ�ǻ��*/
		
		for(e=GL->adjList[gettop].firstedge;e;e=e->next)
		{
			k=e->adjvex;			
			if((ltv[k]-e->weight)<ltv[gettop])
				ltv[gettop]=ltv[k]-e->weight;
		}		
	}
	
	/*��ltv��etv����ete��lte*/
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