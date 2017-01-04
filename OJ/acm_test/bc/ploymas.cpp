#include <cmath>
#include <iostream>

using namespace std;

double Simpson(double(*fun)(double), double a, double b, double n)
{// 抛物线法求定积分
	double k,simp,s1,s2=0.0;
	double h=(b-a)/n;
	s1=fun(b+h/2);
	for(k=1;k<=n-1;k++)
	{ 
		s1+=fun(a+k*h+h/2);
		s2+=fun(a+k*h);
	}
	simp=h/6*(fun(a)+4*s1+2*s2+fun(b));
	return simp;
}

double def_integral(double a, double b, double n, double (*p)(double x))
{// 矩形法求定积分
	double  y1,y2  ;   //积分区间a、b对应的函数值
	double  per_len ,height ;
	double area , fx = 0.0 ;
	int i ;

	per_len = ( b - a ) / n ;   //等分成n分

	y1 = (*p)( a ) ;

	for( i = 1 ; i <= n ; i++ )
	{
		b = a + per_len ;        //单位区间长度
		y2 = (*p)(b) ;            
		height = (y1 + y2 ) / 2.0 ;   //高
		area = per_len * height ;   //每一等份长度对应的面积
		a = b ;               //下限右移
		y1 = y2 ;               //右移 
		fx = fx + area ;       //面积之和为积分的值
	}

	return fx;
}

double fun(double x) 
{ 
	return x*x - 5*x + 1;
}

int main(void)
{
	double a=-1,b=-1,n,simp;
	int flag = 1;
	while (!((int)(a*1000)==0 && (int)(b*1000)==0))
	{
		cout<<"请输入积分下、上限和区间等分数(中间以空格隔开， 最后以a=0 b=0 n=0结束)" << endl;
		cin>>a>>b>>n;
		if (n==0)
		{
			cout << "区间数输入错误" << endl;
			continue;
		}
		cout << "请选择积分方式：1、矩形法， 2、抛物线法" << endl;
		cin >> flag;
		switch(flag) 
		{
		case 1:
			simp = def_integral(a, b, n, fun);
			cout << "以矩阵法运行 " << endl;
			break;
		case 2:
			simp=Simpson(fun,a,b,n);
			cout << "以抛物线法运行 " << endl;
			break;
		default:
			simp=Simpson(fun,a,b,n);
			break;
		}
		cout << "定积分F(" << a << ", " << b  << ") = " << simp <<endl;
		cout << endl;		
	}

	return 0;
}