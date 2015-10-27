#include <iostream>
#include <cmath>

using namespace std;
#define  MAX_INPUT 10000
class Point
{
public:
	double x;
	double y;
};

Point p_axis[MAX_INPUT];
Point caption;
double premeter(double x1,double y1,double x2,double y2);
bool check_in_view(Point p);

int main(void)
{
	int n;
	int start_x,start_y;
	cin>>n;
	for (int i=0; i<n;i++)
	{
		cin>>p_axis[i].x>>p_axis[i].y;
	}
	cin>>caption.x>>caption.y;

	return 0;
}

double premeter(double x1,double y1,double x2,double y2)
{
	double dx=x1-x2;
	double dy=y1-y2;
	return sqrt(dx*dx+dy*dy);
}

bool check_in_view(Point p)
{
	return true;
}