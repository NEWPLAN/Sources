/************************************************************************/
/* 图的基本算法和属性                                                      */
/* 图的存储                                                              */
/* 邻接表(任意顶点之间有边)就可以使用一条线连接起来                             */
/* 邻接矩阵(连接一个顶点与之相关连的顶点的集合)                                */
/* 十字链表（十字链表可以清楚计算入度和出度）                                  */
/* 边集数组（存放边的数组信息）节点包括三个信息：起点、终点、权重                  */
/************************************************************************/

#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#define  MAX_INDEX 20
#define  INFINIT	1<<30

using namespace std;



/*************邻阶矩阵节点******************/
typedef struct ArcCell
{
	int adj;	/*边的权*/
	char* info;	/*边信息*/
}ArcCell, AdjMatrix[MAX_INDEX][MAX_INDEX];
/*************邻阶矩阵节点******************/


/*******************十字链表节点*****************************/
typedef struct OriEdge
{
	struct OriEdge * HeadLink,*TailLink;/*指向弧起点下一个线，指向弧终点下一条线*/
	int TailVax,HeadVax;/*弧起点下标，弧终点下标*/
	int weight;/*权重*/
}OriEdge,*OriEdgePtr;

typedef struct OriNode
{
	struct OriEdge *firstin,*firstout;
	char* vex_info;/*节点信息*/
}OriNode, *OriList;
/*******************十字链表节点*****************************/


/*******************边集数组*****************************/
typedef struct EdgeNode
{
	int TailVax,HeadVax;/*弧起点下标，弧终点下标*/
	int weight;/*权重*/
}EdgeNode,*EdgeNodePtr;

typedef struct EdgeArray
{
	EdgeNode arc;/*边的信息*/
	char* vex_info;/*节点信息*/
}EdgeArray,*EdgeArrayPtr;
/*******************边集数组*****************************/


/*************邻阶表的节点******************/
typedef struct Node
{
	struct Node * first;
	int data;	/*顶点的下标索引*/
	int adj;/*权重*/
}Node;
/*************邻阶表的节点******************/

int comparefunc(const void* p1,const void * p2)
{
	return ((EdgeArrayPtr)p1)->arc.weight>((EdgeArrayPtr)p2)->arc.weight;
}

class Graph
{
private:
	bool orientation;
	AdjMatrix arcs;			/*图的邻接矩阵*/
	string vexs[MAX_INDEX];	/*顶点的标识*/
	int vexum,arcum;		/*图的顶点数目、边的数目*/
	Node AdjTab[MAX_INDEX];	/*邻接表*/
	OriNode orithogonal[MAX_INDEX];/*十字链表*/
	EdgeArray edge[MAX_INDEX];/*边集数组*/
	bool VisitedVex[MAX_INDEX];/*已经访问过的节点*/

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
	void Dijkstra();/*迪杰斯特拉算法：最短路径 （点为操作重点）*/
	void Prim();/*普利姆算法：最小生成树（边为操作重点）*/
	void Kruskal();/*克鲁斯卡尔算法：最小生成树（边为操作重点）*/
	void Floyd();/*弗洛伊德算法：最短路径算法（点为操作重点）*/
	void DFS(int start_point=0);/*深度优先遍历*/
	void BFS(int start_point=0);/*广度优先遍历*/
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
// Qualifier: 创建邻接矩阵
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
// Qualifier: 找到顶点的下标位置，没有的话，添加进去
// Parameter: string ps /*顶点*/
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
// Qualifier: 邻接表
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
// Qualifier: 创建十字链表
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
	/*构建出度表*/
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
	/*构建入度表*/
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
// Qualifier: 广度优先遍历
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
// Qualifier:深度优先遍历(基于邻接矩阵)
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
// Qualifier:迪杰斯特拉算法求最短路径
//************************************
void Graph::Dijkstra()
{
	const int  MAXINT = 32767;
	const int MAXNUM = 10;
	int dist[MAXNUM];
	int prev[MAXNUM];
	int A[MAXNUM][MAXNUM];
	int v0=0;
	
	bool S[MAXNUM];                                  // 判断是否已存入该点到S集合中
	int n=MAXNUM;
	for(int i=1; i<=n; ++i)
	{
		dist[i] = A[v0][i];
		S[i] = false;                                // 初始都未用过该点
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
		int u = v0;// 找出当前未使用的点j的dist[j]最小值
		for(int j=1; j<=n; ++j)
			if((!S[j]) && dist[j]<mindist)
			{
				u = j;                             // u保存当前邻接点中距离最小的点的号码 
				mindist = dist[j];
			}
			S[u] = true;
			for(int j=1; j<=n; j++)
				if((!S[j]) && A[u][j]<MAXINT)
				{
					if(dist[u] + A[u][j] < dist[j])     //在通过新加入的u点路径找到离v0点更短的路径
					{
						dist[j] = dist[u] + A[u][j];    //更新dist 
						prev[j] = u;                    //记录前驱顶点 
					}
				}
	}
}
//************************************
// Method:    Floyd
// FullName:  Graph::Floyd
// Access:    public 
// Returns:   void
// Qualifier:弗洛伊德算法求最短路径
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
// Qualifier:克鲁斯卡尔求最小生成树
//************************************
void Graph::Kruskal()
{
	int * p=new int[vexum];
	memset(p,'-1',sizeof(int)*vexum);
	qsort((void *)edge,arcum,sizeof(EdgeArray),comparefunc);/*排序，从小到大拍*/
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
// Qualifier:普利姆算法求最小生成树
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
		lowcost[i]=arcs[0][i].adj;/* 最短距离初始化为其他节点到1号节点的距离 */
		/* 标记所有节点的依附点皆为默认的1号节点 */
		closeset[i]=0; 
		used[i]=0;
		father[i]=-1; 
	}
	used[0]=1;  /*第一个节点是在U集合里的*/
	/* vcount个节点至少需要vcount-1条边构成最小生成树 */
	for (i=1;i<vexum;i++)
	{
		j=0;
		min = INFINIT;
		/* 找满足条件的最小权值边的节点k */
		for (k=1;k<vexum;k++)
			/* 边权值较小且不在生成树中 */
			if ((!used[k])&&(lowcost[k]<min)) 
			{
				min =  lowcost[k];
				j=k;
			}
			father[j]=closeset[j]; 
			used[j]=1;;//把第j个顶点并入了U中
			for (k=1;k<vexum;k++)
				/* 发现更小的权值 */
				if (!used[k]&&(arcs[j][k].adj<lowcost[k]))
				{ 
					lowcost[k]=arcs[j][k].adj;/*更新最小权值*/
					closeset[k]=j;;/*记录新的依附点*/
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