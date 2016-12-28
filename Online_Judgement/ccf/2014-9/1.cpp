#include <iostream>

using namespace std;

int A[1001];
int main(void)
{
    int allcount=0;
    int i=0;
    cin>>i;
    for(int j=0;j<i;j++)
        cin>>A[j];
    for(int j=0;j<i-1;j++)
        for(int k=j+1;k<i;k++)
            if(A[j]-A[k]==1||A[j]-A[k]==-1)
                allcount++;
    cout<<allcount<<endl;
}
