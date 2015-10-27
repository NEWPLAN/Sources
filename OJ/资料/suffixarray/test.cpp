#include <cstdio>
#include <iostream>
#include <cstdlib>
#include<cstring>
using namespace std;
inline bool leq(int a1, int a2,   int b1, int b2)   // lexic. orderfor pairs
{
    return(a1< b1 || a1 == b1 && a2 <= b2);
}                                                  // and triples
inline bool leq(int a1, int a2, int a3,   int b1, int b2, int b3)
{
    return(a1< b1 || a1 == b1 && leq(a2,a3, b2,b3));
}
// stably sort a[0..n-1] to b[0..n-1] with keys in 0..K fromr
static void radixPass(int* a, int* b, int* r, int n, int K)
{
    // count occurrences
    int* c = new int[K + 1];                          // counter array
    for (int i = 0;  i<= K;  i++) c[i] = 0;         // resetcounters
    for (int i = 0;  i< n;  i++) c[r[a[i]]]++;    // countoccurences
    for (int i = 0, sum = 0; i <= K;  i++)   // exclusive prefix sums
    {
        int t =c[i];
        c[i] = sum;
        sum += t;
    }
    for (int i = 0;  i< n;  i++) b[c[r[a[i]]]++] =a[i];      //sort
    delete [] c;
}

// find the suffix array SA of s[0..n-1] in {1..K}^n
// require s[n]=s[n+1]=s[n+2]=0, n>=2
void suffixArray(int* s, int* SA, int n, int K)
{
    int n0=(n+2)/3, n1=(n+1)/3, n2=n/3, n02=n0+n2;
    //n0是字符串中模为的下标的个数，n1，n2依此类推
    int* s12  = new int[n02 + 3];
    s12[n02]= s12[n02+1]= s12[n02+2]=0;
    int* SA12 = new int[n02 + 3];
    SA12[n02]=SA12[n02+1]=SA12[n02+2]=0;
    int* s0   = new int[n0];
    int* SA0  = new int[n0];

    // generatepositions of mod 1 and mod  2 suffixes
    // the"+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1
    for (int i=0, j=0; i < n+(n0-n1);  i++) if (i%3 != 0) s12[j++] = i;
    //将所有模不为的下标存入s12中

    // lsb radix sortthe mod 1 and mod 2 triples
    radixPass(s12 , SA12, s+2, n02, K);
    radixPass(SA12, s12 , s+1, n02, K);
    radixPass(s12 , SA12, s  , n02, K);
    //radixPass实际是一个计数排序
    //对后缀的前三个字符进行三次计数排序完成了对SA12数组的基数排序
    //这个排序是初步的，没有将SA12数组真正地排好序，因为：
    //若SA12数组中几个后缀的前三个字符相等，则起始位置靠后的排在后面

    // findlexicographic names of triples
    int name = 0,c0 = -1, c1 = -1, c2 = -1;
    for (int i = 0;  i< n02;  i++)
    {
        if(s[SA12[i]] != c0 || s[SA12[i]+1] != c1 || s[SA12[i]+2] != c2)
        {
            name++;
            c0 = s[SA12[i]];
            c1 =s[SA12[i]+1];
            c2 = s[SA12[i]+2];
        }
        //name是计算后缀数组SA12中前三个字符不完全相同的后缀个数
        //这么判断的原因是：SA12有序，故只有相邻后缀的前三个字符才可能相同
        if (SA12[i]% 3 == 1)
        {
            s12[SA12[i]/3]      = name;    // left half
        }
        else
        {
            s12[SA12[i]/3 + n0] = name;   // right half
        }
        //SA12[i]模不是就是，s12保存的是后缀数组SA12中前三个字符的排位
    }

    // recurse if namesare not yet unique
    if (name <n02)
    {
        //如果name等于n02，意味着SA12前三个字母均不相等，即SA12已有序
        //否则，根据s12的后缀数组与SA12等价，对s12的后缀数组进行排序即可
        suffixArray(s12, SA12, n02, name);
        // store uniquenames in s12 using the suffix array
        for (int i = 0;  i< n02;  i++) s12[SA12[i]] = i + 1;
    }
    else   // generate the suffix array of s12 directly
        for (int i = 0;  i< n02;  i++) SA12[s12[i] - 1] = i;
    //s12保存的是后缀数组SA12中前三个字符的排位
    //在所有后缀前三个字符都不一样的情况下，s12就是后缀的排位

    //至此SA12排序完毕
    //SA12[i]是第i小的后缀的序号(序号从到n02)，s12[i]是序号为i的后缀的排位
    //使用后缀序号而不是实际位置的原因是递归调用suffixArray时不能保留该信息

    // stably sort themod 0 suffixes from SA12 by their first character
    for (int i=0, j=0; i < n02;  i++) if (SA12[i] < n0) s0[j++] = 3*SA12[i];
    //将SA12中所有的模为的后缀的实际位置减去按序存储在s0中
    //注意后缀序号到实际位置的转化需将前者乘
    //这意味着首先已经利用模为的后缀对SA0进行了初步排序
    //只需要采用一次计数排序即可对SA0完成基数排序的最后一步
    radixPass(s0, SA0, s, n0, K);

    //最后一步，对有序表SA12和SA0进行归并
    // merge sorted SA0suffixes and sorted SA12 suffixes
    for (int p=0, t=n0-n1,  k=0;  k < n; k++)
    {
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) *3 + 2)
        int i =GetI(); // pos of current offset 12 suffix
        int j =SA0[p]; // pos of current offset 0  suffix
        if (SA12[t]< n0 ?
                leq(s[i],       s12[SA12[t] + n0], s[j],       s12[j/3]) :
                leq(s[i],s[i+1],s12[SA12[t]-n0+1],s[j],s[j+1],s12[j/3+n0]))
        {
            // suffix fromSA12 is smaller
            SA[k] = i;
            t++;
            if (t ==n02)   // done --- only SA0 suffixes left
            {
                for(k++;  p < n0;  p++, k++) SA[k] = SA0[p];
            }
        }
        else
        {
            SA[k] = j;
            p++;
            if (p ==n0)    // done--- only SA12 suffixes left
            {
                for(k++;  t < n02;  t++, k++) SA[k] = GetI();
            }
        }
    }
    delete []s12;
    delete [] SA12;
    delete[] SA0;
    delete [] s0;
}
void suffixArrayHeight(int *s, int *SA, int n, int K, int *rank, int *height)
{
    int i, j,h;
    for(i = 0; i < n; i++)
    {
        rank[SA[i]] = i;
        //rank和SA互逆，即SA[rank[i]] == i&&rank[SA[i]] == i
    }
    h = 0;
    for(i = 0; i < n; i++)
    {
        if(rank[i]== 0)
        {
            height[rank[i]] = 0;
        }
        else
        {
            j = SA[rank[i] - 1];
            //如果用前缀的第一个字符的下标来标识前缀
            //那么，j是前缀i == SA[rank[i]]的左邻前缀
            while(s[i+ h] == s[j + h])
            {
                h++;
                //如果没有关于h[i]和h[i+1]的大小关系的定理，h需要从开始
            }
            height[rank[i]] = h;
            //求出了h[i]的值
            if(h> 0)
            {
                h--;
                //h[i+1]的值大于或等于h[i]-1
            }
        }
    }
}
#define maxNum (100002 * 2 + 3)
/*
int s[maxNum];
int SA[maxNum];
int rank[maxNum];
int height[maxNum];*/
bool isPermutation(int *SA, int n)
{
    bool *seen = new bool[n];
    for (int i = 0;  i< n;  i++) seen[i] = 0;
    for (int i = 0;  i< n;  i++) seen[SA[i]] = 1;
    for (int i = 0;  i< n;  i++) if(!seen[i]) return 0;
    return 1;
}
bool sleq(int *s1, int *s2)
{
    if (s1[0]< s2[0]) return 1;
    if (s1[0]> s2[0]) return 0;
    return sleq(s1+1, s2+1);
}
// is SA a sorted suffix array for s?
bool isSorted(int *SA, int *s, int n)
{
    for (int i = 0;  i< n-1;  i++)
    {
        if(!sleq(s+SA[i], s+SA[i+1])) return 0;
    }
    //每一个后缀都比其后的那个后缀小，那么后缀数组是升序的
    return 1;
}
#define Assert(c) if(!(c))\
  {cout << "\nAssertionviolation " << __FILE__ << ":"<< __LINE__ << endl;}
