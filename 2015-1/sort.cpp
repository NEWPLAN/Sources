#include <iostream>
#include <algorithm>

using namespace std;

int main(void)
{
    int A[19]={1,3,6,9,0,4,2,1,4,6,7,8,44,33};
    sort(A,A+8);
    for(int i=0;i<19;i++)
        cout<<A[i]<<endl;
    return 0;
}
