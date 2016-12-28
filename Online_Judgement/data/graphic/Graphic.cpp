/************************************************************************/
/* ͼ�Ļ����㷨������                                                      */
/* ͼ�Ĵ洢                                                              */
/* �ڽӱ�(���ⶥ��֮���б�)�Ϳ���ʹ��һ������������                             */
/* �ڽӾ���(����һ��������֮������Ķ���ļ���)                                */
/* ʮ������ʮ������������������Ⱥͳ��ȣ�                                  */
/* �߼����飨��űߵ�������Ϣ���ڵ����������Ϣ����㡢�յ㡢Ȩ��                  */
/************************************************************************/

#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#define  MAX_INDEX 20
#define  INFINIT	1<<30

using namespace std;



/*************�ڽ׾���ڵ�******************/
typedef struct ArcCell
{
	int adj;	/*�ߵ�Ȩ*/
	char* info;	/*����Ϣ*/
}ArcCell, AdjMatrix[MAX_INDEX][MAX_INDEX];
/*************�ڽ׾���ڵ�******************/


/*******************ʮ������ڵ�*****************************/
typedef struct OriEdge
{
	struct OriEdge * HeadLink,*TailLink;/*ָ�������һ���ߣ�ָ���յ���һ����*/
	int TailVax,HeadVax;/*������±꣬���յ��±�*/
	int weight;/*Ȩ��*/
}OriEdge,*OriEdgePtr;

typedef struct OriNode
{
	struct OriEdge *firstin,*firstout;
	char* vex_info;/*�ڵ���Ϣ*/
}OriNode, *OriList;
/*******************ʮ������ڵ�*****************************/


/*******************�߼�����*****************************/
typedef struct EdgeNode
{
	int TailVax,HeadVax;/*������±꣬���յ��±�*/
	int weight;/*Ȩ��*/
}EdgeNode,*EdgeNodePtr;

typedef struct EdgeArray
{
	EdgeNode arc;/*�ߵ���Ϣ*/
	char* vex_info;/*�ڵ���Ϣ*/
}EdgeArray,*EdgeArrayPtr;
/*******************�߼�����*****************************/


/*************�ڽױ�Ľڵ�******************/
typedef struct Node
{
	struct Node * first;
	int data;	/*������±�����*/
	int adj;/*Ȩ��*/
}Node;
/*************�ڽױ�Ľڵ�******************/

int comparefunc(const void* p1,const void * p2)
{
	return ((EdgeArrayPtr)p1)->arc.weight>((EdgeArrayPtr)p2)->arc.weight;
}

class Graph
{
private:
	bool orientation;
	AdjMatrix arcs;			/*ͼ���ڽӾ���*/
	string vexs[MAX_INDEX];	/*����ı�ʶ*/
	int vexum,arcum;		/*ͼ�Ķ�����Ŀ���ߵ���Ŀ*/
	Node AdjTab[MAX_INDEX];	/*�ڽӱ�*/
	OriNode orithogonal[MAX_INDEX];/*ʮ������*/
	EdgeArray edge[MAX_INDEX];/*�߼�����*/
	bool VisitedVex[MAX_INDEX];/*�Ѿ����ʹ��Ľڵ�*/

	void AdjMatrixInit(void);
	void AdjTabInit(void);
	void OriListInit(void);
	void EdgeArrayInit(void);
	void CreateAdjMatrix(void);
	void CreateOriList(void);
	void CreateEdgeArray(void);
	void CreateAdjTab(void);
	int FindPosition(string ps);

public:/*interface*/	
	Graph(void);
	~Graph(void);
	void Dijkstra();/*�Ͻ�˹�����㷨�����·�� ����Ϊ�����ص㣩*/
	void Prim();/*����ķ�㷨����С����������Ϊ�����ص㣩*/
	void Kruskal();/*��³˹�����㷨����С����������Ϊ�����ص㣩*/
	void Floyd();/*���������㷨�����·���㷨����Ϊ�����ص㣩*/
	void DFS(int start_point=0);/*������ȱ���*/
	void BFS(int start_point=0);/*������ȱ���*/
	void test();/*test for function*/
};
Graph::Graph(void)
{
	this->vexum=0;
	this->arcum=0;
	memset(VisitedVex,0,sizeof(bool)*MAX_INDEX);
	this->AdjMatrixInit();
	this->AdjTabInit();
	this->OriListInit();
	this->EdgeArrayInit();
}

