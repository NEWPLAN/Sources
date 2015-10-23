/*****第五题  BFS******/
#include<iostream>
#include<stdio.h>
#include<queue>
using namespace std;
#define MAX 55
struct node
{
	int x,y;
	char s;
	bool visited;
	bool flag;
};
int R,C;
node nodes[MAX][MAX];
int BFS(int a,int b,int c,int d)//起点终点 的坐标
{
	queue <node> q;
	nodes[a][b].visited = true;
	q.push(nodes[a][b]);
	while(!q.empty())
	{
		node temp = q.front();
		q.pop();
		if(temp.x == c && temp.y == d)
		{
			for(int i = 0;i < R;i++)//visited重置
				for(int j = 0;j < C;j++)
				{
					nodes[i][j].visited = false;
				}
			return 1;
		}
		if(temp.s == '+' ||temp.s == 'S' || temp.s == 'T')
		{
			if(temp.x-1 >= 0 && nodes[temp.x-1][temp.y].visited == false && nodes[temp.x-1][temp.y].s != '#')
			{
				nodes[temp.x-1][temp.y].visited = true;
				q.push(nodes[temp.x-1][temp.y]);
			}
			if(temp.x+1 < R && nodes[temp.x+1][temp.y].visited == false && nodes[temp.x+1][temp.y].s != '#')
			{
				nodes[temp.x+1][temp.y].visited = true;
				q.push(nodes[temp.x+1][temp.y]);
			}
			if(temp.y-1 >= 0 && nodes[temp.x][temp.y-1].visited == false && nodes[temp.x][temp.y-1].s != '#')
			{
				nodes[temp.x][temp.y-1].visited = true;
				q.push(nodes[temp.x][temp.y-1]);
			}
			if(temp.y+1 < C && nodes[temp.x][temp.y+1].visited == false && nodes[temp.x][temp.y+1].s != '#')
			{
				nodes[temp.x][temp.y+1].visited = true;
				q.push(nodes[temp.x][temp.y+1]);
			}
		}
		else if(temp.s == '-')
		{
			if(temp.y-1 >= 0 && nodes[temp.x][temp.y-1].visited == false && nodes[temp.x][temp.y-1].s != '#')
			{
				nodes[temp.x][temp.y-1].visited = true;
				q.push(nodes[temp.x][temp.y-1]);
			}
			if(temp.y+1 < C && nodes[temp.x][temp.y+1].visited == false && nodes[temp.x][temp.y+1].s != '#')
			{
				nodes[temp.x][temp.y+1].visited = true;
				q.push(nodes[temp.x][temp.y+1]);
			}
		}
		else if(temp.s == '|')
		{
			if(temp.x-1 >= 0 && nodes[temp.x-1][temp.y].visited == false && nodes[temp.x-1][temp.y].s != '#')
			{
				nodes[temp.x-1][temp.y].visited = true;
				q.push(nodes[temp.x-1][temp.y]);
			}
			if(temp.x+1 < R && nodes[temp.x+1][temp.y].visited == false && nodes[temp.x+1][temp.y].s != '#')
			{
				nodes[temp.x+1][temp.y].visited = true;
				q.push(nodes[temp.x+1][temp.y]);
			}
		}
		else if(temp.s == '.')
		{
			if(temp.x+1 < R && nodes[temp.x+1][temp.y].visited == false && nodes[temp.x+1][temp.y].s != '#')
			{
				nodes[temp.x+1][temp.y].visited = true;
				q.push(nodes[temp.x+1][temp.y]);
			}
		}
	}
	for(int i = 0;i < R;i++)//visited重置
		for(int j = 0;j < C;j++)
		{
			nodes[i][j].visited = false;
		}
	return 0;
}

int main()
{
	char s[MAX][MAX];//暂时保存各个字符串
	int x1,y1,x2,y2;//分别作为起点终点坐标
	int count = 0;//记录满足性质的方格个数
	scanf("%d%d",&R,&C);
	for(int i = 0;i < R;i++)
		scanf("%s",s[i]);
	for(int i = 0;i < R;i++)
		for(int j = 0;j < C;j++)
			{
				nodes[i][j].s = s[i][j];
				nodes[i][j].x = i;
				nodes[i][j].y = j;
				if(s[i][j] == 'S')
				{
					x1 = i;
					y1 = j;
				}
				if(s[i][j] == 'T')
				{
					x2 = i;
					y2 = j;
				}
			}
	if(BFS(x1,y1,x2,y2) == 0)
	{
		printf("I'm stuck!\n");
		return 0;
	}
	for(int i = 0;i < R;i++)//判断性质1
		for(int j = 0;j < C;j++)
		{
			if(BFS(x1,y1,i,j) == 1)
				nodes[i][j].flag = true;
		}
	for(int i = 0;i < R;i++)//判断性质2
		for(int j = 0;j < C;j++)
		{
			if(BFS(i,j,x2,y2) == 1)
				nodes[i][j].flag = false;
		}
	for(int i = 0;i < R;i++)
		for(int j = 0;j < C;j++)
		{
			if(nodes[i][j].flag == true)
				count++;
		}
	printf("%d\n",count);
}
