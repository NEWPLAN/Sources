#include <iostream>

using namespace std;

int A[1001][1001];

int main(void)
{
    int raw,col;
    cin>>raw>>col;
    for(int i=0;i<raw;i++)
        for(int j=0;j<col;j++)
            cin>>A[i][j];
    for(int j=col-1;j>=0;j--)
    {
        for(int i=0;i<raw;i++)
            cout<<A[i][j]<<" ";
        cout<<endl;
    }


    return 0;
}
