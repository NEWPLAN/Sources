#include <cmath>
#include <iostream>

using namespace std;

double Simpson(double(*fun)(double), double a, double b, double n)
{// �����߷��󶨻���
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
{// ���η��󶨻���
	double  y1,y2  ;   //��������a��b��Ӧ�ĺ���ֵ
	double  per_len ,height ;
	double area , fx = 0.0 ;
	int i ;

	per_len = ( b - a ) / n ;   //�ȷֳ�n��

	y1 = (*p)( a ) ;

	for( i = 1 ; i <= n ; i++ )
	{
		b = a + per_len ;        //��λ���䳤��
		y2 = (*p)(b) ;            
		height = (y1 + y2 ) / 2.0 ;   //��
		area = per_len * height ;   //ÿһ�ȷݳ��ȶ�Ӧ�����
		a = b ;               //��������
		y1 = y2 ;               //���� 
		fx = fx + area ;       //���֮��Ϊ���ֵ�ֵ
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
		cout<<"����������¡����޺�����ȷ���(�м��Կո������ �����a=0 b=0 n=0����)" << endl;
		cin>>a>>b>>n;
		if (n==0)
		{
			cout << "�������������" << endl;
			continue;
		}
		cout << "��ѡ����ַ�ʽ��1�����η��� 2�������߷�" << endl;
		cin >> flag;
		switch(flag) 
		{
		case 1:
			simp = def_integral(a, b, n, fun);
			cout << "�Ծ������� " << endl;
			break;
		case 2:
			simp=Simpson(fun,a,b,n);
			cout << "�������߷����� " << endl;
			break;
		default:
			simp=Simpson(fun,a,b,n);
			break;
		}
		cout << "������F(" << a << ", " << b  << ") = " << simp <<endl;
		cout << endl;		
	}

	return 0;
}