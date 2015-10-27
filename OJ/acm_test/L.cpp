#include <iostream>
#include <cmath>
#include <cstdio>
using namespace std;

float areas(float x,float a,float b,float c);

int main(void)
{
	float a1,b1,c1,a2,b2,c2;
	float a,b,c;
	float x1,x2,sums;
	cin>>a1>>b1>>c1;
	cin>>a2>>b2>>c2;
	a=a2-a1;
	b=b2-b1;
	c=c2-c1;
	float deta=b*b-4*a*c;
	if (deta<=0)
	{
		sums=0.0;
	}
	else
	{
		deta=sqrt(deta);
		x1=(-b+deta)/(2*a);/*-b+(b^2-4ac)/2a*/
		x2=(-b-deta)/(2*a);
		sums=areas(x1,a,b,c)-areas(x2,a,b,c);
	}
	//cout<<abs(sums);
	printf("%.3f\n",abs(sums));
	return 0;
}
float areas(float x,float a,float b,float c)
{

	return a/3*x*x*x+b/2*x*x+c*x;
}