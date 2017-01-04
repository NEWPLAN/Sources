#include <iostream>
#include <cmath>
using namespace  std;

#define  MAX_N 10000
#define  MAX_M 100
struct location
{
	int x;
	int y;
};

struct Circus
{
	location axis;
	double radium;
};
location AXIS[MAX_M];
Circus	CIRCUS[MAX_N];
int n,m;


int energy(int come_from,int end_position);
int change_ridus(int come_from,int end_position,double ridus);

int main(void)
{
	int w;
	int model,in1,in2;
	double in3;
	cin>>n>>m;
	for(int i=0;i<n;i++)
		cin>>CIRCUS[i].axis.x>>CIRCUS[i].axis.y>>CIRCUS[i].radium;
	for (int i=0;i<m;i++)
		cin>>AXIS[i].x>>AXIS[i].y;
	cin>>w;
	for (int i=0;i<w;i++)
	{
		cin>>model;
		switch(model)
		{
		case 1:/*改变半径*/
			cin>>in1>>in2>>in3;
			change_ridus(in1,in2,in3);
			break;
		case 2:/*计算机能量*/
			cin>>in1>>in2;
			cout<<energy(in1,in2)<<endl;
			break;
		default:
			break;
		}
	}


	return 0;
}

inline int energy(int come_from,int end_position)
{
	int total=0;
	for (int i=0;i<m;i++)
	{
		for(int j=come_from-1;j<end_position;j++)
		{
			int dx,dy;
			dx= AXIS[i].x- CIRCUS[j].axis.x;
			dy=AXIS[i].y - CIRCUS[j].axis.y;
			double computed=sqrt((double)dx*dx+dy*dy);
			if (computed<CIRCUS[j].radium)
				total++;
		}
	}
	return total;
}

inline int change_ridus(int come_from,int end_position,double ridus)
{
	for (int i=come_from-1;i<end_position;i++)
	{
		CIRCUS[i].radium=ridus;
	}
	return 0;
}