/**查询区间最值下标---min**/
#include<iostream>    
  
using namespace std;    
    
#define MAXN 100    
#define MAXIND 256 //线段树节点个数    
    
//构建线段树,目的:得到M数组.    
void build(int node, int b, int e, int M[], int A[])    
{    
    if (b == e)    
        M[node] = b; //只有一个元素,只有一个下标    
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
    
//找出区间 [i, j] 上的最小值的索引    
int query(int node, int b, int e, int M[], int A[], int i, int j)    
{    
    int p1, p2;    
    
    //查询区间和要求的区间没有交集    
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
    int M[MAXIND]; //下标1起才有意义,否则不是二叉树,保存下标编号节点对应区间最小值的下标.    
    memset(M,-1,sizeof(M));    
    int a[]={3,4,5,7,2,1,0,3,4,5};    
    build(1, 0, sizeof(a)/sizeof(a[0])-1, M, a);    
    cout<<query(1, 0, sizeof(a)/sizeof(a[0])-1, M, a, 0, 5)<<endl;    
    return 0;    
}    