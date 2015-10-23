#include <iostream>
#include<cstdio>

using namespace std;



int partitions(int* A,int from,int end)
{
    int p=A[end];

    int i=from-1;

    for(int j=from;j<end-1;j++)
    {
        if(A[j]<=p)
        {
            i++;
            int temp=A[i];
            A[i]=A[j];
            A[j]=temp;
        }
    }
    int temp=A[i+1];
    A[i+1]=A[end];
    A[end]=temp;

    return i+1;

}
void quick_sort(int* A,int from,int end)
{
    if(from>=end)
        return ;
    int mid=partitions(A,from,end);
    quick_sort(A,from,mid-1);
    quick_sort(A,mid+1,end);
}

int main(void)
{
    int A[10]={0,2,1,4,54,67,3,11,32};
    for(int i=0;i<10;i++)
        printf("%d\t",A[i]);
        printf("\n");
    quick_sort(A,0,9);
    for(int i=0;i<10;i++)
        printf("%d\t",A[i]);
    return 0;
}
