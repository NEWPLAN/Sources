/////////////////////////////////////////////////////////////////
//Constructing Suffix Array with Doubling Algorithm, O(n log n).
/////////////////////////////////////////////////////////////////
#include <algorithm>//sort
#include <cstring>//memsetusing namespace std;const int MAX_SFX = 210000;struct Sfx {
    int i;  int key[2];
    bool operator < (const Sfx& s) const
    {   return key[0] < s.key[0]
               || key[0] == s.key[0] && key[1] < s.key[1]; }
};int g_buf[MAX_SFX + 1];
Sfx g_tempSfx[2][MAX_SFX], *g_sa = g_tempSfx[0];void cSort(Sfx* in, int n, int key, Sfx* out) {
    int* cnt = g_buf;  memset( cnt, 0, sizeof(int) * (n + 1) );
    for (int i = 0; i < n; i++) { cnt[ in[i].key[key] ]++; }
    for (int i = 1; i <= n; i++) { cnt[i] += cnt[i - 1]; }
    for (int i = n - 1; i >= 0; i--)
        { out[ --cnt[ in[i].key[key] ] ] = in[i]; }
}//Build a suffix array from string 'text' whose length is 'len'.
//write the result into global array 'g_sa'.
void buildSA(char* text, int len) {
    Sfx *temp = g_tempSfx[1];
    int* rank = g_buf;
    for (int i = 0; i < len; i++)
        { g_sa[i].i = g_sa[i].key[1] = i;  g_sa[i].key[0] = text[i]; }
    sort(g_sa, g_sa + len);
    for (int i = 0; i < len; i++) { g_sa[i].key[1] = 0; }
    int wid = 1;
    while (wid < len) {
        rank[ g_sa[0].i ] = 1;
        for (int i = 1; i < len; i++)
        {   rank[ g_sa[i].i ] = rank[ g_sa[i - 1].i ];
            if ( g_sa[i-1] < g_sa[i] ) { rank[ g_sa[i].i ]++; } }
        for (int i = 0; i < len; i++)
            { g_sa[i].i = i;  g_sa[i].key[0] = rank[i];
              g_sa[i].key[1] = i + wid < len? rank[i + wid]: 0; }
        cSort(g_sa, len, 1, temp);  cSort(temp, len, 0, g_sa);
        wid *= 2;
    }
}int getLCP(char* a, char* b)
{ int l=0;  while(*a && *b && *a==*b) { l++; a++; b++; }  return l; }void getLCP(char* text, Sfx* sfx, int len, int* lcp) {
    int* rank = g_buf;
    for (int i=0, r=0; i < len; i++, r++) { rank[ sfx[i].i ] = r; }
    lcp[0] = 0;
    if (rank[0])
        { lcp[ rank[0] ] = getLCP( text, text + sfx[ rank[0]-1 ].i ); }
    for (int i = 1; i < len; i++) {
        if ( !rank[i] ) { continue; }
        if (lcp[ rank[i - 1] ] <= 1)
        { lcp[ rank[i] ] = getLCP( text+i, text+sfx[ rank[i]-1 ].i ); }
        else
        { int L = lcp[ rank[i - 1] ] - 1;
          lcp[rank[i]] = L+getLCP(text+i+L, text+sfx[rank[i]-1].i+L); }
    }
}//Test suite and usage example
#include <iostream>
using namespace std;
int main() {
    char str[] = "aabbaa\0ababab";
    int from[] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
    int lcp[13];
    buildSA(str, 13);  getLCP(str, g_sa, 13, lcp);
    for (int i=1; i<13; i++)//The first suffix is useless (empty).
    { cout<<from[g_sa[i].i]<<' '<<str+g_sa[i].i<<' '<<lcp[i]<<endl; }
    return 0;//output: 0 a 0
             //        0 aa 1
             //        0 aabbaa 2
             //        1 ab 1
             //        1 abab 2
             //        1 ababab 4
             //        0 abbaa 2
             //        1 b 0
             //        0 baa 1
             //        1 bab 2
             //        1 babab 3
             //        0 bbaa 1
}