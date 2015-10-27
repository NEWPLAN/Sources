#include <iostream>

using namespace std;

class big_num
{
private:
	long long low,high;
	void init(long long ins)
	{
		this->low=ins&0x0000ffff;
		this->high=(ins&0xffff0000)>>32;
	}
	
public:
	big_num(long long ins=0):low(0),high(0)
	{
		this->init(ins);
	}
	
	void set_bignum(long long ins=0)
	{
		this->init(ins);
	}
	friend bool operator >(big_num &n_a,big_num &n_b);
	friend big_num operator+(big_num &n_a,big_num &n_b);
};
bool operator >(big_num &n_a,big_num &n_b)
{
	if(n_a.high>n_b.high)
		return true;
	if(n_a.high<n_b.high)
		return false;
	if(n_a.low>n_b.low)
		return true;
	return false;
}


big_num operator+(big_num &n_a,big_num &n_b)
{
	big_num t;
	t.low=n_a.low+n_b.low;
	t.high=n_a.high+n_b.high+((t.low&0xffff0000)>>32);
	t.low=t.low&0x0000ffff;
	return t;
}

int main(int argc,char** argv)
{
	long long a,b,c;
	int cc;
	cin>>cc;
	int pp=cc;
	while(cc--)
	{
		cin>>a>>b>>c;
		
		big_num ba(a),bb(b),bc(c),bd;
		
		bd=ba+bb;
		
		cout<<"Case #"<<pp-cc;
		if(bd>bc)
			cout<<": true"<<endl;
		else
			cout<<": false"<<endl;
		
	}
	return 0;
}