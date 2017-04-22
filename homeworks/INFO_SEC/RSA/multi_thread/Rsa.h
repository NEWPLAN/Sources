#ifndef __RSA__
#define __RSA__

#include"BigInt.h"
class Rsa
{
public:
	Rsa();
	~Rsa();
	void init(unsigned int n);//��ʼ����������˽Կ��

	friend void test();
public:
	BigInt encryptByPu(const BigInt& m);//˽Կ����
	BigInt decodeByPr(const BigInt& c);//��Կ����

	BigInt encryptByPr(const BigInt& m);//��Կ����
	BigInt decodeByPu(const BigInt& m);//˽Կ����
private:
	BigInt createOddNum(unsigned int n);//���ɳ���Ϊn������
	bool isPrime(const BigInt& a,const unsigned int k);//�ж�����
	bool enchance_isPrime(const BigInt* a, const unsigned int k,int* flag);//�ж�����
	BigInt createPrime(unsigned int n,int it_cout);//���ɳ���Ϊn������
	void createExp(const BigInt& ou);//��һ��ŷ���������ɹ�Կ��˽Կָ��
	BigInt createRandomSmallThan(const BigInt& a);//����С��
	friend ostream& operator <<(ostream& out,const Rsa& rsa)//���
	{
		out<<"N:\t"<<rsa.N<<"\n";
		out<<"p:\t"<<rsa._p<<"\n";
		out<<"q:\t"<<rsa._q<<"\n";
		out<<"e:\t"<<rsa.e<<"\n";
		out<<"d:\t"<<rsa._d;
		return out;
	}
public:
	BigInt e,N;//��Կ��e��������N=p*q
private:
	BigInt _d;//˽Կ(˽Կ)
	BigInt _p,_q;//���ɵĴ�����������N��_ol
	BigInt _ol;//ŷ����_ol(N)=(p-1)(q-1)
private:
	BigInt below_1000[168];
};
#endif