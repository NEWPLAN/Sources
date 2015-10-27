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


location AXIS[MAX_M];
int n,m;

struct Circus
{

	location axis;
	double radium;
	bool records[MAX_M];
	int energys;
	void change_ridus(double x)
	{
		if(x>this->radium)/*扩大半径*/
		{
			this->radium=x;
			for (int i=0;i<m;i++)
			{
				if (!records[i])
				{
						int dx,dy;
						dx= AXIS[i].x- this->axis.x;
						dy=AXIS[i].y -this->axis.y;
						double computed=(double)dx*dx+dy*dy;
						if (computed<(this->radium)*(this->radium))
						{
							this->records[i]=true;
							energys++;
						}
				}
			}
		}
		else if (x<this->radium)/*缩小半径*/
		{
			this->radium=x;
			for (int i=0;i<m;i++)
			{
				if (records[i])
				{
					int dx,dy;
					dx= AXIS[i].x- this->axis.x;
					dy=AXIS[i].y -this->axis.y;
					double computed=(double)dx*dx+dy*dy;
					if (computed>=(this->radium)*(this->radium))
					{
						this->records[i]=false;
						energys--;
					}
				}
			}
		}
		return ;
	}
	void inits()
	{
		energys=0;
		for (int i=0;i<m;i++)
		{
			int dx,dy;
			dx= AXIS[i].x- this->axis.x;
			dy=AXIS[i].y -this->axis.y;
			double computed=(double)dx*dx+dy*dy;
			if (computed<(this->radium)*(this->radium))
			{
				this->records[i]=true;
				energys++;
			}
		}
		return ;
	}
};
Circus	CIRCUS[MAX_N];

int energy(int come_from,int end_position);

int change_ridus(int come_from,int end_position,double ridus);
int energy(int come_from,int end_position)
{
	int total=0;
	for (int i=come_from-1;i<end_position;i++)
		total+=CIRCUS[i].energys;
	return total;
}

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


	for(int i=0;i<n;i++)
		CIRCUS[i].inits();


	for (int i=0;i<w;i++)
	{
		cin>>model;
		switch(model)
		{
		case 1://改变半径
			cin>>in1>>in2>>in3;
			for (int i=in1-1;i<in2;i++)
				CIRCUS[i].change_ridus(in3);
			break;
		case 2://计算能量
			cin>>in1>>in2;
			cout<<energy(in1,in2)<<endl;
			break;
		default:
			break;
		}
	}


	return 0;
}