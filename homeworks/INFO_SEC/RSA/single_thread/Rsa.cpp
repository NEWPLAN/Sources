#include "Rsa.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>


using namespace std;

Rsa::Rsa() {}

Rsa::~Rsa() {}

void Rsa::init(unsigned int n)
{
	srand(time(NULL));
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
	//²úÉú´óËØÊýp¡¢q
	_p = createPrime(n, 10);
	_q = createPrime(n, 10);
	//¼ÆËãN
	N = _p * _q;
	//¼ÆËã³öÅ·À­Êý
	_ol = (_p - 1) * (_q - 1);
	//¼ÓÃÜÖ¸Êýe
=======
	//äº§ç”Ÿå¤§ç´ æ•°pã€q
	_p=createPrime(n,10);
	_q=createPrime(n,10);
	//è®¡ç®—N
	N=_p*_q;
	//è®¡ç®—å‡ºæ¬§æ‹‰æ•°
	_ol=(_p-1)*(_q-1);
	//åŠ å¯†æŒ‡æ•°e
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
	createExp(_ol);
	//d
}

BigInt Rsa::createOddNum(unsigned int n)
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
{
	//Éú³É³¤¶ÈÎªnµÄÆæÊý
	n = n / 4;
	static unsigned char hex_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	if (n)
=======
{//ç”Ÿæˆé•¿åº¦ä¸ºnçš„å¥‡æ•°
	n=n/4;
	static unsigned char hex_table[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	if(n)
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
	{
		ostringstream oss;
		for (size_t i = 0; i < n - 1; ++i)
			oss << hex_table[rand() % 16];
		oss << hex_table[1];
		string str(oss.str());
		return BigInt(str);
	}
	else
		return BigInt::Zero;
}

<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
bool Rsa::isPrime(const BigInt& n, const unsigned int k)
{
	//ÅÐ¶ÏËØÊý
	assert(n != BigInt::Zero);
	if (n == BigInt::Two)
		return true;

	BigInt n_1 = n - 1;
	BigInt::bit b(n_1);//¶þ½øÖÆÎ»
	if (b.at(0) == 1)
=======
bool Rsa::isPrime(const BigInt& n,const unsigned int k)
{//åˆ¤æ–­ç´ æ•°
	assert(n!=BigInt::Zero);
	if(n==BigInt::Two)
		return true;

	BigInt n_1=n-1;
	BigInt::bit b(n_1);//äºŒè¿›åˆ¶ä½
	if(b.at(0)==1)
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
		return false;

	for (std::size_t t = 0; t < k; ++t)
	{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
		BigInt a = createRandomSmallThan(n_1); //Ëæ»úÊý
=======
		BigInt a=createRandomSmallThan(n_1);//éšæœºæ•°
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
		BigInt d(BigInt::One);
		for (int i = b.size() - 1; i >= 0; --i)
		{
			BigInt x = d;
			d = (d * d) % n;
			if (d == BigInt::One && x != BigInt::One && x != n_1)
				return false;

			if (b.at(i))
			{
				assert(d != BigInt::Zero);
				d = (a * d) % n;
			}
		}
		if (d != BigInt::One)
			return false;
	}
	return true;
}

BigInt Rsa::createRandomSmallThan(const BigInt& a)
{
	unsigned long t = 0;
	do
	{
		t = rand();
	}
	while (t == 0);

	BigInt mod(t);
	BigInt r = mod % a;
	if (r == BigInt::Zero)
		r = a - BigInt::One;
	return r;
}

<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
BigInt Rsa::createPrime(unsigned int n, int it_count)
{
	//Éú³É³¤¶ÈÎªnµÄËØÊý
	assert(it_count > 0);
	BigInt res = createOddNum(n);
	while (!isPrime(res, it_count))
=======
BigInt Rsa::createPrime(unsigned int n,int it_count)
{//ç”Ÿæˆé•¿åº¦ä¸ºnçš„ç´ æ•°
	assert(it_count>0);
	BigInt res=createOddNum(n);
	while(!isPrime(res,it_count))
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
	{
		res.add(BigInt::Two);
	}
	return res;
}

void Rsa::createExp(const BigInt& ou)
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
{
	//´ÓÒ»¸öÅ·À­ÊýÖÐÉú³É¹«Ô¿¡¢Ë½Ô¿Ö¸Êý
=======
{//ä»Žä¸€ä¸ªæ¬§æ‹‰æ•°ä¸­ç”Ÿæˆå…¬é’¥ã€ç§é’¥æŒ‡æ•°
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
	//e=5;
	e = 65537;
	_d = e.extendEuclid(ou);
}

BigInt Rsa::encryptByPu(const BigInt& m)
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
{
	//¹«Ô¿¼ÓÃÜ
	return m.moden(e, N);
}

BigInt Rsa::decodeByPr(const BigInt& c)
{
	//Ë½Ô¿½âÃÜ
	return c.moden(_d, N);
}

BigInt Rsa::encryptByPr(const BigInt& m)
{
	//Ë½Ô¿¼ÓÃÜ
=======
{//å…¬é’¥åŠ å¯†
	return m.moden(e,N);
}

BigInt Rsa::decodeByPr(const BigInt& c)
{//ç§é’¥è§£å¯†
	return c.moden(_d,N);
}

BigInt Rsa::encryptByPr(const BigInt& m)
{//ç§é’¥åŠ å¯†
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
	return decodeByPr(m);
}

BigInt Rsa::decodeByPu(const BigInt& c)
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
{
	//¹«Ô¿½âÃÜ
=======
{//å…¬é’¥è§£å¯†
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
	return encryptByPu(c);
}
