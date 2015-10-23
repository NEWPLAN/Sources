#include <iostream>
using namespace std;
const int MOD=1000000007;
//¼ÆËãa*b mod n, O(log b)
int modmul(int a,int b,int n)
{
    int ret=0, tmp=a%n;
    b%=n;
    while(b)
    {
        if(b&0x1) ret=(ret+tmp)%n;
        tmp=(tmp+tmp)%n;
        b>>=1;
    }
    return ret;
}
//¼ÆËãa^b mod n, O(log b)
int modexp(int a,int b,int n)
{
    int ret=1, tmp=a%n;
    while(b)
    {
        if(b&0x1) ret=modmul(ret,tmp,n);
        tmp=modmul(tmp,tmp,n);
        b>>=1;
    }
    return ret;
}
int main()
{
    int n;
    cin>>n;
// f(n)=(n^2-5n+4)*2^(n-3)+n-1
    cout<<(modmul((modmul(n,n,MOD)-modmul(5,n,MOD)+4),modexp(2,n-3,MOD),MOD)+n-1)%MOD<<endl;
    return 0;
}
