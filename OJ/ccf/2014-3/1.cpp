#include <iostream>

using namespace std;

int A[500];
int main(void)
{
    int i=0;
    cin>>i;
    for(int j=0;j<i;j++)
    {
        cin>>A[j];
    }
    int  cc=0;
    for(int k=0;k<i;k++)
        for(int j=k+1;j<i;j++)
            if(0==A[k]+A[j])
                cc++;
    cout<<cc<<endl;
    return 0;
}
