
#include <iostream>  
using namespace std;  
  
const int maxind = 256;  
int segTree[maxind * 4 + 10];  
int array[maxind];   
/* ���캯�����õ��߶��� */  
void build(int node, int begin, int end)    
{    
    if (begin == end)    
        segTree[node] = array[begin]; /* ֻ��һ��Ԫ��,�ڵ��¼�õ�Ԫ�� */  
    else    
    {     
        /* �ݹ鹹���������� */   
        build(2*node, begin, (begin+end)/2);    
        build(2*node+1, (begin+end)/2+1, end);   
           
        /* ����ʱ�õ���ǰnode�ڵ���߶���Ϣ */    
        if (segTree[2 * node] <= segTree[2 * node + 1])    
            segTree[node] = segTree[2 * node];    
        else    
            segTree[node] = segTree[2 * node + 1];    
    }    
}  

int query(int node, int begin, int end, int left, int right)    
{   
    int p1, p2;    
    
    /*  ��ѯ�����Ҫ�������û�н���  */  
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

void Updata(int node, int begin, int end, int ind, int add)/*���ڵ����*/    
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
    /*���ݸ��¸��ڵ�*/    
    segTree[node] = min(segTree[node * 2], segTree[node * 2 + 1]);     
         
}   

void Change(node *p, int a, int b) /* ��ǰ������Ϊp���޸�����Ϊ(a,b]*/  
   
{  
   
  if (a <= p->Left && p->Right <= b)  
   
  /* �����ǰ��������������޸�������*/  
   
  {  
   
     //...... /* �޸ĵ�ǰ������Ϣ�������ϱ��*/  
   
     return;  
   
  }  
   
  Push_Down(p); /* �ѵ�ǰ���ı�����´���*/  
   
  int mid = (p->Left + p->Right) / 2; /* ���������ӽ��ķָ��� 
  
  if (a < mid) Change(p->Lch, a, b); /* �������н������������ӽ��*/  
   
  if (b > mid) Change(p->Rch, a, b); /* ���Һ����н������������ӽ��*/  
   
  Update(p); /* ά����ǰ������Ϣ����Ϊ���ӽ�����Ϣ�����и��ģ�*/  
   
}  

int main()  
{  
    array[0] = 1, array[1] = 2,array[2] = 2, array[3] = 4, array[4] = 1, array[5] = 3;  
    build(1, 0, 5);  
    for(int i = 1; i<=20; ++i)  
     cout<< "seg"<< i << "=" <<segTree[i] <<endl;  
    return 0;  
}   

