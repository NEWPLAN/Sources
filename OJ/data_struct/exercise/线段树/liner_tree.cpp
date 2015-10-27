
#include <iostream>  
using namespace std;  
  
const int maxind = 256;  
int segTree[maxind * 4 + 10];  
int array[maxind];   
/* 构造函数，得到线段树 */  
void build(int node, int begin, int end)    
{    
    if (begin == end)    
        segTree[node] = array[begin]; /* 只有一个元素,节点记录该单元素 */  
    else    
    {     
        /* 递归构造左右子树 */   
        build(2*node, begin, (begin+end)/2);    
        build(2*node+1, (begin+end)/2+1, end);   
           
        /* 回溯时得到当前node节点的线段信息 */    
        if (segTree[2 * node] <= segTree[2 * node + 1])    
            segTree[node] = segTree[2 * node];    
        else    
            segTree[node] = segTree[2 * node + 1];    
    }    
}  

int query(int node, int begin, int end, int left, int right)    
{   
    int p1, p2;    
    
    /*  查询区间和要求的区间没有交集  */  
    if (left > end || right < begin)    
        return -1;    
    
    /*  if the current interval is included in  */    
    /*  the query interval return segTree[node]  */  
    if (begin >= left && end <= right)    
        return segTree[node];    
    
    /*  compute the minimum position in the  */  
    /*  left and right part of the interval  */   
    p1 = query(2 * node, begin, (begin + end) / 2, left, right);   
    p2 = query(2 * node + 1, (begin + end) / 2 + 1, end, left, right);    
    
    /*  return the expect value  */   
    if (p1 == -1)    
        return p2;    
    if (p2 == -1)    
        return p1;    
    if (p1 <= p2)    
        return  p1;    
    return  p2;      
}   

void Updata(int node, int begin, int end, int ind, int add)/*单节点更新*/    
{    
    
    if( begin == end )    
    {    
        segTree[node] += add;    
        return ;    
    }    
    int m = ( left + right ) >> 1;    
    if(ind <= m)    
        Updata(node * 2,left, m, ind, add);    
    else    
        Updata(node * 2 + 1, m + 1, right, ind, add);    
    /*回溯更新父节点*/    
    segTree[node] = min(segTree[node * 2], segTree[node * 2 + 1]);     
         
}   

void Change(node *p, int a, int b) /* 当前考察结点为p，修改区间为(a,b]*/  
   
{  
   
  if (a <= p->Left && p->Right <= b)  
   
  /* 如果当前结点的区间包含在修改区间内*/  
   
  {  
   
     //...... /* 修改当前结点的信息，并标上标记*/  
   
     return;  
   
  }  
   
  Push_Down(p); /* 把当前结点的标记向下传递*/  
   
  int mid = (p->Left + p->Right) / 2; /* 计算左右子结点的分隔点 
  
  if (a < mid) Change(p->Lch, a, b); /* 和左孩子有交集，考察左子结点*/  
   
  if (b > mid) Change(p->Rch, a, b); /* 和右孩子有交集，考察右子结点*/  
   
  Update(p); /* 维护当前结点的信息（因为其子结点的信息可能有更改）*/  
   
}  

int main()  
{  
    array[0] = 1, array[1] = 2,array[2] = 2, array[3] = 4, array[4] = 1, array[5] = 3;  
    build(1, 0, 5);  
    for(int i = 1; i<=20; ++i)  
     cout<< "seg"<< i << "=" <<segTree[i] <<endl;  
    return 0;  
}   

