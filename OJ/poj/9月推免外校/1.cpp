#include <iostream>

using namespace std;

int main(void)
{
    int a=0;
    int pp[10]={0};
    for(int i=0;i<10;i++)
        cin>>pp[i];
    int tt=0;
    cin>>tt;
    for(int i=0;i<10;i++)
        if(tt+30>=pp[i])
            a++;

    cout<<a<<endl;
    return 0;

}
