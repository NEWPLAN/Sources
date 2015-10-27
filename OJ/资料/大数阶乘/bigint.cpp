#include <iostream>
#include <sstream>

#define s_int short int
#define MAXDIGIT 50000
#define RADIX 10

using std::cout;
using std::endl;
using std::cin;

//实现进位
bool carry( s_int result[], int &dgts )
{
    int i;
    s_int carry_value = 0;
    for( i = 0; i < dgts; i++ )
    {
		result[i] += carry_value;
		carry_value = ( result[i] < RADIX ) ? 0 : ( result[i] / RADIX );
		result[i] -= carry_value * RADIX;
	}

	//处理最后一位：
	//若需进位，则循环进位，直到不需进位为止
	result[i] += carry_value;
	while( result[i] >= RADIX && i < MAXDIGIT )
	{
		carry_value = result[i] / RADIX;
		result[i] -= carry_value * RADIX;
		result[++i] = carry_value;
		++dgts;
	}
	
	if( i >= MAXDIGIT )
		return false;
	return true;
}
 //
 // The function return the total digits of the result.
 //
 int factorial( int n, s_int result[] )
 {
	memset(result, 0, sizeof(s_int)*MAXDIGIT);
	int digits = 0;
	result[0] = 1;
	
	// 0! = 1
	if( n == 0 ) return 1;
	
	for( int i = 2; i < n+1; ++i )
	{
		for( int j = 0; j <= digits; ++j )
		{
			result[j] *= i;
		}
		if( !carry( result, digits ) )
			break;
	}
	return digits >= MAXDIGIT ? -1 : digits;
}

void print( s_int result[], const int &digits )
{
	if( digits < 10 )
		for( int i = digits; i >= 0; i-- )
			cout << result[i];
	else
	{
		cout << result[digits] <<".";
		for( int i = digits -1; i >= 0; i-- )
			cout << result[i];
		cout << "E" << digits;
	}
	cout << endl;
}
 
 int main(void)
 {
	s_int result[MAXDIGIT];
	int n;
	int digits;
	
	cout << "Input the value of n: ";
	
	cin >> n;
	
	if( n < 0 )
	{
		cout << "Error: A positive integer is need!" << endl;
		return 0;
	}
	
	if( (digits = factorial(n, result)) == -1 )
	{
		cout << "Error: Overflow!" << endl;
		return 0;
	}
	else
	{
		print(result, digits);
		return 0;
	}
}