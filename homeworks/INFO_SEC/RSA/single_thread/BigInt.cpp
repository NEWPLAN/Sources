#include "BigInt.h"
#include <cassert>

int BigInt::base_char = 8;
int BigInt::base = 0xFFFFFFFF;
int BigInt::basebit = 5; //2^5
int BigInt::basebitchar = 0x1F;
int BigInt::basebitnum = 32;
BigInt BigInt::Zero(0);
BigInt BigInt::One(1);
BigInt BigInt::Two(2);

BigInt operator + (const BigInt& a, const BigInt& b)
{
	BigInt ca(a);
	return ca.add(b);
}

BigInt operator - (const BigInt& a, const BigInt& b)
{
	BigInt ca(a);
	return ca.sub(b);
}

BigInt operator * (const BigInt& a, const BigInt& b)
{
	if (a == (BigInt::Zero) || b == (BigInt::Zero))
		return BigInt::Zero;

	const BigInt &big = a._data.size() > b._data.size() ? a : b;
	const BigInt &small = (&big) == (&a) ? b : a;

	BigInt result(0);

	BigInt::bit bt(small);
	for (int i = bt.size() - 1; i >= 0; --i)
	{
		if (bt.at(i))
		{
			BigInt temp(big, false);
			temp.leftShift(i);
			//std::cout<<"tmp:"<<temp<<std::endl;
			result.add(temp);
			//std::cout<<"res:"<<result<<std::endl;
		}
	}
	result._isnegative = !(a._isnegative == b._isnegative);
	return result;
}

BigInt operator / (const BigInt& a, const BigInt& b)
{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
	assert(b != (BigInt::Zero));
	if (a.equals(b)) //æ¯∂‘÷µœ‡µ»
		return (a._isnegative == b._isnegative) ? BigInt(1) : BigInt(-1);
	else if (a.smallThan(b)) //æ¯∂‘÷µ–°”⁄
=======
	assert(b!=(BigInt::Zero));
	if(a.equals(b))//ÁªùÂØπÂÄºÁõ∏Á≠â
		return (a._isnegative==b._isnegative)?BigInt(1):BigInt(-1);
	else if(a.smallThan(b))//ÁªùÂØπÂÄºÂ∞è‰∫é
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
		return BigInt::Zero;
	else
	{
		BigInt result, ca;
		BigInt::div(a, b, result, ca);
		return result;
	}
}

BigInt operator % (const BigInt& a, const BigInt& b)
{
	assert(b != (BigInt::Zero));
	if (a.equals(b))
		return BigInt::Zero;
	else if (a.smallThan(b))
		return a;
	else
	{
		BigInt result, ca;
		BigInt::div(a, b, result, ca);
		return ca;
	}
}

void BigInt::div(const BigInt& a, const BigInt& b, BigInt& result, BigInt& ca)
{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
	//1.∏¥÷∆a,b
	BigInt cb(b, false);
	ca._isnegative = false;
	ca._data = a._data;

	BigInt::bit bit_b(cb);
	//Œª ˝∂‘∆Î
	while (true) //æ¯∂‘÷µ–°”⁄
=======
	//1.Â§çÂà∂a,b
	BigInt cb(b,false);
	ca._isnegative=false;
	ca._data=a._data;

	BigInt::bit bit_b(cb);
	//‰ΩçÊï∞ÂØπÈΩê
	while(true)//ÁªùÂØπÂÄºÂ∞è‰∫é
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
	{
		BigInt::bit bit_a(ca);
		int len = bit_a.size() - bit_b.size();
		BigInt temp;
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
		//’“µΩ“∆Œªµƒ
		while (len >= 0)
=======
		//ÊâæÂà∞Áßª‰ΩçÁöÑ
		while(len>=0)
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
		{
			temp = cb << len;
			if (temp.smallOrEquals(ca))
				break;
			--len;
		}
		if (len < 0)
			break;
		BigInt::base_t n = 0;
		while (temp.smallOrEquals(ca))
		{
			ca.sub(temp);
			++n;
		}
		BigInt kk(n);
		if (len)
			kk.leftShift(len);
		result.add(kk);
	}
	result.trim();
}

bool operator < (const BigInt& a, const BigInt& b)
{
	if (a._isnegative == b._isnegative)
	{
		if (a._isnegative == false)
			return a.smallThan(b);
		else
			return !(a.smallOrEquals(b));
	}
	else
	{
		if (a._isnegative == false)
			return true;
		else
			return false;
	}
}

