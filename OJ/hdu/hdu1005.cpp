/*因为f[i]只能取0~7，下面的程序用m[x][y],记录f[i]的值x y相邻时候出现过。

鸽巢原理知,状态总数不会超过7*7
View Code
*/
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

const int N = 100;
int f[N], m[8][8];

int main()
{
    int n, a, b, k, x, y;
    while (scanf("%d%d%d", &a, &b, &n) != EOF && a+b+n)
    {
        memset(m, 0, sizeof(m));
        f[1] = f[2] = x = y = 1;
        k = 3;
        while (!m[x][y])
        {
            m[x][y] = k;
            cout<<"m["<<x<<"]"<<"["<<y<<"]="<<k<<endl;
            f[k] = y = (a * y + b * x) % 7;
            x = f[k-1];
            k++;
        }
        int h = m[x][y];cout<<h<<" "<<k-h<<endl;
        if (n < k)
            printf("%d\n", f[n]);
        else printf("%d\n", f[(n-h)%(k-h)+h]);
    }
}

