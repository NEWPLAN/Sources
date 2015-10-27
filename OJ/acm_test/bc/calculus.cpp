//数值计算实验 数值积分
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

//被积函数
double f(double x)
{
	//为便于调试，先弄个有原函数的 y = x^2 + x^3 - 2*x^4
	return x*x + x*x*x - 2*x*x*x*x;
}

//被积函数 给gsl用的
double g(double x, void * params)
{
	return f(x);
}

//原函数 用于调试算法
double F(double x)
{
	return (x*x*x)/3.0 + (x*x*x*x)/4.0 - 0.4*x*x*x*x*x;
}

//输出被积函数的精确解
double Jinque(const double a, const double b)
{
	return (F(b) - F(a));
}

//梯形法 求函数在[a,b]上的定积分，积分区间分为n部分
double Tixing(const double & a, const double & b, const int & n)
{
	double sum = 0.0;
	double gaps = (b-a)/double(n);  //每个间隔的长度
	for (int i = 0; i < n; i++)
	{
		sum += (gaps/2.0) * (f(a + i*gaps) + f(a + (i+1)*gaps));
	}
	return sum;
}

//抛物线法
double Paowuxian(const double & a, const double & b, const int & n)
{
	double sum = 0.0;
	double gaps = (b-a)/double(n);  //每个间隔的长度
	double h = gaps/2.0;
	for (int i = 0; i < n; i++)
	{
		sum += (h/3.0) * (f(a + i*gaps) + f(a + (i+1)*gaps) + 4.0*f((2*a + (2*i+1)*gaps)/2.0));
	}
	return sum;
}

//柯特斯公式
double Cotes(const double & a, const double & b, const int & n)
{
	double sum = 0.0;
	double gaps = (b-a)/double(n);  //每个间隔的长度
	double h = gaps/2.0;
	for (int i = 0; i < n; i++)
	{
		sum += (h/45.0) * (7.0*f(a + i*gaps) +
			32.0*f(a + i*gaps + 0.25*gaps) + 
			12.0*f(a + i*gaps + 0.5*gaps) + 
			32.0*f(a + i*gaps + 0.75*gaps) + 
			7.0*f(a + (i+1)*gaps));
	}
	return sum;
}

//gsl解法，参考gsl文档
double gslIntegration(double & a, double & b)
{
	gsl_function gf;
	gf.function = g;

	double r, er;
	unsigned int n;
	gsl_integration_qng(&gf, a, b, 1e-10, 1e-10, &r, &er, &n);
	return r;
}

int main()
{
	double a, b;
	int n;
	cout<<"请输入积分区间:"<<endl;
	cout<<"a = ";
	cin>>a;
	cout<<"b = ";
	cin>>b;
	cout<<"请输入分割被积区间的数量:";
	cin>>n;
	if (a > b || n <= 1)
	{
		cout<<"输入错误！"<<endl;
		exit(1);
	}

	//设置输出精度
	cout.precision(10);

	//输出精确解
	double result = Jinque(a, b);
	cout<<"函数在["<<a<<","<<b<<"]上的定积分为:"<<result<<endl;

	//梯形法
	double result1 = Tixing(a, b, n);
	cout<<"梯形法:"<<endl;
	cout<<"函数在["<<a<<","<<b<<"]上的定积分为:"<<result1<<" 相对误差为:"
		<<abs((result1 - result)/result)*100<<"%"<<endl;

	//抛物线法
	double result2 = Paowuxian(a, b, n);
	cout<<"抛物线法:"<<endl;
	cout<<"函数在["<<a<<","<<b<<"]上的定积分为:"<<result2<<" 相对误差为:"
		<<abs((result2 - result)/result)*100<<"%"<<endl;

	//柯特斯公式法
	double result3 = Cotes(a, b, n);
	cout<<"柯特斯法:"<<endl;
	cout<<"函数在["<<a<<","<<b<<"]上的定积分为:"<<result3<<" 相对误差为:"
		<<abs((result3 - result)/result)*100<<"%"<<endl;

	//调用gsl函数
	double result4 = gslIntegration(a, b);
	cout<<"gsl函数结果:"<<endl;
	cout<<"函数在["<<a<<","<<b<<"]上的定积分为:"<<result4<<" 相对误差为:"
		<<abs((result4 - result)/result)*100<<"%"<<endl;
	return 0;
}