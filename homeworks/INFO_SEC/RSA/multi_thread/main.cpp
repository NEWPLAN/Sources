#include<iostream>
#include<ctime>
#include<iomanip>
#include"BigInt.h"
#include"Rsa.h"
using namespace std;

//FFT:				http://blog.csdn.net/justdoithai/article/details/51278038
//bignum times		http://www.cnblogs.com/king-ding/p/bigIntegerMul.html
//FFT				http://blog.csdn.net/jackyguo1992/article/details/12613287
//FFT				https://wenku.baidu.com/view/2788b20c52ea551810a6879c.html?re=view
//FFT				https://wenku.baidu.com/view/79162aa1bed5b9f3f90f1ca8.html
//9999!				http://blog.csdn.net/chenyujing1234/article/details/7447682
//FFT				http://blog.jobbole.com/70549/
//FFT				http://daily.zhihu.com/story/3935067
//bignum times		https://wenku.baidu.com/view/317bddf3534de518964bcf84b9d528ea81c72fe2.html
//FFT				http://cjjlsdy.blog.163.com/blog/static/180370563201461133026
//Fast exponential	http://www.cnblogs.com/llsq/p/5810262.html



void menu()
{
	//�˵���ʾ����
	cout << "==========Welcome to use RSA encoder==========" << endl;
	cout << "\te->" << setw(25) << "Encrypt Messages\t" << "����\t" << endl;
	cout << "\td->" << setw(25) << "Decrypt CipherMsg\t" << "����\t" << endl;
	cout << "\ts->" << setw(25) << "Reset RSA Key\t" << "����\t" << endl;
	cout << "\tp->" << setw(25) << "Display KeyInfo\t" << "��ʾ\t" << endl;
	cout << "\tq->" << setw(25) << "Quit and exit\t" << "�˳�\t" << endl;
	cout << "input your choice:" << endl;
}

bool islegal(const string& str)
{
	//�ж������Ƿ�Ϸ�
	for (string::const_iterator it = str.begin(); it != str.end(); ++it)
		if (!isalnum(*it)) //������ĸ����
			return false;
	return true;
}

bool decode(Rsa& rsa)
{
	//����
	string str;
	do
	{
		cout << ">����16��������:";
		cin >> str;
	}
	while (cin && str.length() < 1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt c(str);

	long t1 = clock();
	BigInt m = rsa.decodeByPr(c);
	long t2 = clock();
	cout << "��ʱ:" << (t2 - t1) << "ms." << endl;

	cout << "����:" << c << endl
	     << "����:" << m << endl;
	return true;
}

bool encry(Rsa& rsa, BigInt& c)
{
	//����
	string str;
	do
	{
		cout << ">����16��������:";
		cin >> str;
	}
	while (cin && str.length() < 1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt m(str);

	c = rsa.encryptByPu(m);

	cout << "����:" << m << endl
	     << "����:" << c << endl;
	return true;
}

void print(Rsa& rsa)
{
	//���
	cout << rsa << endl;
}

void init(Rsa& rsa, int n)
{
	//��ʼ��

	cout << "��ʼ��...." << endl;
	long t1 = clock();
	rsa.init(n);
	long t2 = clock();
	cout << "��ʼ�����." << endl;
	cout << "��ʱ:" << (t2 - t1) / 1000 << "s." << endl;
}

int go()
{
	//���ƺ���
	char ch;
	string str;
	Rsa rsa;
	BigInt c, m;
	cout << "����λ��:";
	int n;
	cin >> n;
	init(rsa, n / 2);

	while (true)
	{
		menu();//�˵���ʾ
		cout << ">";
		cin >> str;
		if (!cin)
			return 0;

		if (str.length() < 1)
			cout << "��������" << endl;
		else
		{
			ch = str.at(0);
			switch (ch)
			{
			case 'e':
			case 'E':
				encry(rsa, c); //����
				break;
			case 'd':
			case 'D':
				decode(rsa);//����
				break;
			case 's':
			case 'S':
				go();//���¿�ʼ��ʼ
				break;
			case 'p':
			case 'P':
				print(rsa);//�����˽Կ��Ϣ
				break;
			case 'q':
			case 'Q':
				return 0;
			}
		}
	}
}

void testBignum(void)
{
	string s1, s2;

	while (1)
	{
		cin >> s1 >> s2;
		cout << s1 << endl;
		cout << s2 << endl;
		BigInt a(s1), b(s2);
		long t1 = clock();
		cout << a*b << endl;
		cout << (clock() - t1) / 1000.0 << " ms" << endl;
		//cin >> s1 >> s2;
	}
	return ;
}

int main()
{
	//testBignum();
	go();
}