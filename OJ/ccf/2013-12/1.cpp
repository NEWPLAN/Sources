#include <iostream>

using namespace std;

int main(void)
{
    int A[1001]={0};
    int a=0;
    int t=0;
    cin>>a;
    while(a--)
    {
        cin>>t;
        A[t]++;
    }

    int i=1;
    for(int j=0;j<=1000;j++)
        if(A[i]<A[j])
            i=j;

    cout<<i<<endl;
    return 0;
}
