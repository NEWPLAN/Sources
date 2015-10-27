#include <iostream>
#include <cstdio>

using namespace std;

class strange_num
{
private:
	long long Galleon;
	int Sickle;
	int Knut;

public:
	strange_num():Galleon(0),Sickle(0),Knut(0){}
	strange_num(long long,int, int);
	~strange_num(){};
	friend strange_num operator+( strange_num& aa, strange_num& pp);
	long long get_Galleon()
	{
		return Galleon;
	}
	int get_Sickle()
	{
		return Sickle;
	}
	int get_Knut()
	{
		return Knut;
	}
	
};

strange_num::strange_num(long long Galleons,int Sickles, int Knuts)
{
	int rr=0;
	rr=Knuts/29;
	this->Knut=Knuts%29;

	this->Sickle=(Sickles+rr)%17;
	rr=(Sickles+rr)/17;
	
	this->Galleon=Galleons+rr;
	
}
strange_num operator+( strange_num& aa, strange_num& pp)
{
	return strange_num (aa.Galleon+pp.Galleon,aa.Sickle+pp.Sickle,aa.Knut+pp.Knut);
}

int main(void)
{
	long long a;
	int b,c;
	scanf("%lld.%d.%d",&a,&b,&c);
	strange_num tt(a,b,c);
	scanf("%lld.%d.%d",&a,&b,&c);
	strange_num cc(a,b,c);
	strange_num mm=tt+cc;

	printf("%lld.%d.%d",mm.get_Galleon(),mm.get_Sickle(),mm.get_Knut());
	return 0;
}