#include <iostream>
#include <stdio.h>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
#define N 50500*2
const double PI = acos(-1.0);
struct Vir
{
    double re, im;
    Vir(double _re = 0., double _im = 0.): re(_re), im(_im) {}
    Vir operator*(Vir r) { return Vir(re * r.re - im * r.im, re * r.im + im * r.re);}
    Vir operator+(Vir r) { return Vir(re + r.re, im + r.im);}
    Vir operator-(Vir r) { return Vir(re - r.re, im - r.im);}
};
void bit_rev(Vir *a, int loglen, int len)
{
    for (int i = 0; i < len; ++i)
    {
        int t = i, p = 0;
        for (int j = 0; j < loglen; ++j)
        {
            p <<= 1;
            p = p | (t & 1);
            t >>= 1;
        }
        if (p < i)
        {
            Vir temp = a[p];
            a[p] = a[i];
            a[i] = temp;
        }
    }
}
void FFT(Vir *a, int loglen, int len, int on)
{
    bit_rev(a, loglen, len);

    for (int s = 1, m = 2; s <= loglen; ++s, m <<= 1)
    {
        Vir wn = Vir(cos(2 * PI * on / m), sin(2 * PI * on / m));
        for (int i = 0; i < len; i += m)
        {
            Vir w = Vir(1.0, 0);
            for (int j = 0; j < m / 2; ++j)
            {
                Vir u = a[i + j];
                Vir v = w * a[i + j + m / 2];
                a[i + j] = u + v;
                a[i + j + m / 2] = u - v;
                w = w * wn;
            }
        }
    }
    if (on == -1)
    {
        for (int i = 0; i < len; ++i) a[i].re /= len, a[i].im /= len;
    }
}
char a[N * 2], b[N * 2];
Vir pa[N * 2], pb[N * 2];
int ans[N * 2];
int main (int argc, char** argv)
{
    while (scanf("%s%s", a, b) != EOF)
    {
        int lena = strlen(a);
        int lenb = strlen(b);
        int n = 1, loglen = 0;
        while (n < lena + lenb) n <<= 1, loglen++;
        for (int i = 0, j = lena - 1; i < n; ++i, --j)
            pa[i] = Vir(j >= 0 ? a[j] - '0' : 0., 0.);
        for (int i = 0, j = lenb - 1; i < n; ++i, --j)
            pb[i] = Vir(j >= 0 ? b[j] - '0' : 0., 0.);
        for (int i = 0; i <= n; ++i) ans[i] = 0;

        FFT(pa, loglen, n, 1);
        FFT(pb, loglen, n, 1);
        for (int i = 0; i < n; ++i)
            pa[i] = pa[i] * pb[i];
        FFT(pa, loglen, n, -1);

        for (int i = 0; i < n; ++i) ans[i] = pa[i].re + 0.5;
        for (int i = 0; i < n; ++i) ans[i + 1] += ans[i] / 10, ans[i] %= 10;

        int pos = lena + lenb - 1;
        for (; pos > 0 && ans[pos] <= 0; --pos) ;
        for (; pos >= 0; --pos) printf("%d", ans[pos]);
        puts("");
    }
    return 0;
}