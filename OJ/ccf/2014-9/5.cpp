#include <iostream>

using namespace std;

int indexs(int m,int n)
{
    if(m<=2)
        return 0;
    if(m==3)
        return 2;
    return 2*indexs(m-3,n);
}

int indexs2(int m,int n)
{
    if(m<2)
        return 0;
    if(m==2)
        return 2;
    return 2*indexs(m-2,n);
}

int main(void)
{
    int m,n;
    cin>>m>>n;
    int nummax,nummin;
    if(m>n)
    {
        nummax=m;nummin=n;
    }

    else
    {
        nummax=n;nummin=m;
    }
    if(nummin==2)
        cout<<indexs(nummax,nummin)<<endl;
    if(nummin==4)
    {
        int s=indexs(nummax,nummin);
        cout<<s*s<<endl;
    }
    if(nummin==3)
        cout<<indexs2(nummax,nummin)<<endl;
    return 0;

}
