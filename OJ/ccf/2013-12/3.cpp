#include <iostream>
#include <string>
#include <time.h>
using namespace std;
int a[1001];
int main()
{
    int n,max = 0;
    cin>>n;
    for(int i=0; i<n; i++)
    {
        cin>>a[i];
    }
    max = a[0];
    for(int i=1; i<n; i++)
    {
        int now = a[i];
        //往前走
        for(int j=i-1; j>=0; j--)
        {
            if(a[i]<=a[j])
                now+=a[i];
            else
                break;
        }
        //往后走
        for(int j=i+1; j<n; j++)
        {
            if(a[i]<=a[j])
                now+=a[i];
            else
            break;
        }
        if(now>max)
        {
            max = now;
        }
    }
    cout<<max;
    return 0;
}
