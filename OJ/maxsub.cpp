#include <iostream>

using namespace std;

int max_sub2(int a[], int size)
{
    int i,max=0,temp_sum=0;
    for(i=0; i<size; i++)
    {
        temp_sum+=a[i];
        if(temp_sum>max)
            max=temp_sum;
        else if(temp_sum<0)
            temp_sum=0;
    }
    return max;
}

int main(void)
{
    int pp[100000]= {0};
    int n;
    cin>>n;
    int i=0;
    while(i<n)
    {
        cin>>pp[i];
        i++;
    }
    cout<<max_sub2(pp,n);
    return 0;
}
