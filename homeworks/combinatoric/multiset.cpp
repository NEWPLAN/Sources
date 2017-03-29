/*************************************************************************
	> File Name: multiset.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri 24 Mar 2017 12:38:05 PM CST
 ************************************************************************/

#include <iostream>
#include <cstring>
 
using namespace std;
 
#define N 6
 
void Swap(char *pa, char *pb);
void FullPermutation(char *str, int k, int n);
int IsAppeared(char *str, char t, int begin, int end);
 
char str[N+1] = "ADCDBA";
 
int main()
{
    FullPermutation(str, 0, N);
    return 0;
}
 
 
void Swap(char *pa, char *pb)
{
    if(pa != pb)
    {
        char tmp = *pa;
        *pa = *pb;
        *pb = tmp;
    }
}
 
//判断字符t在字符串的下标begin到end处是否出现过
int IsAppeared(char *str, char t, int begin, int end)
{
    for(int j=begin; j<=end; ++j)
    {
        if(t == str[j])
            return 1;
    }
    return 0;
}
 
/*对字符串进行全排列,注意该函数处理了字符重复的情况,字符重复的情况有两种：
    1. str[i]本身和后面的str[k]相同
   2. str[k]在k+1到i-1的下标之间已经出现过(用IsAppeared()函数去判断)
*/
void FullPermutation(char *str, int k, int n)
{
    if(k == n)
    {
        cout<<str<<endl;
        return;
    }
 
    for(int i=k; i<n; ++i)
    {
        if(i!=k && (str[i]==str[k]) || IsAppeared(str,str[i],k+1,i-1)) ////用以处理元素重复的情况
            continue;
        Swap(str+k, str+i);
        FullPermutation(str, k+1, n);
        Swap(str+k, str+i);
    }
}
