#include<iostream>
using namespace std;
int add_mod(int a, int b, int n){
  a %= n; b %= n;
  return (a + b) % n;
}
int sub_mod(int a, int b, int n){
  a %= n; b %= n;
  return (a + b + n) % n;
}
int div_mod(int a, int b, int n){//�������ֿܷ���
  return (a/b) % n;
}
int mul_mod(int a, int b, int n){
  a %= n; b %= n;
  return (int)((long long)a * b % n); //�ر�ע��int*int���ܻ���� 
}  
int main(){
  int a, b, n;
  cin >> a >> b >> n;
  cout << add_mod(a, b, n) << endl;
  cout << sub_mod(a, b, n) << endl;
  cout << div_mod(a, b, n) << endl;
  cout << mul_mod(a, b, n) << endl;
  while(1);
  return 0;
}  
  
