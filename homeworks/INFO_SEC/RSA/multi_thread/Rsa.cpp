#include "Rsa.h"
#include<iostream>
#include<sstream>
#include<ctime>
#include<cstdlib>
#include <thread>

using namespace std;

Rsa::Rsa()
{
	long prims[168] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
		101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,
		211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,
		307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,
		401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,
		503,509,521,523,541,547,557,563,569,571,577,587,593,599,
		601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,
		701,709,719,727,733,739,743,751,757,761,769,773,787,797,
		809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,
		907,911,919,929,937,941,947,953,967,971,977,983,991,997 };
	for (int index = 0; index < 168; index++)
		this->below_1000[index] = BigInt(prims[index]);
}

Rsa::~Rsa()
{}

void Rsa::init(unsigned int n)
{
	srand(time(NULL));
	//����������p��q
	_p = createPrime(n, 10);
	_q = createPrime(n, 10);
	//����N
	N = _p * _q;
	//�����ŷ����
	_ol = (_p - 1) * (_q - 1);
	//����ָ��e
	createExp(_ol);
	//d
}

BigInt Rsa::createOddNum(unsigned int n)
{
	//���ɳ���Ϊn������
	n = n / 4;
	static unsigned char hex_table[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	if (n)
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

bool Rsa::isPrime(const BigInt& n, const unsigned int k)
{
	//�ж�����
	assert(n != BigInt::Zero);
	if (n == BigInt::Two) return true;

	//�Ƚ���Ԥ�жϣ��ų���1-1000�����е�����
	for (int index = 0; index < 168; index++)
	{
		if (n%this->below_1000[index] == BigInt::Zero)
		{
			//cout <<n<< " is not prime return" << endl;
			return false;
		}
	}

	BigInt n_1 = n - 1;
	BigInt::bit b(n_1);//������λ
	if (b.at(0) == 1)
		return false;

	for (size_t t = 0; t < k; ++t)
	{
		BigInt a = createRandomSmallThan(n_1); //�����
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
#include<process.h>
#include<chrono>
#include<mutex>
std::mutex mtx;
bool Rsa::enchance_isPrime(const BigInt* a, const unsigned int k, int* flag)
{
	for (int index = 0;; index++)
	{
		//cout << index<<"\t" << this_thread::get_id()<<'\t'<<flag[index]<< endl;
		index %= 10;
		if (flag[index] == 1)//1 ����
		{
			cout << "find a prime :\t" << a[index] << endl;
			this_thread::sleep_for(chrono::microseconds(1));
			return 0;
		}
		if (flag[index] == 0)//0 ��ʾû�н����ж�
		{
			mtx.lock();
			flag[index] = 2;//2��ʾ�Ѿ����ж�
			mtx.unlock();
			if (!isPrime(a[index], k))
			{
				mtx.lock();
				flag[index] = -1;//-1��ʾ��������
				mtx.unlock();
				//cout << "is not prime\t" /*<<a[index]*/<< endl;
			}
			else//find prime
			{
				mtx.lock();
				flag[index] = 1;
				mtx.unlock();
				cout << "find a prime :\t" << a[index] << endl;
				this_thread::sleep_for(chrono::microseconds(1));
				return 0;
			}
		}
	}
}


BigInt Rsa::createPrime(unsigned int n, int it_count)
{
	//���ɳ���Ϊn������
	assert(it_count > 0);
	BigInt res = createOddNum(n);
	BigInt thread_res[10] = { res };

	{
		int index = 0;
		do
		{
			thread_res[index++] = res;
			res.add(BigInt::Two);
		} while (index<10);
	}
	int flags[10] = { 0 };
	thread th[10];
	for (int index = 0; index < 10; index++)
	{
		//cout << thread_res[index] << endl;
		th[index] = thread(&Rsa::enchance_isPrime,this,thread_res,it_count,flags);
	}
	

	for (int index=0;; index++)
	{
		index %= 10;
		//cout <<index<<'\t'<< flags[index] << endl;
		if (flags[index] == 1)
		{
			//break;
			//cout << "come here and will return now" << endl;
			//cout << "index= " << index << '\t' << thread_res[index] << endl;
			//return thread_res[index];
			for (int index2 = 0; index2 < 10; index2++)
			{
				th[index2].join();
			}
			//cout << "index= "<<index<<'\t' << thread_res[index] << endl;
			return thread_res[index];
		}
		if (flags[index] == -1)//update and re_run
		{
			mtx.lock();
			thread_res[index] = res;
			res.add(BigInt::Two);
			flags[index] = 0;
			mtx.unlock();
		}
	}
	
	while (!isPrime(res, it_count))
	{
		res.add(BigInt::Two);
	}
	return res;
}

void Rsa::createExp(const BigInt& ou)
{
	//��һ��ŷ���������ɹ�Կ��˽Կָ��
	//e=5;
	e = 65537;
	_d = e.extendEuclid(ou);
}

BigInt Rsa::encryptByPu(const BigInt& m)
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
	return decodeByPr(m);
}

BigInt Rsa::decodeByPu(const BigInt& c)
{
	//��Կ����
	return encryptByPu(c);
}