int main()
{
    int i, j;
    int n;
    int b = 'z' - 'a' + 2;

    int* s;
    int* SA;
    int* rank;
    int*height;
    char str1[100002];
    char str2[100002];
    int length1;
    int length2;
    int max;
    scanf("%s%s",str1, str2);
    length1 = strlen(str1);
    length2 = strlen(str2);
    n = length1 + length2 + 1;
    s = new int[n+3];
    SA = new int[n+3];
    rank = new int[n];
    height = new int[n];
    s[n] = s[n+1] = s[n+2] = SA[n] = SA[n+1] =SA[n+2] = 0;
    for(i = 0; i < length1; i++)
    {
        s[i] = str1[i] - 'a' + 1;
    }
    s[i++] = 27;
    for(j = 0; j < length2; j++)
    {
        s[i++] = str2[j] - 'a' + 1;
    }
    suffixArray(s, SA, n, b);
    //构建后缀数组
    /*
    Assert(s[n] == 0);
    Assert(s[n+1] ==0);
    //s是字符串数组，求后缀数组时不应被改变
    Assert(SA[n] == 0);
    Assert(SA[n+1] ==0);
    //长度为n的字符串有n个后缀
    Assert(isPermutation(SA,n));
    //后缀数组是以第i(0<= i < n)个字符作为起点的后缀的数组
    Assert(isSorted(SA,s, n));
    //后缀数组必须有序
    */
    suffixArrayHeight(s, SA, n, b, rank,height);
    max = 0;
    for(i = 1; i < n; i++)
    {
        if((SA[i]< length1 && SA[i - 1] > length1)||
                (SA[i] > length1 && SA[i -1] < length1))
        {
            if(max< height[i])
                max = height[i];
        }
    }
    cout << max << endl;
    delete []s;
    delete [] SA;
    delete[] rank;
    delete [] height;
}
