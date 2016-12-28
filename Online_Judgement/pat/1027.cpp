#include <iostream>

using namespace std;

int main(void)
{
	char MARS[13]={'0','1','2','3','4','5','6','7','8','9','A','B','C'};
	
	int N1,N2,N3;
	cin>>N1>>N2>>N3;
	int divs,modle;
	cout<<"#";
	divs=N1/13;
	modle=N1%13;
	cout<<MARS[divs]<<MARS[modle];
	divs=N2/13;
	modle=N2%13;
	cout<<MARS[divs]<<MARS[modle];
	divs=N3/13;
	modle=N3%13;
	cout<<MARS[divs]<<MARS[modle];
	return 0;
}