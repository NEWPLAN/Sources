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
	//菜单显示函数
	cout << "==========Welcome to use RSA encoder==========" << endl;
	cout << "\te->" << setw(25) << "Encrypt Messages\t" << "加密\t" << endl;
	cout << "\td->" << setw(25) << "Decrypt CipherMsg\t" << "解密\t" << endl;
	cout << "\ts->" << setw(25) << "Reset RSA Key\t" << "重置\t" << endl;
	cout << "\tp->" << setw(25) << "Display KeyInfo\t" << "显示\t" << endl;
	cout << "\tq->" << setw(25) << "Quit and exit\t" << "退出\t" << endl;
	cout << "input your choice:" << endl;
}

bool islegal(const string& str)
{
	//判断输入是否合法
	for (string::const_iterator it = str.begin(); it != str.end(); ++it)
		if (!isalnum(*it)) //不是字母数字
			return false;
	return true;
}

bool decode(Rsa& rsa)
{
	//解密
	string str;
	do
	{
		cout << ">输入16进制数据:";
		cin >> str;
	}
	while (cin && str.length() < 1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt c(str);

	long t1 = clock();
	BigInt m = rsa.decodeByPr(c);
	long t2 = clock();
	cout << "用时:" << (t2 - t1) << "ms." << endl;

	cout << "密文:" << c << endl
	     << "明文:" << m << endl;
	return true;
}

bool encry(Rsa& rsa, BigInt& c)
{
	//加密
	string str;
	do
	{
		cout << ">输入16进制数据:";
		cin >> str;
	}
	while (cin && str.length() < 1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt m(str);

	c = rsa.encryptByPu(m);

	cout << "明文:" << m << endl
	     << "密文:" << c << endl;
	return true;
}

void print(Rsa& rsa)
{
	//输出
	cout << rsa << endl;
}

void init(Rsa& rsa, int n)
{
	//初始化

	cout << "初始化...." << endl;
	long t1 = clock();
	rsa.init(n);
	long t2 = clock();
	cout << "初始化完成." << endl;
	cout << "用时:" << (t2 - t1) / 1000 << "s." << endl;
}

int go()
{
	//控制函数
	char ch;
	string str;
	Rsa rsa;
	BigInt c, m;
	cout << "输入位数:";
	int n;
	cin >> n;
	init(rsa, n / 2);

	while (true)
	{
		menu();//菜单显示
		cout << ">";
		cin >> str;
		if (!cin)
			return 0;

		if (str.length() < 1)
			cout << "重新输入" << endl;
		else
		{
			ch = str.at(0);
			switch (ch)
			{
			case 'e':
			case 'E':
				encry(rsa, c); //加密
				break;
			case 'd':
			case 'D':
				decode(rsa);//解密
				break;
			case 's':
			case 'S':
				go();//重新开始初始
				break;
			case 'p':
			case 'P':
				print(rsa);//输出公私钥信息
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