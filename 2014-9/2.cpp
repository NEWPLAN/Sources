#include <iostream>

using namespace std;

int arrays[101][101];
int main(void)
{
    int n=0;
    cin>>n;
    int allcount=0;
    int row1=0,cow1=0,row2=0,cow2=0;
    while(n--)
    {
        cin>>row1>>cow1>>row2>>cow2;
        for(int i=row1; i<row2; i++)
            for(int j=cow1; j<cow2; j++)
                arrays[i][j]=1;
    }
    for(int i=0; i<101; i++)
        for(int j=0; j<101; j++)
            if(arrays[i][j]==1)
                allcount++;

    cout<<allcount<<endl;
    return 0;
}
