/**��ѯ������ֵ�±�---min**/
#include<iostream>    
  
using namespace std;    
    
#define MAXN 100    
#define MAXIND 256 //�߶����ڵ����    
    
//�����߶���,Ŀ��:�õ�M����.    
void build(int node, int b, int e, int M[], int A[])    
{    
    if (b == e)    
        M[node] = b; //ֻ��һ��Ԫ��,ֻ��һ���±�    
    else    
    {     
        build(2 * node, b, (b + e) / 2, M, A);    
        build(2 * node + 1, (b + e) / 2 + 1, e, M, A);    
  
        if (A[M[2 * node]] <= A[M[2 * node + 1]])    
            M[node] = M[2 * node];    
        else    
            M[node] = M[2 * node + 1];    
    }    
}    
    
//�ҳ����� [i, j] �ϵ���Сֵ������    
int query(int node, int b, int e, int M[], int A[], int i, int j)    
{    
    int p1, p2;    
    
    //��ѯ�����Ҫ�������û�н���    
    if (i > e || j < b)    
        return -1;    
  
    if (b >= i && e <= j)    
        return M[node];    
   
    p1 = query(2 * node, b, (b + e) / 2, M, A, i, j);    
    p2 = query(2 * node + 1, (b + e) / 2 + 1, e, M, A, i, j);    
    
    //return the position where the overall    
    //minimum is    
    if (p1 == -1)    
        return M[node] = p2;    
    if (p2 == -1)    
        return M[node] = p1;    
    if (A[p1] <= A[p2])    
        return M[node] = p1;    
    return M[node] = p2;    
    
}    
    
    
int main()    
{    
    int M[MAXIND]; //�±�1���������,�����Ƕ�����,�����±��Žڵ��Ӧ������Сֵ���±�.    
    memset(M,-1,sizeof(M));    
    int a[]={3,4,5,7,2,1,0,3,4,5};    
    build(1, 0, sizeof(a)/sizeof(a[0])-1, M, a);    
    cout<<query(1, 0, sizeof(a)/sizeof(a[0])-1, M, a, 0, 5)<<endl;    
    return 0;    
}    