Graph::~Graph()
{
	return;
}

void Graph::AdjMatrixInit(void)
{
	int i,j;
	for (i=0;i<MAX_INDEX;i++)
	{
		for (j=0;j<MAX_INDEX;j++)
		{
			this->arcs[i][j].adj=0;
			this->arcs[i][j].info=NULL;
		}
	}
}
void Graph::AdjTabInit(void)
{
	for (int i=0;i<MAX_INDEX;i++)
	{
		this->AdjTab[i].data=0;
		this->AdjTab[i].first=NULL;
	}
}

void Graph::OriListInit(void)
{
	for (int i=0;i<MAX_INDEX;i++)
	{
		this->orithogonal[i].firstout=NULL;
		this->orithogonal[i].firstin=NULL;
		this->orithogonal[i].vex_info =NULL;
	}
}
void Graph::EdgeArrayInit(void)
{
	for (int i=0;i<MAX_INDEX;i++)
	{
		this->edge[i].vex_info=NULL;
		this->edge[i].arc.HeadVax=0;
		this->edge[i].arc.TailVax=0;
		this->edge[i].arc.weight=0;
	}
}


//************************************
// Method:    CreateAdjMatrix
// FullName:  Graph::CreateAdjMatrix
// Access:    private 
// Returns:   void
// Qualifier: �����ڽӾ���
// Parameter: void
//************************************
void Graph::CreateAdjMatrix(void)
{
	cout<<"enter vexs and weight"<<endl;
	int vex_num, weight_num;
	cin>>vex_num>>weight_num;
	string vexs1,vexs2;
	int pos1,pos2;
	int wel;
	for (int i=0;i<weight_num;i++)
	{
		cin>>vexs1>>vexs2>>wel;
		pos1=FindPosition(vexs1);
		pos2=FindPosition(vexs2);
		this->arcs[pos1][pos2].adj=wel;
		this->arcs[pos2][pos1].adj=wel;
	}
}
//************************************
// Method:    FindPosition
// FullName:  Graph::FindPosition
// Access:    private 
// Returns:   int
// Qualifier: �ҵ�������±�λ�ã�û�еĻ�����ӽ�ȥ
// Parameter: string ps /*����*/
//************************************
int Graph::FindPosition(string ps)
{
	for(int i=0;i<vexum;i++)
	{
		if(vexs[i]==ps)
			return i;
	}
	if (vexum<MAX_INDEX)
	{
		vexs[vexum]=ps;
		return vexum++;
	}
	cerr<<"overflow...."<<endl;
	exit(-1);
}
//************************************
// Method:    CreateAdjTab
// FullName:  Graph::CreateAdjTab
// Access:    private 
// Returns:   void
// Qualifier: �ڽӱ�
// Parameter: void
//************************************
void Graph::CreateAdjTab(void)
{
	cout<<"enter vexs and weight"<<endl;
	int vex_num, weight_num;
	cin>>vex_num>>weight_num;
	string vexs1,vexs2;
	int pos1,pos2;
	int wel;
	for (int i=0;i<weight_num;i++)
	{
		cin>>vexs1>>vexs2>>wel;
		pos1=FindPosition(vexs1);
		pos2=FindPosition(vexs2);
		Node ** ps=&AdjTab[pos1].first;
		while(*ps)
		{
			ps=&(*ps)->first;
		}
		*ps=(Node*)malloc(sizeof(Node));
		(*ps)->first=NULL;
		(*ps)->adj=pos2;
		(*ps)->data=wel;
	}
}
//************************************
// Method:    CreateOriList
// FullName:  Graph::CreateOriList
// Access:    private 
// Returns:   void
// Qualifier: ����ʮ������
// Parameter: void
//************************************
void Graph::CreateOriList(void)
{
	cout<<"enter vexs and weight"<<endl;
	int vex_num, weight_num;
	cin>>vex_num>>weight_num;
	string vexs1,vexs2;
	int pos1,pos2;
	int wel;
	/*�������ȱ�*/
	for (int i=0;i<weight_num;i++)
	{
		cin>>vexs1>>vexs2>>wel;
		pos1=FindPosition(vexs1);
		pos2=FindPosition(vexs2);
		OriEdgePtr *pt_out=&orithogonal[pos1].firstout;
		while(*pt_out)
		{
			pt_out=&(*pt_out)->TailLink;
		}
		*pt_out=(OriEdgePtr)malloc(sizeof(OriEdge));
		(*pt_out)->HeadLink=NULL;
		(*pt_out)->TailLink=NULL;
		(*pt_out)->HeadVax=pos2;
		(*pt_out)->TailVax=pos1;
		(*pt_out)->weight=wel;
	}
	/*������ȱ�*/
	for (int j=0;j<vex_num;j++)
	{
		OriEdgePtr *pt_out=&orithogonal[j].firstout;
		while(*pt_out)
		{
			OriEdgePtr *pt_in=&orithogonal[(*pt_out)->HeadVax].firstin;
			while(*pt_in)
			{
				pt_in=&(*pt_in)->HeadLink;
			}
			*pt_in=*(pt_out);
			pt_out=&(*pt_out)->TailLink;
		}
	}
	return ;
}
//************************************
// Method:    BFS
// FullName:  Graph::BFS
// Access:    public 
// Returns:   void
// Qualifier: ������ȱ���
//************************************
void Graph::BFS(int start_point)
{
	static queue<int > ps;
	if (start_point>=vexum)
	{
		cerr<<"error overflow"<<endl ;
	}
	if (!VisitedVex[start_point])
	{
		cout<<"points visited :"<<vexs[start_point]<<endl;
		for (int i=0;i<vexum;i++)
		{
			if (i!=start_point&&arcs[start_point][i].adj)
			{
				ps.push(i);
			}
		}
		if (!ps.empty())
		{
			int p=ps.front();
			ps.pop();
			BFS(p);
		}		
	}
}
//************************************
// Method:    DFS
// FullName:  Graph::DFS
// Access:    public 
// Returns:   void
// Qualifier:������ȱ���(�����ڽӾ���)
//************************************
void Graph::DFS(int start_point)
{
	if (start_point>=vexum)
	{
		return ;
	}
	if (!VisitedVex[start_point])
	{
		VisitedVex[start_point]=true;
		cout<<"visited point:"<<vexs[start_point]<<endl;
		for (int i=0;i<vexum;i++)
		{
			if(i!=start_point&&arcs[start_point][i].adj)
				DFS(i);
		}
	}
	
}
//************************************
// Method:    Dijkstra
// FullName:  Graph::Dijkstra
// Access:    public 
// Returns:   void
// Qualifier:�Ͻ�˹�����㷨�����·��
//************************************
void Graph::Dijkstra()
{
	const int  MAXINT = 32767;
	const int MAXNUM = 10;
	int dist[MAXNUM];
	int prev[MAXNUM];
	int A[MAXNUM][MAXNUM];
	int v0=0;
	
	bool S[MAXNUM];                                  // �ж��Ƿ��Ѵ���õ㵽S������
	int n=MAXNUM;
	for(int i=1; i<=n; ++i)
	{
		dist[i] = A[v0][i];
		S[i] = false;                                // ��ʼ��δ�ù��õ�
		if(dist[i] == MAXINT)
			prev[i] = -1;
		else 
			prev[i] = v0;
	}
	dist[v0] = 0;
	S[v0] = true;
	for(int i=2; i<=n; i++)
	{
		int mindist = MAXINT;
		int u = v0;// �ҳ���ǰδʹ�õĵ�j��dist[j]��Сֵ
		for(int j=1; j<=n; ++j)
			if((!S[j]) && dist[j]<mindist)
			{
				u = j;                             // u���浱ǰ�ڽӵ��о�����С�ĵ�ĺ��� 
				mindist = dist[j];
			}
			S[u] = true;
			for(int j=1; j<=n; j++)
				if((!S[j]) && A[u][j]<MAXINT)
				{
					if(dist[u] + A[u][j] < dist[j])     //��ͨ���¼����u��·���ҵ���v0����̵�·��
					{
						dist[j] = dist[u] + A[u][j];    //����dist 
						prev[j] = u;                    //��¼ǰ������ 
					}
				}
	}
}
//************************************
// Method:    Floyd
// FullName:  Graph::Floyd
// Access:    public 
// Returns:   void
// Qualifier:���������㷨�����·��
//************************************
void Graph::Floyd()
{
	int A[MAX_INDEX][MAX_INDEX];
	int path[MAX_INDEX][MAX_INDEX];
	int i,j,k;
	for(i=0;i<vexum;i++)
		for(j=0;j<vexum;j++)
		{
			A[i][j]=arcs[i][j].adj;
			path[i][j]=-1;
		}
		for(k=0;k<vexum;k++)
		{
			for(i=0;i<vexum;i++)
				for(j=0;j<vexum;j++)
					if(A[i][j]>(A[i][k]+A[k][j]))
					{
						A[i][j]=A[i][k]+A[k][j];
						path[i][j]=k;
					} 
		} 
}
//************************************
// Method:    Kruskal
// FullName:  Graph::Kruskal
// Access:    public 
// Returns:   void
// Qualifier:��³˹��������С������
//************************************
void Graph::Kruskal()
{
	int * p=new int[vexum];
	memset(p,'-1',sizeof(int)*vexum);
	qsort((void *)edge,arcum,sizeof(EdgeArray),comparefunc);/*���򣬴�С������*/
	for(int i=0;i<arcum;i++)
	{
		int pos1=edge[i].arc.TailVax;
		int pos2=edge[i].arc.HeadVax;
		int temp=-1;
		while(p[pos2]!=-1&&p[pos2]!=pos1)
		{
			temp=p[pos2];
			pos2=temp;
		}
		if(pos2==-1)
		{
			p[edge[i].arc.HeadVax]=pos1;
			cout<<vexs[edge[i].arc.TailVax]<<"----->"<<vexs[edge[i].arc.HeadVax]<<"\tweight :"<<edge[i].arc.weight<<endl;
		}
	}
	delete p;
}
//************************************
// Method:    Prim
// FullName:  Graph::Prim
// Access:    public 
// Returns:   void
// Qualifier:����ķ�㷨����С������
//************************************
void Graph::Prim()
{
	int i,j,k;
	int *lowcost=new int[vexum];
	int *closeset=new int[vexum];
	int *used=new int[vexum];
	int *father=new int[vexum];
	int min;
	for (i=0;i<vexum;i++)
	{
		lowcost[i]=arcs[0][i].adj;/* ��̾����ʼ��Ϊ�����ڵ㵽1�Žڵ�ľ��� */
		/* ������нڵ���������ΪĬ�ϵ�1�Žڵ� */
		closeset[i]=0; 
		used[i]=0;
		father[i]=-1; 
	}
	used[0]=1;  /*��һ���ڵ�����U�������*/
	/* vcount���ڵ�������Ҫvcount-1���߹�����С������ */
	for (i=1;i<vexum;i++)
	{
		j=0;
		min = INFINIT;
		/* ��������������СȨֵ�ߵĽڵ�k */
		for (k=1;k<vexum;k++)
			/* ��Ȩֵ��С�Ҳ����������� */
			if ((!used[k])&&(lowcost[k]<min)) 
			{
				min =  lowcost[k];
				j=k;
			}
			father[j]=closeset[j]; 
			used[j]=1;;//�ѵ�j�����㲢����U��
			for (k=1;k<vexum;k++)
				/* ���ָ�С��Ȩֵ */
				if (!used[k]&&(arcs[j][k].adj<lowcost[k]))
				{ 
					lowcost[k]=arcs[j][k].adj;/*������СȨֵ*/
					closeset[k]=j;;/*��¼�µ�������*/
				}
	}
	delete lowcost;
	delete closeset;
	delete used;
	delete father;
}
void Graph::test()
{
	//this->CreateAdjMatrix();
	//this->CreateAdjTab();
	this->CreateOriList();
}

int main(int argc,char** argv)
{
	Graph p;
	p.test();
	return 0;
}