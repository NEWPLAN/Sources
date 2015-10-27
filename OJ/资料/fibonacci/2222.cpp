#include <iostream>
#include <math.h>
#include <string.h>
using namespace std;
#define repeatTime	50
#define __int32Max 2147483648
//0x7FFFFFFF = 2147483647 = 536870911.75 * 4
#define maxSize 5000000
int F[maxSize];
int ModularFibonacci(int n, int p)
{
	//0, 1, 1, 2, 3, 5, 8, 13, 21, 34, бн
	//0  1  2  3  4  5  6  7   8   9
	int temp;
	if(n < 0)
		return 0;
	if(n < maxSize && F[n] != 0){
		return F[n];
	}
	if(n % 2 == 0){
		temp = ((ModularFibonacci(n / 2 - 1, p) * ModularFibonacci(n / 2 + 1, p)) + (ModularFibonacci(n / 2 - 2, p) * ModularFibonacci(n / 2, p))) % p;
	}else{
		temp = ((ModularFibonacci(n / 2, p) * ModularFibonacci(n / 2 + 1, p)) + (ModularFibonacci(n / 2 - 1, p) * ModularFibonacci(n / 2, p))) % p;
	}
	if(n < maxSize){
		F[n] = temp;
	}
	return temp;
}
int main()
{
	int n;
	memset(F, 0, sizeof(F));
	F[0] = 1;
	F[1] = 1;
	F[2] = 2;
	F[3] = 3;
	F[4] = 5;
	while(cin >> n && n != -1){
		cout<< ModularFibonacci(n - 1, 10000) << endl;
	}
	return 1;
}
