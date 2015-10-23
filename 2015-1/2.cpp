#include <iostream>
#include <algorithm>

using namespace std;

typedef struct nu
{
    int index;
    int counts;
}NUMBER,*nuptr;


int comp(const void *A,const void *B)
{
    int a=(*(nuptr)A).counts;
    int b=(*(nuptr)B).counts;
    if(b==a)
        return (*(nuptr)A).index-(*(nuptr)B).index;
    return b-a;
}


int main(void)
{
    nu A[1001];
    int c=0,k;
    for(int i=0;i<=1000;i++)
    {
        A[i].index=i;
        A[i].counts=0;
    }

    cin>>c;
    while(c--)
    {
        cin>>k;
        A[k].index=k;
        A[k].counts++;
    }
    qsort((void*)A,1001,sizeof(A[0]),comp);
    for(int i=0;i<=1000;i++)
        if(A[i].counts)
            cout<<A[i].index<<" "<<A[i].counts<<endl;
    return 0;
}
