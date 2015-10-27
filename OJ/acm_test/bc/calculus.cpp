//��ֵ����ʵ�� ��ֵ����
#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

//��������
double f(double x)
{
	//Ϊ���ڵ��ԣ���Ū����ԭ������ y = x^2 + x^3 - 2*x^4
	return x*x + x*x*x - 2*x*x*x*x;
}

//�������� ��gsl�õ�
double g(double x, void * params)
{
	return f(x);
}

//ԭ���� ���ڵ����㷨
double F(double x)
{
	return (x*x*x)/3.0 + (x*x*x*x)/4.0 - 0.4*x*x*x*x*x;
}

//������������ľ�ȷ��
double Jinque(const double a, const double b)
{
	return (F(b) - F(a));
}

//���η� ������[a,b]�ϵĶ����֣����������Ϊn����
double Tixing(const double & a, const double & b, const int & n)
{
	double sum = 0.0;
	double gaps = (b-a)/double(n);  //ÿ������ĳ���
	for (int i = 0; i < n; i++)
	{
		sum += (gaps/2.0) * (f(a + i*gaps) + f(a + (i+1)*gaps));
	}
	return sum;
}

//�����߷�
double Paowuxian(const double & a, const double & b, const int & n)
{
	double sum = 0.0;
	double gaps = (b-a)/double(n);  //ÿ������ĳ���
	double h = gaps/2.0;
	for (int i = 0; i < n; i++)
	{
		sum += (h/3.0) * (f(a + i*gaps) + f(a + (i+1)*gaps) + 4.0*f((2*a + (2*i+1)*gaps)/2.0));
	}
	return sum;
}

//����˹��ʽ
double Cotes(const double & a, const double & b, const int & n)
{
	double sum = 0.0;
	double gaps = (b-a)/double(n);  //ÿ������ĳ���
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

//gsl�ⷨ���ο�gsl�ĵ�
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
	cout<<"�������������:"<<endl;
	cout<<"a = ";
	cin>>a;
	cout<<"b = ";
	cin>>b;
	cout<<"������ָ�����������:";
	cin>>n;
	if (a > b || n <= 1)
	{
		cout<<"�������"<<endl;
		exit(1);
	}

	//�����������
	cout.precision(10);

	//�����ȷ��
	double result = Jinque(a, b);
	cout<<"������["<<a<<","<<b<<"]�ϵĶ�����Ϊ:"<<result<<endl;

	//���η�
	double result1 = Tixing(a, b, n);
	cout<<"���η�:"<<endl;
	cout<<"������["<<a<<","<<b<<"]�ϵĶ�����Ϊ:"<<result1<<" ������Ϊ:"
		<<abs((result1 - result)/result)*100<<"%"<<endl;

	//�����߷�
	double result2 = Paowuxian(a, b, n);
	cout<<"�����߷�:"<<endl;
	cout<<"������["<<a<<","<<b<<"]�ϵĶ�����Ϊ:"<<result2<<" ������Ϊ:"
		<<abs((result2 - result)/result)*100<<"%"<<endl;

	//����˹��ʽ��
	double result3 = Cotes(a, b, n);
	cout<<"����˹��:"<<endl;
	cout<<"������["<<a<<","<<b<<"]�ϵĶ�����Ϊ:"<<result3<<" ������Ϊ:"
		<<abs((result3 - result)/result)*100<<"%"<<endl;

	//����gsl����
	double result4 = gslIntegration(a, b);
	cout<<"gsl�������:"<<endl;
	cout<<"������["<<a<<","<<b<<"]�ϵĶ�����Ϊ:"<<result4<<" ������Ϊ:"
		<<abs((result4 - result)/result)*100<<"%"<<endl;
	return 0;
}