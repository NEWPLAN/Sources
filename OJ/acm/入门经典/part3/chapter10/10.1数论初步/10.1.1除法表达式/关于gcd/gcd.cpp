#include<iostream>
#include<cassert>
using namespace std;
int gcd1(const int& a, const int& b){return b?gcd1(b, a%b):a;}
int gcd2(int a, int b){int t; while(b){t=a%b;a=b;b=t;} return a;}
 //���ܰ�t=a%b�������Ϊb������0��a%0��Runtime Error��
int main(){
    int a, b, g1, g2;
    cin >> a >> b;
    cout << (g1 = gcd1(a, b)) << endl;
    cout << (g2 = gcd2(a, b)) << endl;
    cout << "lcm: " << a/g1 * b << endl; //�ȳ��ٳ˷���� 
    assert(g1 == g2);
    return 0;
}
