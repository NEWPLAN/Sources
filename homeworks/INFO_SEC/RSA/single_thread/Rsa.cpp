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
	//����������p��q
	_p = createPrime(n, 10);
	_q = createPrime(n, 10);
	//����N
	N = _p * _q;
	//�����ŷ����
	_ol = (_p - 1) * (_q - 1);
	//����ָ��e
=======
	//产生大素数p、q
	_p=createPrime(n,10);
	_q=createPrime(n,10);
	//计算N
	N=_p*_q;
	//计算出欧拉数
	_ol=(_p-1)*(_q-1);
	//加密指数e
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
	createExp(_ol);
	//d
}

BigInt Rsa::createOddNum(unsigned int n)
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
{
	//���ɳ���Ϊn������
	n = n / 4;
	static unsigned char hex_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	if (n)
=======
{//生成长度为n的奇数
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
	//�ж�����
	assert(n != BigInt::Zero);
	if (n == BigInt::Two)
		return true;

	BigInt n_1 = n - 1;
	BigInt::bit b(n_1);//������λ
	if (b.at(0) == 1)
=======
bool Rsa::isPrime(const BigInt& n,const unsigned int k)
{//判断素数
	assert(n!=BigInt::Zero);
	if(n==BigInt::Two)
		return true;

	BigInt n_1=n-1;
	BigInt::bit b(n_1);//二进制位
	if(b.at(0)==1)
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
		return false;

	for (std::size_t t = 0; t < k; ++t)
	{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
		BigInt a = createRandomSmallThan(n_1); //�����
=======
		BigInt a=createRandomSmallThan(n_1);//随机数
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
	//���ɳ���Ϊn������
	assert(it_count > 0);
	BigInt res = createOddNum(n);
	while (!isPrime(res, it_count))
=======
BigInt Rsa::createPrime(unsigned int n,int it_count)
{//生成长度为n的素数
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
	//��һ��ŷ���������ɹ�Կ��˽Կָ��
=======
{//从一个欧拉数中生成公钥、私钥指数
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
	//e=5;
	e = 65537;
	_d = e.extendEuclid(ou);
}

BigInt Rsa::encryptByPu(const BigInt& m)
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
{
	//��Կ����
	return m.moden(e, N);
}

BigInt Rsa::decodeByPr(const BigInt& c)
{
	//˽Կ����
	return c.moden(_d, N);
}

BigInt Rsa::encryptByPr(const BigInt& m)
{
	//˽Կ����
=======
{//公钥加密
	return m.moden(e,N);
}

BigInt Rsa::decodeByPr(const BigInt& c)
{//私钥解密
	return c.moden(_d,N);
}

BigInt Rsa::encryptByPr(const BigInt& m)
{//私钥加密
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
	return decodeByPr(m);
}

BigInt Rsa::decodeByPu(const BigInt& c)
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/Rsa.cpp
{
	//��Կ����
=======
{//公钥解密
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/Rsa.cpp
	return encryptByPu(c);
}
