#include <iostream>
#include <ctime>
#include"BigInt.h"
#include"Rsa.h"
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/main.cpp
using namespace std;

void menu(void)
{
	//²Ëµ¥ÏÔÊ¾º¯Êı
	cout << "==========Welcome to use RSA encoder==========" << endl;
	cout << "               e.encrypt ¼ÓÃÜ              " << endl;
	cout << "               d.decrypt ½âÃÜ              " << endl;
	cout << "               s.setkey ÖØÖÃ               " << endl;
	cout << "               p.print ÏÔÊ¾               " << endl;
	cout << "               q.quit ÍË³ö                 " << endl;
	cout << "input your choice:" << endl;
}

bool islegal(const string& str)
{
	//ÅĞ¶ÏÊäÈëÊÇ·ñºÏ·¨
	for (string::const_iterator it = str.begin(); it != str.end(); ++it)
		if (!isalnum(*it)) //²»ÊÇ×ÖÄ¸Êı×Ö
=======
using std::cout;
using std::endl;
using std::cin;

void menu()
{//èœå•æ˜¾ç¤ºå‡½æ•°
	cout<<"==========Welcome to use RSA encoder=========="<<endl;
	cout<<"               e.encrypt åŠ å¯†              "<<endl;
	cout<<"               d.decrypt è§£å¯†              "<<endl;
	cout<<"               s.setkey é‡ç½®               "<<endl;
	cout<<"               p.print æ˜¾ç¤º               "<<endl;
	cout<<"               q.quit é€€å‡º                 "<<endl;
	cout<<"input your choice:"<<endl;
}

bool islegal(const string& str)
{//åˆ¤æ–­è¾“å…¥æ˜¯å¦åˆæ³•
	for(string::const_iterator it=str.begin();it!=str.end();++it)
		if(!isalnum(*it))//ä¸æ˜¯å­—æ¯æ•°å­—
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/main.cpp
			return false;
	return true;
}

bool decode(Rsa& rsa)
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/main.cpp
{
	//½âÃÜ
	string str;
	do
	{
		cout << ">ÊäÈë16½øÖÆÊı¾İ:";
		cin >> str;
	}
	while (cin && str.length() < 1);
	if (!cin || islegal(str) == false)
		return false;
	BigInt c(str);

	long t1 = clock();
	BigInt m = rsa.decodeByPr(c);
	long t2 = clock();
	cout << "ÓÃÊ±:" << (t2 - t1) << "ms." << endl;

	cout << "ÃÜÎÄ:" << c << endl
	     << "Ã÷ÎÄ:" << m << endl;
	return true;
}

bool encry(Rsa& rsa, BigInt& c)
{
	//¼ÓÃÜ
	string str;
	do
	{
		cout << ">ÊäÈë16½øÖÆÊı¾İ:";
		cin >> str;
	}
	while (cin && str.length() < 1);
	if (!cin || islegal(str) == false)
=======
{//è§£å¯†
	string str;
	do
	{
		cout<<">è¾“å…¥16è¿›åˆ¶æ•°æ®:";
		cin>>str;
	}while(cin && str.length()<1);
	if(!cin || islegal(str)==false)
		return false;
	BigInt c(str);
	
	long t1=clock();
	BigInt m=rsa.decodeByPr(c);
	long t2=clock();
	cout<<"ç”¨æ—¶:"<<(t2-t1)<<"ms."<<endl;

	cout<<"å¯†æ–‡:"<<c<<endl
		<<"æ˜æ–‡:"<<m<<endl;
	return true;
}

bool encry(Rsa& rsa,BigInt& c)
{//åŠ å¯†
	string str;
	do
	{
		cout<<">è¾“å…¥16è¿›åˆ¶æ•°æ®:";
		cin>>str;
	}while(cin && str.length()<1);
	if(!cin || islegal(str)==false)
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/main.cpp
		return false;
	BigInt m(str);

	c = rsa.encryptByPu(m);

<<<<<<< HEAD:homeworks/INFO_SEC/RSA/main.cpp
	cout << "Ã÷ÎÄ:" << m << endl
	     << "ÃÜÎÄ:" << c << endl;
=======
	cout<<"æ˜æ–‡:"<<m<<endl
		<<"å¯†æ–‡:"<<c<<endl;
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/main.cpp
	return true;
}

void print(Rsa& rsa)
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/main.cpp
{
	//Êä³ö
	cout << rsa << endl;
}

void init(Rsa& rsa, int n)
{
	//³õÊ¼»¯

	cout << "³õÊ¼»¯...." << endl;
	long t1 = clock();
	rsa.init(n);
	long t2 = clock();
	cout << "³õÊ¼»¯Íê³É." << endl;
	cout << "ÓÃÊ±:" << (t2 - t1) / 1000 << "s." << endl;
}

int go()
{
	//¿ØÖÆº¯Êı
	char ch;
	string str;
	Rsa rsa;
	BigInt c, m;
	cout << "ÊäÈëÎ»Êı:";
=======
{//è¾“å‡º
	cout<<rsa<<endl;
}

void init(Rsa& rsa,int n)
{//åˆå§‹åŒ–

	cout<<"åˆå§‹åŒ–...."<<endl;
	long t1=clock();
	rsa.init(n);
	long t2=clock();
	cout<<"åˆå§‹åŒ–å®Œæˆ."<<endl;
	cout<<"ç”¨æ—¶:"<<(t2-t1)/1000<<"s."<<endl;
}

int go()
{//æ§åˆ¶å‡½æ•°
	char ch;
	string str;
	Rsa rsa;
	BigInt c,m;
	cout<<"è¾“å…¥ä½æ•°:";
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/main.cpp
	int n;
	cin >> n;
	init(rsa, n / 2);

	while (true)
	{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/main.cpp
		menu();//²Ëµ¥ÏÔÊ¾
		cout << ">";
		cin >> str;
		if (!cin)
			return 0;

		if (str.length() < 1)
			cout << "ÖØĞÂÊäÈë" << endl;
=======
		menu();//èœå•æ˜¾ç¤º
		cout<<">";
		cin>>str;
		if(!cin)
			return 0;
		
		if(str.length()<1)
			cout<<"é‡æ–°è¾“å…¥"<<endl;
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/main.cpp
		else
		{
			ch = str.at(0);
			switch (ch)
			{
			case 'e':
			case 'E':
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/main.cpp
				encry(rsa, c); //¼ÓÃÜ
=======
				encry(rsa,c);//åŠ å¯†
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/main.cpp
				break;
			case 'd':
			case 'D':
				decode(rsa);//è§£å¯†
				break;
			case 's':
			case 'S':
				go();//é‡æ–°å¼€å§‹åˆå§‹
				break;
			case 'p':
			case 'P':
				print(rsa);//è¾“å‡ºå…¬ç§é’¥ä¿¡æ¯
				break;
			case 'q':
			case 'Q':
				return 0;
			}
		}
	}
}


int main()
{
	go();
}