bool operator <= (const BigInt& a, const BigInt& b)
{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
	if (a._isnegative == b._isnegative)
	{
		//Õ¨∫≈
		if (a._isnegative == false)
=======
	if(a._isnegative==b._isnegative)
	{//ÂêåÂè∑
		if(a._isnegative==false)
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
			return a.smallOrEquals(b);
		else
			return !(a.smallThan(b));
	}
	else//ÂºÇÂè∑
	{
		if (a._isnegative == false)
			return true;
		else
			return false;
	}
}

bool operator == (const BigInt& a, const BigInt& b)
{
	return a._data == b._data && a._isnegative == b._isnegative;
}

ostream& operator << (ostream& out, const BigInt& a)
{
	static char hex[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	if (a._isnegative)
		out << "-";
	BigInt::base_t T = 0x0F;
	std::string str;
	for (BigInt::data_t::const_iterator it = a._data.begin(); it != a._data.end(); ++it)
	{
		BigInt::base_t ch = (*it);
		for (int j = 0; j < BigInt::base_char; ++j)
		{
			str.push_back(hex[ch & (T)]);
			ch = ch >> 4;
		}
	}
	reverse(str.begin(), str.end());
	out << str;
	return out;
}

BigInt operator <<(const BigInt& a, unsigned int n)
{
	BigInt ca(a);
	return ca.leftShift(n);
}

BigInt& BigInt::leftShift(const unsigned int n)
{
	int k = n >> (BigInt::basebit); //5
	int off = n & (BigInt::basebitchar); //0xFF

	int inc = (off == 0) ? k : 1 + k;
	for (int i = 0; i < inc; ++i)
		_data.push_back(0);

	if (k)
	{
		inc = (off == 0) ? 1 : 2;
		for (int i = _data.size() - inc; i >= k; --i)
			_data[i] = _data[i - k];
		for (int i = 0; i < k; ++i)
			_data[i] = 0;
	}

	if (off)
	{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
		BigInt::base_t T = BigInt::base; //0xffffffff
		T = T << (BigInt::basebitnum - off); //32
		//◊Û“∆
		BigInt::base_t ch = 0;
		for (std::size_t i = 0; i < _data.size(); ++i)
		{
			BigInt::base_t t = _data[i];
			_data[i] = (t << off) | ch;
			ch = (t & T) >> (BigInt::basebitnum - off); //32,◊Ó∏ﬂŒª
=======
		BigInt::base_t T=BigInt::base;//0xffffffff
		T=T<<(BigInt::basebitnum-off);//32
		//Â∑¶Áßª
		BigInt::base_t ch=0;
		for(std::size_t i=0;i<_data.size();++i)
		{
			BigInt::base_t t=_data[i];
			_data[i]=(t<<off)|ch;
			ch=(t&T)>>(BigInt::basebitnum-off);//32,ÊúÄÈ´ò‰Ωç
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
		}
	}
	trim();
	return *this;
}

BigInt& BigInt::rightShift(const unsigned int n)
{
	int k = n >> (BigInt::basebit); //5
	int off = n & (BigInt::basebitchar); //0xFF

	if (k)
	{
		for (int i = 0; i > k; ++i)
			_data[i] = _data[i + k];
		for (int i = 0; i < k; ++i)
			_data.pop_back();
		if (_data.size() == 0)
			_data.push_back(0);
	}

	if (off)
	{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
		BigInt::base_t T = BigInt::base; //0xFFFFFFFF
		T = T >> (BigInt::basebitnum - off); //32
		//◊Û“∆
		BigInt::base_t ch = 0;
		for (int i = _data.size() - 1; i >= 0; --i)
		{
			BigInt::base_t t = _data[i];
			_data[i] = (t >> off) | ch;
			ch = (t & T) << (BigInt::basebitnum - off); //32,◊Ó∏ﬂŒª
=======
		BigInt::base_t T=BigInt::base;//0xFFFFFFFF
		T=T>>(BigInt::basebitnum-off);//32
		//Â∑¶Áßª
		BigInt::base_t ch=0;
		for(int i=_data.size()-1;i>=0;--i)
		{
			BigInt::base_t t=_data[i];
			_data[i]=(t>>off)|ch;
			ch=(t&T)<<(BigInt::basebitnum-off);//32,ÊúÄÈ´ò‰Ωç
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
		}
	}
	trim();
	return *this;
}

BigInt& BigInt::sub(const BigInt& b)
{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
	if (b._isnegative == _isnegative)
	{
		//Õ¨∫≈

		BigInt::data_t &res = _data;
		if (!(smallThan(b))) //æ¯∂‘÷µ¥Û”⁄b
		{
			int cn = 0; //ΩËŒª
			//¥Û ˝ºı–° ˝
			for (std::size_t i = 0; i < b._data.size(); ++i)
=======
	if(b._isnegative==_isnegative)
	{//ÂêåÂè∑
		
		BigInt::data_t &res=_data;
		if(!(smallThan(b)))//ÁªùÂØπÂÄºÂ§ß‰∫éb
		{
			int cn=0;//ÂÄü‰Ωç
			//Â§ßÊï∞ÂáèÂ∞èÊï∞
			for(std::size_t i=0;i<b._data.size();++i)
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
			{
				BigInt::base_t temp = res[i];
				res[i] = (res[i] - b._data[i] - cn);
				cn = temp < res[i] ? 1 : temp < b._data[i] ? 1 : 0;
			}

			for (size_t i = b._data.size(); i < _data.size() && cn != 0; ++i)
			{
				BigInt::base_t temp = res[i];
				res[i] = res[i] - cn;
				cn = temp < cn;
			}
			trim();
		}
		else//ÁªùÂØπÂÄºÂ∞è‰∫éb
		{
			_data = (b - (*this))._data;
			_isnegative = !_isnegative;
		}
	}
	else
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
	{
		//“Ï∫≈µƒ«Èøˆ
		bool isnegative = _isnegative;
		_isnegative = b._isnegative;
=======
	{//ÂºÇÂè∑ÁöÑÊÉÖÂÜµ
		bool isnegative=_isnegative;
		_isnegative=b._isnegative;
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
		add(b);
		_isnegative = isnegative;
	}
	return *this;
}

BigInt& BigInt::add(const BigInt& b)
{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
	if (_isnegative == b._isnegative)
	{
		//Õ¨∫≈
		//“˝”√
		BigInt::data_t &res = _data;
		int len = b._data.size() - _data.size();

		while ((len--) > 0) //∏ﬂŒª≤π0
			res.push_back(0);

		int cn = 0; //Ω¯Œª
		for (std::size_t i = 0; i < b._data.size(); ++i)
=======
	if(_isnegative==b._isnegative)
	{//ÂêåÂè∑
		//ÂºïÁî®
		BigInt::data_t &res=_data;
		int len=b._data.size()-_data.size();

		while((len--)>0)//È´ò‰ΩçË°•0
			res.push_back(0);

		int cn=0;//Ëøõ‰Ωç
		for(std::size_t i=0;i<b._data.size();++i)
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
		{
			BigInt::base_t temp = res[i];
			res[i] = res[i] + b._data[i] + cn;
			cn = temp > res[i] ? 1 : temp > (temp + b._data[i]) ? 1 : 0; //0xFFFFFFFF
		}

		for (std::size_t i = b._data.size(); i < _data.size() && cn != 0; ++i)
		{
			BigInt::base_t temp = res[i];
			res[i] = (res[i] + cn);
			cn = temp > res[i];
		}

		if (cn != 0)
			res.push_back(cn);

		trim();
	}
	else
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
	{
		//“Ï∫≈µƒ«Èøˆ
		bool isnegative;
		if (smallThan(b)) //æ¯∂‘÷µ–°”⁄b
			isnegative = b._isnegative;
		else if (equals(b)) //æ¯∂‘÷µµ»”⁄b
			isnegative = false;
		else//æ¯∂‘÷µ¥Û”⁄b
			isnegative = _isnegative;
=======
	{//ÂºÇÂè∑ÁöÑÊÉÖÂÜµ
		bool isnegative;
		if(smallThan(b))//ÁªùÂØπÂÄºÂ∞è‰∫éb
			isnegative=b._isnegative;
		else if(equals(b))//ÁªùÂØπÂÄºÁ≠â‰∫éb
			isnegative=false;
		else//ÁªùÂØπÂÄºÂ§ß‰∫éb
			isnegative=_isnegative;
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp

		_isnegative = b._isnegative;
		sub(b);
		_isnegative = isnegative;
	}
	return *this;
}

<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
BigInt BigInt::moden(const BigInt& exp, const BigInt& p)const
{
	//ƒ£√›‘ÀÀ„
=======
BigInt BigInt::moden(const BigInt& exp,const BigInt& p)const
{//Ê®°ÂπÇËøêÁÆó
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
	BigInt::bit t(exp);

	BigInt d(1);
	for (int i = t.size() - 1; i >= 0; --i)
	{
		d = (d * d) % p;
		if (t.at(i))
			d = (d * (*this)) % p;
	}
	return d;
}

BigInt BigInt::extendEuclid(const BigInt& m)
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
{
	//¿©’π≈∑º∏¿Ôµ√À„∑®«Û≥À∑®ƒÊ‘™
	assert(m._isnegative == false); //mŒ™’˝ ˝
	BigInt a[3], b[3], t[3];
=======
{//Êâ©Â±ïÊ¨ßÂá†ÈáåÂæóÁÆóÊ≥ïÊ±Ç‰πòÊ≥ïÈÄÜÂÖÉ
	assert(m._isnegative==false);//m‰∏∫Ê≠£Êï∞
	BigInt a[3],b[3],t[3];
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
	a[0] = 1; a[1] = 0; a[2] = m;
	b[0] = 0; b[1] = 1; b[2] = *this;
	if (b[2] == BigInt::Zero || b[2] == BigInt::One)
		return b[2];

	while (true)
	{
		if (b[2] == BigInt::One)
		{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
			if (b[1]._isnegative == true) //∏∫ ˝
				b[1] = (b[1] % m + m) % m;
=======
			if(b[1]._isnegative==true)//Ë¥üÊï∞
				b[1]=(b[1]%m+m)%m;
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
			return b[1];
		}

		BigInt q = a[2] / b[2];
		for (int i = 0; i < 3; ++i)
		{
			t[i] = a[i] - q * b[i];
			a[i] = b[i];
			b[i] = t[i];
		}
	}
}

std::size_t BigInt::bit::size()
{
	return _size;
}

bool BigInt::bit::at(size_t i)
{
	std::size_t index = i >> (BigInt::basebit);
	std::size_t off = i & (BigInt::basebitchar);
	BigInt::base_t t = _bitvec[index];
	return (t & (1 << off));
}

BigInt::bit::bit(const BigInt& ba)
{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
	_bitvec = ba._data;
	BigInt::base_t a = _bitvec[_bitvec.size() - 1]; //◊Ó∏ﬂŒª
	_size = _bitvec.size() << (BigInt::basebit);
	BigInt::base_t t = 1 << (BigInt::basebitnum - 1);

	if (a == 0)
		_size -= (BigInt::basebitnum);
=======
	_bitvec=ba._data;
	BigInt::base_t a=_bitvec[_bitvec.size()-1];//ÊúÄÈ´ò‰Ωç
	_size=_bitvec.size()<<(BigInt::basebit);
	BigInt::base_t t=1<<(BigInt::basebitnum-1);
	
	if(a==0)
		_size-=(BigInt::basebitnum);
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
	else
	{
		while (!(a & t))
		{
			--_size;
			t = t >> 1;
		}
	}
}

bool BigInt::smallThan(const BigInt& b)const
{
	if (_data.size() == b._data.size())
	{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
		for (BigInt::data_t::const_reverse_iterator it = _data.rbegin(), it_b = b._data.rbegin();
		        it != _data.rend(); ++it, ++it_b)
			if ((*it) != (*it_b))
				return (*it) < (*it_b);
		return false;//œ‡µ»
=======
		for(BigInt::data_t::const_reverse_iterator it=_data.rbegin(),it_b=b._data.rbegin();
			it!=_data.rend();++it,++it_b)
			if((*it)!=(*it_b))
				return (*it)<(*it_b);
		return false;//Áõ∏Á≠â
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
	}
	else
		return _data.size() < b._data.size();
}

bool BigInt::smallOrEquals(const BigInt& b)const
{
	if (_data.size() == b._data.size())
	{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
		for (BigInt::data_t::const_reverse_iterator it = _data.rbegin(), it_b = b._data.rbegin();
		        it != _data.rend(); ++it, ++it_b)
			if ((*it) != (*it_b))
				return (*it) < (*it_b);
		return true;//œ‡µ»
=======
		for(BigInt::data_t::const_reverse_iterator it=_data.rbegin(),it_b=b._data.rbegin();
			it!=_data.rend();++it,++it_b)
			if((*it)!=(*it_b))
				return (*it)<(*it_b);
		return true;//Áõ∏Á≠â
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
	}
	else
		return _data.size() < b._data.size();
}

bool BigInt::equals(const BigInt& a)const
{
<<<<<<< HEAD:homeworks/INFO_SEC/RSA/BigInt.cpp
	return _data == a._data;
}
=======
	return _data==a._data;
}
>>>>>>> df09c67d6239f0e687713439c6a18c966ee9bbcc:homeworks/INFO_SEC/RSA/single_thread/BigInt.cpp
