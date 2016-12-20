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
    //n0���ַ�����ģΪ���±�ĸ�����n1��n2��������
    int* s12  = new int[n02 + 3];
    s12[n02]= s12[n02+1]= s12[n02+2]=0;
    int* SA12 = new int[n02 + 3];
    SA12[n02]=SA12[n02+1]=SA12[n02+2]=0;
    int* s0   = new int[n0];
    int* SA0  = new int[n0];

    // generatepositions of mod 1 and mod  2 suffixes
    // the"+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1
    for (int i=0, j=0; i < n+(n0-n1);  i++) if (i%3 != 0) s12[j++] = i;
    //������ģ��Ϊ���±����s12��

    // lsb radix sortthe mod 1 and mod 2 triples
    radixPass(s12 , SA12, s+2, n02, K);
    radixPass(SA12, s12 , s+1, n02, K);
    radixPass(s12 , SA12, s  , n02, K);
    //radixPassʵ����һ����������
    //�Ժ�׺��ǰ�����ַ��������μ�����������˶�SA12����Ļ�������
    //��������ǳ����ģ�û�н�SA12�����������ź�����Ϊ��
    //��SA12�����м�����׺��ǰ�����ַ���ȣ�����ʼλ�ÿ�������ں���

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
        //name�Ǽ����׺����SA12��ǰ�����ַ�����ȫ��ͬ�ĺ�׺����
        //��ô�жϵ�ԭ���ǣ�SA12���򣬹�ֻ�����ں�׺��ǰ�����ַ��ſ�����ͬ
        if (SA12[i]% 3 == 1)
        {
            s12[SA12[i]/3]      = name;    // left half
        }
        else
        {
            s12[SA12[i]/3 + n0] = name;   // right half
        }
        //SA12[i]ģ���Ǿ��ǣ�s12������Ǻ�׺����SA12��ǰ�����ַ�����λ
    }

    // recurse if namesare not yet unique
    if (name <n02)
    {
        //���name����n02����ζ��SA12ǰ������ĸ������ȣ���SA12������
        //���򣬸���s12�ĺ�׺������SA12�ȼۣ���s12�ĺ�׺����������򼴿�
        suffixArray(s12, SA12, n02, name);
        // store uniquenames in s12 using the suffix array
        for (int i = 0;  i< n02;  i++) s12[SA12[i]] = i + 1;
    }
    else   // generate the suffix array of s12 directly
        for (int i = 0;  i< n02;  i++) SA12[s12[i] - 1] = i;
    //s12������Ǻ�׺����SA12��ǰ�����ַ�����λ
    //�����к�׺ǰ�����ַ�����һ��������£�s12���Ǻ�׺����λ

    //����SA12�������
    //SA12[i]�ǵ�iС�ĺ�׺�����(��Ŵӵ�n02)��s12[i]�����Ϊi�ĺ�׺����λ
    //ʹ�ú�׺��Ŷ�����ʵ��λ�õ�ԭ���ǵݹ����suffixArrayʱ���ܱ�������Ϣ

    // stably sort themod 0 suffixes from SA12 by their first character
    for (int i=0, j=0; i < n02;  i++) if (SA12[i] < n0) s0[j++] = 3*SA12[i];
    //��SA12�����е�ģΪ�ĺ�׺��ʵ��λ�ü�ȥ����洢��s0��
    //ע���׺��ŵ�ʵ��λ�õ�ת���轫ǰ�߳�
    //����ζ�������Ѿ�����ģΪ�ĺ�׺��SA0�����˳�������
    //ֻ��Ҫ����һ�μ������򼴿ɶ�SA0��ɻ�����������һ��
    radixPass(s0, SA0, s, n0, K);

    //���һ�����������SA12��SA0���й鲢
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
        //rank��SA���棬��SA[rank[i]] == i&&rank[SA[i]] == i
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
            //�����ǰ׺�ĵ�һ���ַ����±�����ʶǰ׺
            //��ô��j��ǰ׺i == SA[rank[i]]������ǰ׺
            while(s[i+ h] == s[j + h])
            {
                h++;
                //���û�й���h[i]��h[i+1]�Ĵ�С��ϵ�Ķ���h��Ҫ�ӿ�ʼ
            }
            height[rank[i]] = h;
            //�����h[i]��ֵ
            if(h> 0)
            {
                h--;
                //h[i+1]��ֵ���ڻ����h[i]-1
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
    //ÿһ����׺���������Ǹ���׺С����ô��׺�����������
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
    //������׺����
    /*
    Assert(s[n] == 0);
    Assert(s[n+1] ==0);
    //s���ַ������飬���׺����ʱ��Ӧ���ı�
    Assert(SA[n] == 0);
    Assert(SA[n+1] ==0);
    //����Ϊn���ַ�����n����׺
    Assert(isPermutation(SA,n));
    //��׺�������Ե�i(0<= i < n)���ַ���Ϊ���ĺ�׺������
    Assert(isSorted(SA,s, n));
    //��׺�����������
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
